#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "mensagem/mensagem.h"
#include "fileWritting/fileWritting.h"

char* juntapids(char* argv[], int n) {
    char* buffer = malloc(BUFFER_SIZE);

    buffer[0] = '\0';

    for (int i = 0; i < n; i++) {
        strcat(buffer, argv[i+2]);
        strcat(buffer, ",");
    }
    buffer[strlen(buffer)-1] = '\0';
    return buffer;
}

int main(int argc, char* argv[]){

    if(argc < 2){
        puts("ERROR few arguments!!!");
        return -1;
    }

    int fifo = open(PIPEGLOBAL, O_RDWR), f;
    int fifo2 = open(PIPEGLOBAL2, O_RDWR);

    if(strcmp(argv[1], "execute") == 0){
        struct timeval start, end;
        int pid = getpid();
        gettimeofday(&start, NULL);

        if ((f = mkfifo(fileName(pid),0666)) < 0) puts("ERRO!!!! pipe ja existe!");
        
        int tipo;
        if(strcmp(argv[2], "-u") == 0) tipo = 1;//executar um so programa   
        else if(strcmp(argv[2], "-p") == 0) tipo = 2;//executar varios 
            
        printf("Running PID %d\n", pid);
        //enviar para o servidor
        //unificar tudo para que seja efetuada apenas uma escrita
        sendMessage(fifo, tipo, pid, start.tv_sec, start.tv_usec, argv[3]);

        //ler do servidor 
        int readfifo = open(fileName(pid), O_RDONLY);
        reciveMessage(readfifo);
        close(readfifo);
            
        //end
        gettimeofday(&end, NULL);
        sendEndMessage(fifo, pid, end.tv_sec, end.tv_usec);
        printf("Ended in %d ms\n", getExecutionTime(start.tv_sec, start.tv_usec, end.tv_sec, end.tv_usec));
       
    }else if(strcmp(argv[1], "status") == 0){
        int pid = getpid();
        if ((f = mkfifo(fileName(pid),0666)) < 0) puts("ERRO!!!! pipe ja existe!");
        //puts("Pipe de leitura aberto!!");

        sendStatus(fifo, pid);

        int readfifo = open(fileName(pid), O_RDONLY);
        reciveHeadMessage(readfifo);
        close(readfifo);

    }else if(strcmp(argv[1], "stats-time") == 0 || strcmp(argv[1], "stats-command") == 0 || strcmp(argv[1], "stats-uniq") == 0){
        int pid = getpid();
        if ((f = mkfifo(fileName(pid),0666)) < 0) puts("ERRO!!!! pipe ja existe!");
        
        int tipo, Npids = argc - 2;
        if(strcmp(argv[1], "stats-command") == 0) tipo = 5;
        else if(strcmp(argv[1], "stats-time") == 0) tipo = 4;
        else if(strcmp(argv[1], "stats-uniq") == 0) tipo = 6;
 
        sendStats(fifo2, tipo, pid, juntapids(argv, Npids));
        int readfifo = open(fileName(pid), O_RDONLY);
        reciveHeadMessage(readfifo);
        close(readfifo);
    }
    //else outros casos
    close(fifo);
    close(fifo2);
    return 0;
}
