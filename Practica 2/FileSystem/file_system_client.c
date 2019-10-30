#include "file_system.h"
#include "stdio.h"
#include "string.h"

int rcv_file(read_data read_arg, char* destiny, CLIENT* clnt) {

	file_data* result;
	result = read_1(&read_arg, clnt);

	if ((result == (file_data *) NULL) || (result->error == 1)) {
		clnt_perror (clnt, "call failed");
		return -1;
	} else {
		FILE* fp = fopen(destiny, "w");
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

int send_file(write_data write_arg, char* source, CLIENT* clnt) {

	int result = 0;
	FILE* fp = fopen(source, "r");
	write_arg.data.data_val = malloc(1024*sizeof(char));
	int bytes_to_send = 1024;

	if(fp) {
		while(!feof(fp)){
			write_arg.data.data_len = fread(write_arg.data.data_val, sizeof(char), bytes_to_send, fp);
			
			int* bytes_written;
			bytes_written = write_1(&write_arg, clnt);

			if (bytes_written == (int *) NULL) {
				clnt_perror(clnt, "call failed");
			} else {
				result += *bytes_written;
			}
		}
		fclose(fp);
	} else {
		result = -1;
	}
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

    pch = strtok(buffer, " ");

		if (!strcmp(pch,"get")) {
			read_data read_arg;
			read_arg.file_name = strtok(NULL, " \n");
			char* destiny = strtok(NULL, " \n");

			char* amount_st = strtok(NULL, " \n");
			read_arg.amount = (amount_st != NULL) ? atoi(amount_st) : -1;
			
			read_arg.pos = 0;
			
			if(read_arg.file_name == NULL || destiny == NULL || amount_st == NULL) {
				printf("Faltan parametros, usar: 'get file_name destiny amount'\n");
			} else rcv_file(read_arg, destiny, clnt);
			continue;
		}	

		if(!strcmp(pch,"send")) {
			write_data  write_arg;
			char* source = strtok(NULL, " \n");
			write_arg.file_name = strtok(NULL, " \n");
			if(write_arg.file_name == NULL) write_arg.file_name = source;
			int bytes_written = -1;

			if(source == NULL) {
				printf("Faltan parametros, usar: 'send file_name [source]'\n");
			} else bytes_written = send_file(write_arg,source,clnt);

			if(bytes_written != -1) {
				printf("Se enviaron %d bytes\n",bytes_written);
			} else {
				printf("No se pudo enviar el archivo\n");
			}
			continue;
		}
		printf("Comando erroneo, usar 'get' o 'send'\n");
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
