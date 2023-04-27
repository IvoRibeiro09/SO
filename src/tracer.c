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

    }
    //else outros casos
    close(fifo);
    return 0;
}
