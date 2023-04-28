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

void reciveStats(int fifo){

    int NbytesRead;
    char buffer[50];
    while((NbytesRead = read(fifo, buffer, 50)) > 0){
        buffer[NbytesRead] = '\0';
        write(1, buffer, strlen(buffer) + 1);
    }
}



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


void sendStats(int fifo, int pid,int tag,char* msg){
    char buffer[BUFFER_SIZE];
    
    snprintf(buffer, sizeof(buffer), "%d%03ld%d,%s", tag, strlen(msg) + 1 + digitCount(pid) + 1 , pid,msg);
    write(fifo, buffer, strlen(buffer) + 1);
    
}

int main(int argc, char* argv[]){

    if(argc < 2){
        puts("ERROR few arguments!!!");
        return -1;
    }

    int fifo = open(PIPEGLOBAL, O_RDWR), f;

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
        reciveStatus(readfifo);
        close(readfifo);

    }else if(strcmp(argv[1], "stats-time") == 0){
        int pid = getpid();
        if ((f = mkfifo(fileName(pid),0666)) < 0) puts("ERRO!!!! pipe ja existe!");
        int pids = argc - 2;
        char* pidstr = juntapids(argv,pids);
        //puts("Pipe de leitura aberto!!");
        sendStats(fifo, pid,4,pidstr);

        int readfifo = open(fileName(pid), O_RDONLY);
        reciveStats(readfifo);
        close(readfifo);
    }else if(strcmp(argv[1], "stats-command") == 0){
        int pid = getpid();
        if ((f = mkfifo(fileName(pid),0666)) < 0) puts("ERRO!!!! pipe ja existe!");
        int pids = argc - 2;
        char* pidstr = juntapids(argv,pids);
        //puts("Pipe de leitura aberto!!");
        sendStats(fifo, pid,5,pidstr);

        int readfifo = open(fileName(pid), O_RDONLY);
        reciveStats(readfifo);
        close(readfifo);
    }else if(strcmp(argv[1], "stats-uniq") == 0){
        int pid = getpid();
        if ((f = mkfifo(fileName(pid),0666)) < 0) puts("ERRO!!!! pipe ja existe!");
        int pids = argc - 2;
        char* pidstr = juntapids(argv,pids);
        //puts("Pipe de leitura aberto!!");
        sendStats(fifo, pid,6,pidstr);

        int readfifo = open(fileName(pid), O_RDONLY);
        reciveStatus(readfifo);
        close(readfifo);
        }
    //else outros casos
    close(fifo);
    return 0;
}
