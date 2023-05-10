#ifndef FILEWRITTING_
#define FILEWRITTING_
#include <fcntl.h>
#include "../mensagem/mensagem.h"
#include "../hTable/hTable.h"

int create_file(char* caminho, Pid_Stat stat, long int end_sec, long int end_milisec);

char* fileWpath(int pid, char* caminho);

#endif