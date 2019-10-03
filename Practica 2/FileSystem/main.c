#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getAddress(char* file){
  char* address = (char*) malloc(sizeof(char) * 100);
  strcpy(address, "FS/");
  strcat(address,file);
  return address;
}

char* read(char* file_name, int pos, int amount) {
  FILE* fp;
  char* address = (char*) malloc(sizeof(char) * 100);
  address = getAddress(file_name);

  char* file = (char*) malloc(sizeof(char)*amount);
  fp = fopen(address, "r");
  fseek(fp, pos, 0);
  if(fp) {
    fgets(file, amount, fp);
  }
  fclose(fp);
  return file;
}

int write(char* file_name, int amount, char* data) {
  FILE* fp;
  char* address = (char*) malloc(sizeof(char) * 100);
  address = getAddress(file_name);

  fp = fopen(address, "a");
  int i = -1;
  if(fp) {
    for (i = 0; i < amount && i < strlen(data); i++) {
      fputc(data[i],fp);
    }
  }
  return i;
}

int main() {
  char* file = read("arch.txt", 0, 50);
  int amount = write("nuevo.txt",500,"holssssssssssssssssssssssssssssss");
  printf("%s \n",file);
  printf("%d \n",amount);
}