#ifndef AUXFUNC_
#define AUXFUNC_

#define PIPEGLOBAL "logs"
#define PIPEGLOBAL2 "logs2"

char* cleanArguments(char* input);

int numberSpaces(char* string);

char* juntapids(char* argv[], int n);

int numberVirgulas(char* string);

int* arraypid(char* msg, int n);

int get_pid(int fifo, int tamanho);

#endif