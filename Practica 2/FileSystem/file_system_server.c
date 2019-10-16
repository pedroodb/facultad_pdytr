
#include "file_system.h"
#include "string.h"

//Funcion para concatenar la direccion del archivo a la de la carpeta raiz del file system
char* getAddress(char* file){
  char* address = (char*) malloc(sizeof(char) * 100);
  strcpy(address, "FS/");
  strcat(address,file);
  return address;
}

int* write_1_svc(write_data *argp, struct svc_req *rqstp) {
	static int min = -1;

	char* address = (char*) malloc(sizeof(char) * 100);
	address = getAddress(argp->file_name);

	FILE* fp = fopen(address, "a");
	if(fp) {
		min = (argp->data.data_len < argp->amount) ? argp->data.data_len : argp->amount;
		printf("Writting into file: '%s', %d bytes\n",address,min);
		fwrite(argp->data.data_val, min, sizeof(char), fp);
	} else {
		printf("Error on trying to write into non existent file: '%s'\n",address);
	}

	fclose(fp);
	return &min;
}

file_data* read_1_svc(read_data *argp, struct svc_req *rqstp) {
	static file_data result;

	char * file;
	FILE* fp;
	char* address = (char*) malloc(sizeof(char) * 32);
	address = getAddress(argp->file_name);

	file = (char*) malloc(sizeof(char)*argp->amount);
	fp = fopen(address, "r");
	
	if(fp) {
		fseek(fp, argp->pos, 0);
		printf("Reading from file: '%s', %d chars since position %d \n",address,argp->amount,argp->pos);
		fread(file, sizeof(char), argp->amount, fp);
		result.data.data_val = file;
		result.data.data_len = ftell(fp);
		fseek(fp, 0L, SEEK_END);
		result.finished = (ftell(fp) > result.data.data_len) ? 1 : 0;
	} else {
		printf("Error on trying to read from non existent file: '%s'\n",address);
		result.error = 1;
	}

	fclose(fp);
	return &result;
}
