#include  <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>
#include "mensagem/mensagem.h"

int espaco(char* buffer){
    int espacos = 0;
    for(int i =0;i<strlen(buffer)-1;i++){
        if(buffer[i] == ' '){
            espacos++;
        }
    }
    return espacos;
}

void execute(char *pid, char buffer[])
{
    printf("Execute\nPid --> %s\nBuffer ---> %s\n", pid, buffer);

    int writefifo = open(pid, O_WRONLY);

    char str_len[512];
    sprintf(str_len,"%04ld%s",strlen(buffer)+1, buffer);
    write(writefifo, str_len, strlen(str_len)+1);
    close(writefifo);
    //mudar o display do exec do terminal para o pipe 
    //fazer o exec
    _exit(1);
}

int main(){

    int f;
    char *fiforeader = "logs";

    if ((f = mkfifo(fiforeader,0666)) < 0) puts("ficheiro ja existe!");
    puts("Servidor aberto!!");
    //abrir o pipe para leitura
    int fifo = open(fiforeader , O_RDWR);
    //
    //leitura de novos clientes
    char buffer[503], tamanho[5], pid_str[7];
    int n;

    while((n = read(fifo, tamanho, 4)) > 0){   
        //time_t segundos,seg;
        struct timeval start,end;
        struct tm *info1,*info2;
        
        //int error = read(fifo, tamanho, 4);
        //if(error == 0) break;
        puts("Nova Conexão!!"); //alguam coisa foi colocada no pipe
        gettimeofday(&start,NULL);
        read(fifo, pid_str, 6); // pid do cliente       
        int Nread = atoi(tamanho);
        n = read(fifo ,buffer, Nread); //mensagem
        printf("Numero de bytes: %d-%d\nPid --> %s\nBuffer ---> %s\nTime -->%ld.%06ld\n",Nread,n,pid_str, buffer,start.tv_sec,start.tv_usec);
        pid_t filho = fork();
        if(filho == 0){
            int esp = espaco(buffer);
            if(esp!=0){
                char* argv[esp+1];
                for(int i=0;i<esp;i++){
                    argv[i] = strtok(buffer," ");
                }
                argv[esp] = strtok(buffer,"\0");
                execv(argv[0],argv);//falta ver melhor este!!!!!
            }else{
                execl(buffer,buffer,"-l",NULL);
            }
            int status;
            waitpid(filho,&status,0);
            gettimeofday(&end,NULL);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0){
                printf("Comando executado com sucesso");
            }else{
                printf("Erro ao executar o comando!!!");
            }
            printf("Comando executado ás %ld.%06ld\n Demorou %ld \n microsegundos",start.tv_sec,start.tv_usec,((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)));
            execute(pid_str, buffer);

        }
        
    }
    close(fifo);
    return 0;
}


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
