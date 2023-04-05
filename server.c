#include  <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void execute(char* pid, char buffer[])
{
    _exit(0);
}

int main()
{
    int f;
    char *fiforeader = "logs";

    if ((f = mkfifo(fiforeader,0666)) < 0)
    {
        puts("ficheiro ja existe!");
    }
    puts("Servidor aberto!!");

    //abrir o pipe para leitura
    int fifo = open(fiforeader ,O_RDONLY);
    //leitura de novos clientes

    char buffer[1024], tamanho[4], pid[6];
    int n = 1;

    while((n = read(fifo, tamanho, sizeof(tamanho))) > 0)
    {   
       
        printf("--%d\n",n);
        puts("Nova Conexão!!"); //alguam coisa foi colocada no pipe

        int Nread = atoi(tamanho);
        printf("--%d\n",Nread);
        n = read(fifo, pid, sizeof(pid)); // pid do cliente
        printf("--%d\n",n);
        n = read(fifo ,buffer, Nread); //mensagem
        printf("%s\n", pid);
        printf("%s\n", buffer);
        pid_t filho = fork();
        if(filho == 0)
        {
            execute(pid, buffer);
        }
    }
    close(fifo);

    /*
    read(fifo, name, sizeof(name));
            int Nread = atoi(name);
            n = read(fifo ,buffer, Nread);
            write(1,buffer,n);


    int fifo = open(fiforeader ,O_RDONLY), n;
    puts("Servidor aberto!!");
    char buffer[1024];
    char name[4];
    char tamanho[4];


    while(n > 0)
    {
        read(fifo, name, sizeof(name));
        int Nread = atoi(name);
        n = read(fifo ,buffer, Nread);
        write(1,buffer,n);
    }

    close(fifo);
    */
    return 0;
}

/*
dois fifos 
um de leitura do server e escriat do cliente
e outro de escrita pelo server e leitura pelo cliente   
o de escrita pelo cliente deve ter de ser fiferente para cada cliente um por cliente*/