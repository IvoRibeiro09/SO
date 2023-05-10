#include "mensagem.h"

int digitCount(int n){
    int count = 0;
    while(n > 0){
        count++;
        n = n/10; 
    }
    return count;
}

int getExecutionTime(long int sec, long int milisec, long int sec_end, long int milisec_end){
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

void sendStatus(int fifo, int pid){
    char buffer[BUFFER_SIZE];
    
    snprintf(buffer, sizeof(buffer), "%d%03d%d", 3, digitCount(pid) + 1, pid);
    write(fifo, buffer, strlen(buffer) + 1);
    
}

void sendStats(int fifo, int tipo, int pid_tracer, char* msg){
    char buffer[BUFFER_SIZE];
    
    snprintf(buffer, sizeof(buffer), "%d%03ld%d,%s", tipo, digitCount(pid_tracer) + strlen(msg) + 4, pid_tracer, msg);
    write(fifo, buffer, strlen(buffer) + 1);
    //write(1, buffer, strlen(buffer) + 1);
}

void reciveHeadMessage(int fifo){
    int NbytesRead;
    char tamanho[4], buffer[BUFFER_SIZE];
    while((NbytesRead = read(fifo, tamanho, 3)) > 0){
        tamanho[NbytesRead] = '\0';
        int tam = atoi(tamanho);
        NbytesRead = read(fifo, buffer, tam);
        buffer[NbytesRead] = '\0';
        write(1, buffer, strlen(buffer) + 1);
    }
}