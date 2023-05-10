#include "fileWritting.h"

int create_file(char* caminho, Pid_Stat stat, long int end_sec, long int end_milisec){
    
    int pid = get_stat_pid(stat);
    char *msg = get_stat_message(stat);
    long int st_sec = get_stat_sec(stat), st_milisec = get_stat_milisec(stat);

    int fd = open(fileWpath(pid, caminho), O_WRONLY | O_CREAT, 0666);
    if (fd < 0) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }

    int time = getExecutionTime(st_sec, st_milisec, end_sec, end_milisec);
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "%d,%s,%d\n", pid, msg, time);

    write(fd, buffer, strlen(buffer) + 1);

    close(fd);

    return time;
}

char* fileWpath(int pid, char* caminho){
    char buffer[strlen(caminho) + 11];
    snprintf(buffer, sizeof(buffer), "%s%06d.txt", caminho, pid);
    return strdup(buffer);
}