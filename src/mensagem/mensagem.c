#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mensagem.h"

/*
struct mensagem {
    size_t tamanho;
    pid_t pid;
};

size_t get_tamanho(Mensagem m){
    return m->tamanho;
}

void set_tamanho(Mensagem m, size_t tamanho){
    m->tamanho = tamanho;
}

pid_t get_pid(Mensagem m){
    return m->pid;
}

void set_pid(Mensagem m, pid_t pid){
    m->pid = pid;
}
*/
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

int getExecutionTime(long int sec, long int milisec, long int sec_end, long int milisec_end){
    return (sec_end - sec) * 1000 + (milisec_end - milisec); 
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