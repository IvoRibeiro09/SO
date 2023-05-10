#ifndef AUXFUNC_
#define AUXFUNC_
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../mensagem/mensagem.h"

#define PIPEGLOBAL "logs"
#define PIPEGLOBAL2 "logs2"

int numberSpaces(char* string);

char* juntapids(char* argv[], int n);

int numberVirgulas(char* string);

int numberBar(char* string);

int* arraypid(char* msg, int n);

int get_pid(int fifo, int tamanho);

#endif