#ifndef MENSAGEM_
#define MENSAGEM_

typedef struct mensagem *Mensagem;

size_t get_tamanho(Mensagem m);

void set_tamanho(Mensagem m, size_t tamanho);

pid_t get_pid(Mensagem m);

void set_pid(Mensagem m, pid_t pid);

#endif