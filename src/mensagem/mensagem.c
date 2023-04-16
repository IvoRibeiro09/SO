#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mensagem.h"

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