#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "fileWritting.h"

void writeLine(int pid, char* msg, long int st_sec, long int st_milisec){
    FILE *fp = fopen("dados.csv", "a");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    fprintf(fp, "%d,%s,%ld,%ld,%d,%d\n", pid, msg, st_sec, st_milisec, -1, -1);
    fclose(fp);
}

void updateLine(int pid, long int end_sec, long int end_milisec){
    FILE *fp = fopen("dados.csv", "r+");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    char linha[300];
    int pid_atual;
    char msg[100];
    long int st_sec, st_milisec;
    while (fgets(linha, 300, fp) != NULL){
        sscanf(linha,"%d,%[^,],%ld,%ld", &pid_atual, msg, &st_sec, &st_milisec);
        if (pid_atual == pid){
            long int posicao = ftell(fp);
            fseek(fp, posicao - strlen(linha), SEEK_SET);
            fprintf(fp, "%d,%s,%ld,%ld,%ld,%ld\n", pid, msg, st_sec, st_milisec, end_sec, end_milisec);
            break;
        }
    }

    fclose(fp);
}
//funçao auxiliar so para ver o que esta no ficheiro
void printFile(){
    FILE * fp = fopen("dados.csv", "r");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    char linha[300];
    puts("dados");
    while(fgets(linha, 300, fp) != NULL){
        write(1, linha, strlen(linha)+1);
    }
    fclose(fp);
}
