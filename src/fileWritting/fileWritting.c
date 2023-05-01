#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "fileWritting.h"
#include "../mensagem/mensagem.h"

void writeLine(int pid,char *fname, char* msg, long int st_sec, long int st_milisec){
    FILE *fp = fopen(fname, "a");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    fprintf(fp, "%d,%s,%ld,%ld\n", pid, msg, st_sec, st_milisec);
    fclose(fp);
}

//funçao auxiliar so para ver o que esta no ficheiro
void printFile(char* fname){
    FILE * fp = fopen(fname, "r");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    char linha[300];
    while(fgets(linha, 300, fp) != NULL){
        write(1, linha, strlen(linha)+1);
    }
    fclose(fp);
}

int updateLine(int pid, char* temp_file, char* caminho, long int end_sec, long int end_milisec){
    FILE* ftemp = fopen(temp_file, "r");
    if (ftemp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }
    FILE* aux = fopen("aux.csv", "w");
    if (ftemp == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        return -1;
    };

    char linha[1024];
    int pid_atual, found = 0;
    char msg[100];
    long int st_sec, st_milisec;
    while (fgets(linha, sizeof(linha), ftemp)) {
        if(found == 0){
            sscanf(linha,"%d,%[^,],%ld,%ld", &pid_atual, msg, &st_sec, &st_milisec);
            if(pid != pid_atual){
                fputs(linha, aux);
            }else{
                found++;
            }
        }else fputs(linha, aux);
    }

    fclose(aux);
    fclose(ftemp);

    // Renomeia o arquivo temporário para o nome do arquivo original
    if (rename("aux.csv", temp_file) != 0) {
        printf("Erro ao renomear arquivo.\n");
        return -1 ;
    }

    FILE* f = fopen(fileWpath(pid, caminho), "a");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }

    int time = getExecutionTime(st_sec, st_milisec, end_sec, end_milisec);
    fprintf(f, "%d,%s,%d\n", pid, msg, time);

    fclose(f);

    return time;
}

char* fileWpath(int pid, char* caminho){
    char buffer[strlen(caminho)+11];
    snprintf(buffer, sizeof(buffer), "%s%06d.txt", caminho, pid);
    return strdup(buffer);
}

int openFile(char* fname){
    FILE *fp;
    fp = fopen(fname, "w");
    if (fp == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return -1;
    }
    fclose(fp);
    return 0;
}   

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

void activity(int pid, char* fname){
    int writefifo = open(fileName(pid), O_WRONLY);

    FILE *fp = fopen(fname, "r");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    char linha[300];
    struct timeval now;
    gettimeofday(&now,NULL);
    while (fgets(linha, 300, fp) != NULL){
        char buffer[300];
        int pid_atual;
        char msg[100];
        long int st_sec, st_milisec;
        sscanf(linha,"%d,%[^,],%ld,%ld", &pid_atual, msg, &st_sec, &st_milisec);
        
        int exectime = getExecutionTime(st_sec, st_milisec, now.tv_sec, now.tv_usec);
        snprintf(buffer, sizeof(buffer), "%03ld%d %s %d ms\n", digitCount(pid_atual) + 1 + strlen(msg) + 1 + digitCount(exectime) + 5,pid_atual, msg, exectime);
        write(writefifo, buffer, strlen(buffer) + 1);
    }
    fclose(fp);
    close(writefifo);
}

int get_pid(int fifo, int tamanho){
    char pid[tamanho + 1];
    int n = read(fifo, pid, tamanho);
    pid[n] = '\0';
    return atoi(pid);
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

