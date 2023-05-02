#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "auxFunc.h"
#include "../mensagem/mensagem.h"

char* cleanArguments(char* input){
    char modified[strlen(input) + 1]; 
    int i = 0, j = 0;
    while (input[i] != '\0') {
        while(input[i] != ' ' && input[i] != '\0'){
          modified[j++] = input[i];
          i++;
        }
        while(input[i] != '|' && input[i] != '\0'){
          i++;
        }
        if(input[i] != '\0'){
          modified[j++] = ' ';
          modified[j++] = '|';
          modified[j++] = ' ';
          i++;
          if(input[i] == ' ') i++;
        }
    }
    modified[j] = '\0';
    return strdup(modified);
}

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

int* arraypid(char* msg, int n){
  int* array = malloc((n + 1) * sizeof(int));
  int i = 0;
  char* aux = strtok(msg, ",");
  array[i] = atoi(aux);
  while (aux != NULL) {
    aux = strtok(NULL, ",");
    if (aux != NULL) {
      i++;
      array[i] = atoi(aux);
    }
  }
  return array; 
}

int get_pid(int fifo, int tamanho){
    char pid[tamanho + 1];
    int n = read(fifo, pid, tamanho);
    pid[n] = '\0';
    return atoi(pid);
}