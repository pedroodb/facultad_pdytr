#include "file_system.h"
#include "stdio.h"
#include "string.h"

int read_local(read_data read_arg, CLIENT* clnt) {

	file_data *result;
	result = read_1(&read_arg, clnt);

	if ((result == (file_data *) NULL) || (result->error == 1)) {
		clnt_perror (clnt, "call failed");
	} else {
		FILE* fp = fopen(read_arg.file_name, "a");
		fwrite(result->data.data_val, sizeof(char), result->data.data_len, fp);
		fclose(fp);
		return 1;
	}
}

int* write_local(write_data write_arg, CLIENT* clnt) {
	int  *result;
	result = write_1(&write_arg, clnt);

	if (result == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	} else {
		FILE* fp = fopen(write_arg.file_name, "r");

		int bytes_left = write_arg.amount;
		int bytes_to_send = 4096;

		// while(!feof(fp) && (bytes_left > 0)){
		// 	if(bytes_left < 4096) {
		// 		bytes_to_send = bytes_left;
		// 	}
		// 	write_arg.data.data_len = fread(write_arg.data.data_val, sizeof(char), bytes_to_send, fp);
		// 	write_arg.amount = write_arg.data.data_len;
		// 	bytes_left -= write_arg.data.data_len;

		// 	printf("am:%d,   name:%s",write_arg.amount,write_arg.file_name);
			
		// 	result = write_1(&write_arg, clnt);
		// 	break;
		// }
		fclose(fp);
	}
	return result;
} 

void
file_system_1(char *host)
{
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

		if(!strcmp(pch,"write")) {
			write_data  write_arg;
			write_arg.file_name = strtok (NULL, " ");
			write_arg.amount = atoi(strtok (NULL, " "));

			int* write_result = write_local(write_arg,clnt);
			printf("%d bytes were written\n",*write_result);
			continue;
		}

		if (!strcmp(pch,"read")) {
			read_data read_arg;
			read_arg.file_name = strtok (NULL, " \n");
			read_arg.amount = atoi(strtok (NULL, " \n"));
			read_arg.pos = atoi(strtok (NULL, " \n"));

			read_local(read_arg, clnt);
			continue;
		}

		if (!strcmp(pch,"run")) {
			//Reading file from server

			read_data read_arg;
			read_arg.file_name = strtok (NULL, " \n");
			read_arg.amount = 32768;
			read_arg.pos = 0;

			result_2 = read_local(read_arg, clnt);
			if (result_2 == (file_data *) NULL) {
				clnt_perror (clnt, "call failed");
			} else {
				//Copying file to local
				FILE* fp;
				fp = fopen(read_arg.file_name, "w");
				printf("%s\n",result_2->data.data_val);
				if(fp) {
					fputs(result_2->data.data_val,fp);
  			}
  			fclose(fp);

				//Writing file to server
				write_data  write_arg;
				write_arg.file_name = strcat(read_arg.file_name,"_copy");
				write_arg.amount = strlen(result_2->data.data_val);
				write_arg.data.data_val = result_2->data.data_val;
				
				write_local(write_arg,clnt);
			}
			continue;
		}		
		printf("Command not recognized\n");
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
