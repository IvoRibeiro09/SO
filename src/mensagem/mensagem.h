#ifndef MENSAGEM_
#define MENSAGEM_
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 512
#define MAX_ARGS_CARACTERS 490

int digitCount(int n);

int getExecutionTime(long int sec, long int milisec, long int sec_end, long int milisec_end);

int messageSize(int fifo);

char* fileName(int pid);

void sendMessage(int fifo, int tipo, int pid, long int sec, long int milisec, char *msg);

void sendEndMessage(int fifo, int pid,long int sec,long int milisec);

void reciveMessage(int fifo);

void sendStatus(int fifo, int pid);

void sendStats(int fifo, int tipo, int pid_tracer, char* msg);

void reciveHeadMessage(int fifo);

#endif