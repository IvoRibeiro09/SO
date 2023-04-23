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

bool isArgument(char *arg){
    if(arg[0] == '-') return false;
    else return true;
}

void execute(int pid, char buffer[]){
    int writefifo = open(fileName(pid), O_WRONLY);

    char *argv[10];
    int i = 0;
    char *token = strtok(buffer, " ");
    while(token != NULL){
        
        argv[i++] = token;
        token = strtok(NULL, " ");
        while(token != NULL && !isArgument(token)){
            argv[i++] = token;
            token = strtok(NULL, " ");
        }
        argv[i++] = NULL;

        int filho = fork();

        if(filho == 0){
            dup2(writefifo, 1);//mudar de print do ecra para print no fifo
            execvp(argv[0], argv);//executar os comandos
        }
        i = 0;
        for (int j = 0; j < 10; j++) argv[j] = NULL;
    }
    
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

    //variaveis
    char option[2], buffer[BUFFER_SIZE], message[MAX_ARGS_CARACTERS];
    int NbytesRead, OP, tamanho, pid;
    long int start_sec, start_milisec, end_sec, end_milisec;

    while((NbytesRead = read(fifo, option, 1)) > 0)
    {   
        option[NbytesRead] = '\0';
        OP = atoi(option);
        if(OP == 1)
        {
            puts("Nova Conexão!!"); //alguam coisa foi colocada no pipe
            //escrever no ficheiro csv "pid,prog,timestart,timeend(-1)"   

            tamanho = messageSize(fifo);
            NbytesRead = read(fifo, buffer, tamanho);
            buffer[NbytesRead] = '\0';
            sscanf(buffer, "%d,%ld.%ld,%[^;]",&pid, &start_sec, &start_milisec, message);

            pid_t filho = fork();
            if(filho == 0)
            {   
                printf("PID %d A EXECUTAR!!!\n",pid);
                execute(pid, message);
            }
        }
        else if(OP == 0)
        {
            puts("FIM de Conexão!!"); //alguam coisa foi colocada no pipe
            tamanho = messageSize(fifo);
            NbytesRead = read(fifo, buffer, tamanho);
            buffer[NbytesRead] = '\0';
            sscanf(buffer, "%d,%ld.%ld",&pid, &end_sec, &end_milisec);
            //escrever no ficheiro 
            printf("PID %d TERMINOU!!\nEnded in %d ms\n", pid, getExecutionTime(start_sec, start_milisec, end_sec, end_milisec));
        }
        
    }
    close(fifo);

    return 0;
}
/*
    read(fifo, pid_str, 5); // pid do cliente       
    read(fifo, args, 2); //numero de argumentos
        
    Nargs = atoi(args);
    n = read(fifo ,buffer, Nread); //mensagem
    printf("Numero de bytes: %d-%d\nPid --> %s\nBuffer ---> %s\n",Nread,n,pid_str, buffer);
    
    char bb[sizeof(Mensagem)], buffer2[512];
    puts("espera que seja escrito no pipe");
    Mensagem m = malloc(sizeof(Mensagem));
    n = read(fifo, bb, sizeof(bb));
    printf("%d\n%s\n",n,bb);
    printf("algo no fifo\n");
        
    memcpy(&m, bb, sizeof(Mensagem));
    char* conteudo ="";// buffer2 + sizeof(Mensagem);
    printf("Mensagem recebida: tamanho = %ld, pid = %d, texto = %s\n",get_tamanho(m),get_pid(m),conteudo);
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