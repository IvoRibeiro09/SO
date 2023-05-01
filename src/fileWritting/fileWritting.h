#ifndef FILEWRITTING_
#define FILEWRITTING_

void writeLine(int pid, char* fname, char* msg, long int st_sec, long int st_milisec);

int updateLine(int pid, char* temp_file, char* filename, long int end_sec, long int end_milisec);
 
void printFile();

int openFile(char* fname);

char* fileWpath(int pid, char* caminho);

char* cleanArguments(char* input);

void activity(int pid, char* fname);

int get_pid(int fifo, int tamanho);

int numberSpaces(char* string);

#endif