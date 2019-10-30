
#include "file_system.h"
#include "string.h"

//Funcion para concatenar la direccion del archivo a la de la carpeta raiz del file system
char* getAddress(char* file){
  char* address = (char*) malloc(sizeof(char) * 32);
  strcpy(address, "FS/");
  strcat(address,file);
  return address;
}

int* write_1_svc(write_data *argp, struct svc_req *rqstp) {

	char* address = (char*) malloc(sizeof(char) * 32);
	address = getAddress(argp->file_name);

	FILE* fp = fopen(address, "a");
	if(fp) {
		printf("Se escribieron %d bytes en el archivo: '%s'\n",argp->data.data_len,address);
		fwrite(argp->data.data_val, argp->data.data_len, sizeof(char), fp);
	} else {
		printf("Error tratando de escribir a archivo: '%s'\n",address);
	}

	fclose(fp);
	return &argp->data.data_len;
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

		int bytes_to_read = (argp->amount < 1024) ? argp->amount : 1024;

		result.data.data_len = fread(file, sizeof(char), bytes_to_read, fp);
		result.data.data_val = file;
		fseek(fp, 0L, SEEK_END);

		result.finished = (ftell(fp) <= (result.data.data_len + argp->pos)) ? 1 : 0;

		printf("Se leyeron %d bytes de: '%s', desde la posicion %d \n",result.data.data_len,address,argp->pos);
	} else {
		printf("Error tratando de abrir archivo inexistente: '%s'\n",address);
		result.error = 1;
	}

	fclose(fp);
	return &result;
}
