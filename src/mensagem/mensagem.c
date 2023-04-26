#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mensagem.h"

int digitCount(int n)
{
    int count = 0;
    while(n > 0)
    {
        count++;
        n = n/10; 
    }
    return count;
}

long int getExecutionTime(long int sec, long int milisec, long int sec_end, long int milisec_end){
    return (sec_end - sec) * 1000 + (milisec_end - milisec) / 1000; 
}

int messageSize(int fifo){
    char tamanho[4];
    int n = read(fifo, tamanho, 3);
    tamanho[n] = '\0';
    return atoi(tamanho);
}

char* fileName(int pid){
    char *pid_str = malloc(7);
    if (pid_str == NULL)return NULL;
    sprintf(pid_str, "%06d", pid);
    pid_str[6] = '\0';
    return pid_str;
}

void sendMessage(int fifo, int tipo, int pid, long int sec, long int milisec, char *msg){
    char writebuffer[BUFFER_SIZE];
    snprintf(writebuffer, sizeof(writebuffer), "%d%03ld%d,%ld.%ld,%s", tipo, strlen(msg) + 1 + digitCount(pid) + 1  + digitCount(sec) + 1 + digitCount(milisec) + 1, pid, sec, milisec, msg);
    write(fifo, writebuffer, strlen(writebuffer) + 1);
    //write(1, writebuffer, strlen(writebuffer) + 1);//so para visualizar o que foi enviado
    //printf("\nMensagem enviada\n");
}

void sendEndMessage(int fifo, int pid,long int sec,long int milisec){
    char writebuffer[BUFFER_SIZE];
    snprintf(writebuffer, sizeof(writebuffer), "%d%03d%d,%ld.%ld", 9, digitCount(pid) + 1 + digitCount(sec) + 1 + digitCount(milisec) + 1, pid, sec, milisec);
    write(fifo, writebuffer, strlen(writebuffer) + 1);
    //write(1, writebuffer, strlen(writebuffer) + 1);//so para visualizar o que foi enviado
    //printf("\nEnd Mensagem enviada\n");
}

void reciveMessage(int fifo){
    char bufferleitura[BUFFER_SIZE];
    int NbytesRead;
    while((NbytesRead = read(fifo, bufferleitura, BUFFER_SIZE)) > 0){
        bufferleitura[NbytesRead] = '\0';
        write(1, bufferleitura, strlen(bufferleitura) + 1);
        memset(bufferleitura, 0, sizeof(bufferleitura));
    }
}