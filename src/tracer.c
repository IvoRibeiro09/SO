#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "mensagem/mensagem.h"
#include "auxFunc/auxFunc.h"

void execute(char buffer[]){
    
    char *argv[numberSpaces(buffer) + 1];
    int i = 0;
    char *token = strtok(buffer, " ");
    while(token != NULL){
        
        argv[i++] = token;
        token = strtok(NULL, " ");
        
    }
    argv[i++] = NULL;

    int filho = fork();
    if(filho == 0){
        execvp(argv[0], argv);//executar os comandos
    }
    int status;
    wait(&status);

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
        printf("Running PID %d\n", pid);
        if(strcmp(argv[2], "-u") == 0){
            tipo = 1;//executar um so programa   
            
            //enviar para o servidor
            //unificar tudo para que seja efetuada apenas uma escrita
            sendMessage(fifo, tipo, pid, start.tv_sec, start.tv_usec, argv[3]);

            //ler do servidor 
            execute(argv[3]);

        }else if(strcmp(argv[2], "-p") == 0){
            tipo = 2;//executar varios 
            
            sendMessage(fifo, tipo, pid, start.tv_sec, start.tv_usec, argv[3]);
            
            int barNum = numberBar(argv[3]);
            int pd[barNum + 1][2];
            for (int i = 0; i < barNum + 1; i++) {
                if (pipe(pd[i]) < 0) {
                    perror("pipe error");
                    exit(1);
                }
            }

            char *token = strtok(argv[3], "|");
            int count = 0, status;

            while (token != NULL) {
                int filho = fork();
                if (filho == 0) {
                    if(count == 0){
                        dup2(pd[0][1], 1);

                    } else if (count == barNum) {
                        dup2(pd[count - 1][0], 0);
                    } else {
                        dup2(pd[count - 1][0], 0);
                        dup2(pd[count][1], 1);
                    }

                    for (int i = 0; i < barNum + 1; i++) {
                        close(pd[i][0]);
                        close(pd[i][1]);
                    }

                    execute(token);
                    _exit(1);
                }

                token = strtok(NULL, "|");
                count++;
            }

            for (int i = 0; i < barNum + 1; i++) {
                close(pd[i][0]);
                close(pd[i][1]);
            }

            for (int i = 0; i < barNum + 1; i++) {
                wait(&status);
            }
   
        }
        //end
        gettimeofday(&end, NULL);
        sendEndMessage(fifo, pid, end.tv_sec, end.tv_usec);
        printf("Ended in %d ms\n", getExecutionTime(start.tv_sec, start.tv_usec, end.tv_sec, end.tv_usec));
       
    }else if(strcmp(argv[1], "status") == 0){
        int pid = getpid();
        if ((f = mkfifo(fileName(pid),0666)) < 0) puts("ERRO!!!! pipe ja existe!");
    
        sendStatus(fifo, pid);

        int readfifo = open(fileName(pid), O_RDONLY);
        
        reciveMessage(readfifo);
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
    close(fifo);
    close(fifo2);
    return 0;
}
