#include  <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include<stdio.h>
#include <stdlib.h>

int main()
{
    int f, loop = 0;
    char *fiforeader = "logs";

    if ((f = mkfifo(fiforeader,0666)) < 0)
    {
        puts("ficheiro ja existe!");
    }
    puts("Servidor aberto!!");

    //leitura de novos clientes
    while(loop == 0)
    {
        char buffer[1024], tamanho[4];
        int fifo = open(fiforeader ,O_RDONLY), n;
        puts("Nova Conexão!!");

        read(fifo, tamanho, 4); //tamanho da mensagem 
        int Nread = atoi(tamanho);
        char pid[6];
        read(fifo, pid, 6); // pid do cliente
        n = read(fifo ,buffer, Nread); //mensagem
        write(1, pid, 6);
        printf("\n");
        write(1, buffer, n);
        printf("\n");

        close(fifo);
    }

    /*
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