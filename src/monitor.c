#include  <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mensagem/mensagem.h"

void execute(int pid, int args, char buffer[])
{
    //printf("Execute\nPid --> %d\nBuffer ---> %s\n", pid, buffer);
    char pid_str[7]; 
    sprintf(pid_str,"%06d", pid);

    int writefifo = open(pid_str, O_WRONLY);

    char str_len[512];
    sprintf(str_len,"%04ld%s",strlen(buffer)+1, buffer);

    char *token = strtok(buffer, " ");

    char *argv[args];
    int i = 0;

    while (token != NULL && i < 6) {
        argv[i++] = token;
        token = strtok(NULL, " ");
    }
    argv[i++] = NULL;

    dup2(writefifo, 1);//mudar de print do ecra para print no fifo

    execvp(argv[0], argv);//executar os comandos

    close(writefifo);
    
    _exit(1);
}

int main(){
    int f;
    char *fiforeader = "logs";

    if ((f = mkfifo(fiforeader,0666)) < 0) puts("ficheiro ja existe!");
    puts("Servidor aberto!!");

    //abrir o pipe para leitura
    int fifo = open(fiforeader , O_RDWR);
    //leitura de novos clientes

    char buffer[512], buffer2[503], tamanho[4];
    int n;

    while((n = read(fifo, tamanho, 3)) > 0)
    {   
        puts("Nova Conexão!!"); //alguam coisa foi colocada no pipe
        
        tamanho[4] = '\0';
        int Nread = atoi(tamanho), Nargs, pid;
        n = read(fifo, buffer, Nread);
        buffer[n] = '\0';

        sscanf(buffer, "%d,%d,%[^;]",&pid, &Nargs, buffer2);
        
        pid_t filho = fork();
        if(filho == 0)
        {   
            printf("PID %d A EXECUTAR!!!\n",pid);
            execute(pid, Nargs, buffer2);
        }
        
    }
    close(fifo);

    return 0;
}
/*
        read(fifo, pid_str, 5); // pid do cliente       
        read(fifo, args, 2); //numero de argumentos
        */
        /*
        Nargs = atoi(args);
        n = read(fifo ,buffer, Nread); //mensagem
        printf("Numero de bytes: %d-%d\nPid --> %s\nBuffer ---> %s\n",Nread,n,pid_str, buffer);
        */

/*
    char bb[sizeof(Mensagem)], buffer2[512];
    puts("espera que seja escrito no pipe");
    Mensagem m = malloc(sizeof(Mensagem));
    n = read(fifo, bb, sizeof(bb));
    printf("%d\n%s\n",n,bb);
    printf("algo no fifo\n");
        
    memcpy(&m, bb, sizeof(Mensagem));
    char* conteudo ="";// buffer2 + sizeof(Mensagem);
        printf("Mensagem recebida: tamanho = %ld, pid = %d, texto = %s\n",get_tamanho(m),get_pid(m),conteudo);
        */



/*
        pid_t filho = fork();
        if(filho == 0)
        {
            execute(pid, buffer);
            _exit(1);
        }
        */


/*
dois fifos 
um de leitura do server e escriat do cliente
e outro de escrita pelo server e leitura pelo cliente   
o de escrita pelo cliente deve ter de ser fiferente para cada cliente um por cliente*/

//guardard tudo num ficheiro de logs para cumprir as avançadas 
//pode ser csv e arganilar por colunas nome e tempo
//