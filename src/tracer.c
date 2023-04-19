#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mensagem/mensagem.h"

#define BUFFER_SIZE 512
#define MAX_ARGS_CARACTERS 490

int digitCount(int n)
{
    int count = 0;
    while(n > 0)
    {
        count++;
        n = n/10;
    }
    return count;
}


int main(int argc, char* argv[])
{

    if(argc < 2)
    {
        puts("ERROR few arguments!!!");
        return -1;
    }
    printf("Running PID %d\n", getpid());

    
    if(strcmp(argv[1],"execute") == 0 && strcmp(argv[2],"-u") == 0)//executar um so programa
    {   
        int fifo = open("logs",O_WRONLY);
        
        //passar todos os comandos para uma string
        //rever o caso de sejam passados mais argumentos que 502 bytes
        char args_str[MAX_ARGS_CARACTERS] = "";
        for (int i = 3; i < argc && strlen(args_str) < MAX_ARGS_CARACTERS; i++)
        {
            strcat(args_str,argv[i]);
            if(i < argc-1)strcat(args_str," ");
        }

        //enviar para o servidor
        //unificar tudo para que seja efetuada apenas uma escrita
        char writebuffer[BUFFER_SIZE];
        snprintf(writebuffer, sizeof(writebuffer), "%03ld%d,%d,%s", strlen(args_str) + 1 + digitCount(getpid()) + 1 + digitCount(argc) + 1 , getpid(), argc, args_str);
        write(fifo, writebuffer, strlen(writebuffer) + 1);
        write(1, writebuffer, strlen(writebuffer) + 1);//so para visualizar o que foi enviado


        int f;
        char pid_str[7]; 
        sprintf(pid_str,"%06d",getpid()); //criar o pipe de leitura que vai ser escrito pelo servidor

        if ((f = mkfifo(pid_str,0666)) < 0) puts("ERRO!!!! pipe ja existe!");
        //puts("Pipe de leitura aberto!!");

        int readfifo = open(pid_str, O_RDONLY);
        int n;
        char bufferleitura[BUFFER_SIZE];

        while((n = read(readfifo, bufferleitura, BUFFER_SIZE)) > 0)
        {
            bufferleitura[n] = '\0';
            printf("%s", bufferleitura);
            memset(bufferleitura, 0, sizeof(bufferleitura));
        }
        close(readfifo);

        close(fifo);
    }
    //else outros casos
    int milisec = 90;
    printf("Ended in %d ms\n", milisec);
    return 0;
}


/*
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