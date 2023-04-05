#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <string.h>
#include <stdio.h>


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("ERROR few arguments!!!");
        return -1;
    }

    if(strcmp(argv[1],"execute") == 0 && strcmp(argv[2],"-u") == 0)
    {   
        int fifo = open("logs",O_WRONLY);
        puts("Novo cliente !");
        char buffer[1024] = "";
        
        //passar todos os comandos para uma string
        for (int i = 3; i < argc ; i++)
        {
            strcat(buffer,argv[i]);
            if(i < argc-1)strcat(buffer," ");
        }

        //enviar para o servidor
        char str_len[512];
        sprintf(str_len,"%4ld%6d",strlen(buffer)+1,getpid());
        strcat(str_len, buffer);
        write(fifo,str_len,strlen(str_len)+1);
        write(1,str_len,strlen(str_len)+1);
        printf("\n");
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