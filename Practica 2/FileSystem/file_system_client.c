#include "file_system.h"
#include "stdio.h"
#include "string.h"

int rcv_file(read_data read_arg, CLIENT* clnt) {

	file_data* result;
	result = read_1(&read_arg, clnt);

	if ((result == (file_data *) NULL) || (result->error == 1)) {
		clnt_perror (clnt, "call failed");
		return -1;
	} else {
		FILE* fp = fopen(read_arg.file_name, "w");
		fwrite(result->data.data_val, sizeof(char), result->data.data_len, fp);
		if(!result->finished) {
			while(!result->finished) {
				read_arg.pos = read_arg.pos + result->data.data_len;
				result = read_1(&read_arg, clnt);
				fwrite(result->data.data_val, sizeof(char), result->data.data_len, fp);
			}
		}
		fclose(fp);
		return 0;
	}
}

int send_file(write_data write_arg, CLIENT* clnt) {

	int result = 0;
	FILE* fp = fopen(write_arg.file_name, "r");
	write_arg.data.data_val = malloc(1024*sizeof(char));
	int bytes_to_send = 1024;

	if(fp) {
		while(!feof(fp)){
			write_arg.data.data_len = fread(write_arg.data.data_val, sizeof(char), bytes_to_send, fp);
			write_arg.amount = write_arg.data.data_len;

			int* bytes_written;
			bytes_written = write_1(&write_arg, clnt);

			if (bytes_written == (int *) NULL) {
				clnt_perror(clnt, "call failed");
			} else {
				result += *bytes_written;
			}
		}
	} else {
		result = -1;
	}
	fclose(fp);
	return result;
}

void file_system_1(char *host) {
	CLIENT *clnt;
	file_data  *result_2;
	int  *result_1;

#ifndef	DEBUG
	clnt = clnt_create (host, file_system, display_ver, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	char* buffer;
	char* pch;
	size_t bufsize = 256;

	buffer = (char *)malloc(bufsize * sizeof(char));

	while (1){
    getline(&buffer, &bufsize, stdin);

    pch = strtok (buffer, " ");

		if(!strcmp(pch,"send")) {
			write_data  write_arg;
			write_arg.file_name = strtok (NULL, " \n");

			int bytes_written = send_file(write_arg,clnt);
			if(bytes_written != -1) {
				printf("%d bytes were written\n",bytes_written);
			} else {
				printf("Unable to send file");
			}
			continue;
		}

		if (!strcmp(pch,"get")) {
			read_data read_arg;
			read_arg.file_name = strtok (NULL, " \n");
			read_arg.amount = atoi(strtok (NULL, " \n"));
			read_arg.pos = 0;

			rcv_file(read_arg, clnt);
			continue;
		}

		if (!strcmp(pch,"run")) {
			//Reading file from server

			// read_data read_arg;
			// read_arg.file_name = strtok (NULL, " \n");
			// read_arg.amount = 32768;
			// read_arg.pos = 0;

			// result_2 = read_local(read_arg, clnt);
			// if (result_2 == (file_data *) NULL) {
			// 	clnt_perror (clnt, "call failed");
			// } else {
			// 	//Copying file to local
			// 	FILE* fp;
			// 	fp = fopen(read_arg.file_name, "w");
			// 	printf("%s\n",result_2->data.data_val);
			// 	if(fp) {
			// 		fputs(result_2->data.data_val,fp);
  		// 	}
  		// 	fclose(fp);

			// 	//Writing file to server
			// 	write_data  write_arg;
			// 	write_arg.file_name = strcat(read_arg.file_name,"_copy");
			// 	write_arg.amount = strlen(result_2->data.data_val);
			// 	write_arg.data.data_val = result_2->data.data_val;
				
			// 	write_local(write_arg,clnt);
			// }
			continue;
		}		
		printf("Command not recognized, use send 'file_name' or get 'file_name' 'amount'\n");
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int main (int argc, char *argv[]) {
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	file_system_1 (host);
	exit (0);
}
