#include "auxFunc.h"

int numberSpaces(char* string){
  int c=0;
  for(int i=0;i<strlen(string);i++){
    if(string[i] == ' '){
      c++;
    }
  }
  return c;
}

char* juntapids(char* argv[], int n) {
    char* buffer = malloc(BUFFER_SIZE);

    buffer[0] = '\0';

    for (int i = 0; i < n; i++) {
        strcat(buffer, argv[i+2]);
        strcat(buffer, ",");
    }
    buffer[strlen(buffer)-1] = '\0';
    return buffer;
}

int numberVirgulas(char* string){
  int c=0;
  for(int i=0;i<strlen(string);i++){
    if(string[i] == ','){
      c++;
    }
  }
  return c;
}

int numberBar(char* string){
  int c=0;
  for(int i=0;i<strlen(string);i++){
    if(string[i] == '|'){
      c++;
    }
  }
  return c;
}

int* arraypid(char* msg, int n) {
  int* array = malloc(n * sizeof(int));
  int i = 0;
  char* aux = strtok(msg, ",");
  while (aux != NULL && i < n) {
    array[i] = atoi(aux);
    aux = strtok(NULL, ",");
    i++;
  }
  return array;
}


int get_pid(int fifo, int tamanho){
    char pid[tamanho + 1];
    int n = read(fifo, pid, tamanho);
    pid[n] = '\0';
    return atoi(pid);
}