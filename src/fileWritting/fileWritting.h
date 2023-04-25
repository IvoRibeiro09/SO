#ifndef FILEWRITTING_
#define FILEWRITTING_

void writeLine(int pid, char* msg, long int st_sec, long int st_milisec);

void updateLine(int pid, long int end_sec, long int end_milisec);

void printFile();

#endif