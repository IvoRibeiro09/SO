#ifndef MENSAGEM_
#define MENSAGEM_

#define BUFFER_SIZE 512
#define MAX_ARGS_CARACTERS 490

int digitCount(int n);

int getExecutionTime(long int sec, long int milisec, long int sec_end, long int milisec_end);

int messageSize(int fifo);

char* fileName(int pid);
//char* messageWPID(int pid, char* msg);
/*
typedef struct mensagem *Mensagem;

size_t get_tamanho(Mensagem m);

void set_tamanho(Mensagem m, size_t tamanho);

pid_t get_pid(Mensagem m);

void set_pid(Mensagem m, pid_t pid);
*/
#endif