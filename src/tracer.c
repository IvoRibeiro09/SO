#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mensagem/mensagem.h"


int main(int argc, char* argv[]){
    if(argc < 2)
    {
        puts("ERROR few arguments!!!");
        return -1;
    }

    if(strcmp(argv[1],"execute") == 0 && strcmp(argv[2],"-u") == 0)
    {   
        int fifo = open("logs",O_WRONLY);
        puts("Novo cliente !");
        char buffer[502] = "";
        
        //passar todos os comandos para uma string
        //rever o caso de sejam passados mais argumentos que 502 bytes
        for (int i = 3; i < argc ; i++)
        {
            strcat(buffer ,argv[i]);
            if(i < argc-1)strcat(buffer," ");
        }

        //enviar para o servidor
        /*
        
        Mensagem m = malloc(sizeof(Mensagem));
        set_tamanho(m, strlen(buffer)+1);
        set_pid(m, getpid());

        char buffer2[512], buffer3[512];
        memcpy(buffer2, &m , sizeof(Mensagem));
        strcat(buffer2,buffer);
        printf("%s\n",buffer2);
        write(fifo, buffer2, sizeof(Mensagem) + strlen(buffer)+1);
        write(1, buffer2, sizeof(Mensagem) + strlen(buffer)+1);
        printf("\n");
        printf("%ld--%d\n", get_tamanho(m), get_pid(m));
        */

        char str_len[512];
        sprintf(str_len,"%04ld%06d%s",strlen(buffer)+1,getpid(),buffer);
        write(fifo,str_len,strlen(str_len)+1);
        puts(str_len);
        //depois ler uma estrutura

        char pid_str[7];
        sprintf(pid_str,"%06d",getpid());

        int f;

        if ((f = mkfifo(pid_str,0666)) < 0) puts("ERRO!!!! pipe ja existe!");
        puts("Pipe de leitura aberto!!");

        int readfifo = open(pid_str, O_RDONLY);
        int n;
        char tamanho[5], bufferleitura[512];

        while((n = read(readfifo, tamanho, 4)) > 0)
        {
            int Nread = atoi(tamanho);
            n = read(readfifo, bufferleitura, Nread);
            printf("Recebido\ntamnaho --> %d\nConteudo --> %s\n",Nread, bufferleitura);
        }
        close(readfifo);


        //criar um processo filho para cada cliente 
        //este pocesso filho ira ter acesso ao pid do cliente e à mensagem recebida do mesmo 
        //criar um pipe com o nome do pid do filho para que possa ser escrita a informaçao/resultado dos comandos passsados
        //que vai ser lido e apresentado no terminal do cliente 

        close(fifo);
    }
    
}

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