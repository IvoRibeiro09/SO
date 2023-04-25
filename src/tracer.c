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

void sendStatus(int fifo, int pid){
    char buffer[BUFFER_SIZE];
    
    snprintf(buffer, sizeof(buffer), "%d%03d%d", 2, digitCount(pid) + 1, pid);
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
    
        if(strcmp(argv[2], "-u") == 0){//executar um so programa   
            int pid = getpid();
            printf("Running PID %d\n", pid);
            gettimeofday(&start,NULL);
            //enviar para o servidor
            //unificar tudo para que seja efetuada apenas uma escrita
            sendMessage(fifo, pid, start.tv_sec, start.tv_usec, argv[3]);

            if ((f = mkfifo(fileName(pid),0666)) < 0) puts("ERRO!!!! pipe ja existe!");
            //puts("Pipe de leitura aberto!!");

            int readfifo = open(fileName(pid), O_RDONLY);
            reciveMessage(readfifo);
            close(readfifo);
            
            //end
            gettimeofday(&end,NULL);
            sendEndMessage(fifo, pid, end.tv_sec, end.tv_usec);
            printf("Ended in %ld ms\n", getExecutionTime(start.tv_sec, start.tv_usec, end.tv_sec, end.tv_usec));

        }else if(strcmp(argv[2], "-p") == 0){//executar mais de um programa      
            char *token = strtok(argv[3], "|");
            int count = 0;

            while(token != NULL){   
                count++;
                int filho = fork();
                if(filho == 0){   
                    struct timeval start, end;
                    int pid = getpid();
                    printf("Running PID %d\n", pid);
                    gettimeofday(&start,NULL);
                    sendMessage(fifo, pid, start.tv_sec, start.tv_usec, token);

                    if ((f = mkfifo(fileName(pid),0666)) < 0) puts("ERRO!!!! pipe ja existe!");
                    //puts("Pipe de leitura aberto!!");

                    int readfifo = open(fileName(pid), O_RDONLY);
                    reciveMessage(readfifo);
                    close(readfifo);
                    
                    gettimeofday(&end,NULL);
                    sendEndMessage(fifo, pid, end.tv_sec, end.tv_usec);
                    printf("Ended in %ld ms\n", getExecutionTime(start.tv_sec, start.tv_usec, end.tv_sec, end.tv_usec));
                    _exit(1);
                }

                token = strtok(NULL, "|");
            }
            //esperar a morte dos filhos
            for (int i = 0; i < count; i++){
                int status;
                wait(&status);
            }
        }
    }else if(strcmp(argv[1], "status") == 0){
        int pid = getpid();
        if ((f = mkfifo(fileName(pid),0666)) < 0) puts("ERRO!!!! pipe ja existe!");
        //puts("Pipe de leitura aberto!!");

        sendStatus(fifo, pid);

        int readfifo = open(fileName(pid), O_RDONLY);
        reciveMessage(readfifo);
        close(readfifo);

    }
    //else outros casos
    close(fifo);
    return 0;
}


/*

//passar todos os comandos para uma string
        //rever o caso de sejam passados mais argumentos que 502 bytes
        char args_str[MAX_ARGS_CARACTERS] = "";
        for (int i = 3; i < argc && strlen(args_str) < MAX_ARGS_CARACTERS; i++)
        {
            strcat(args_str,argv[i]);
            if(i < argc-1)strcat(args_str," ");
        }

        Mensagem m = malloc(sizeof(Mensagem));
        set_tamanho(m, strlen(buffer)+1);
        set_pid(m, getpid());

        char buffer2[512];
        memcpy(buffer2, &m + argv , sizeof(Mensagem)+ (argc*sizeof(char *)));
        
        write(fifo, buffer2, sizeof(Mensagem) + strlen(buffer)+1);
        write(1, buffer2, sizeof(Mensagem) + strlen(buffer)+1);
        printf("\n");
        printf("%ld--%d\n", get_tamanho(m), get_pid(m));
        */

/*

int main(int argc, char *argv[])
{
    int fifo = open("log",O_WRONLY), n = 1;
    puts("Novo cliente !");
    char buffer[1024];
    int i = 2;

    while(n <= argc) {
        char str_len[512];
        sprintf(str_len,"%4ld",strlen(argv[n])+1);
        strcat(str_len, argv[n]);
        write(fifo,str_len,strlen(str_len)+1);
        n++;
    }

    close(fifo);
    return 0;
}*/

/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd[2];
    int num = 42;
    int result;

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    // Escreve o valor do inteiro no pipe
    if (write(fd[1], &num, sizeof(int)) != sizeof(int)) {
        perror("write");
        exit(1);
    }

    // Lê o valor do inteiro do pipe
    if (read(fd[0], &result, sizeof(int)) != sizeof(int)) {
        perror("read");
        exit(1);
    }

    printf("Valor lido do pipe: %d\n", result);

    return 0;
}
*/