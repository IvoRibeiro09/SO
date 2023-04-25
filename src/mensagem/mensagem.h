#ifndef MENSAGEM_
#define MENSAGEM_

#define BUFFER_SIZE 512
#define MAX_ARGS_CARACTERS 490
#define PIPEGLOBAL "logs"

int digitCount(int n);

long int getExecutionTime(long int sec, long int milisec, long int sec_end, long int milisec_end);

int messageSize(int fifo);

char* fileName(int pid);

void sendMessage(int fifo, int pid, long int sec, long int milisec, char *msg);

void sendEndMessage(int fifo, int pid,long int sec,long int milisec);

void reciveMessage(int fifo);

#endif