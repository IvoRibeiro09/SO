#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mensagem/mensagem.h"
#include "fileWritting/fileWritting.h"

char* cleanArguments(char* string){
  char buffer[20];
  if(string[0] == ' '){
    for(int i =0;i<strlen(string);i++){
      buffer[i]=string[i+1];
    }
  }
  if(buffer[strlen(buffer)-1] == ' '){
    buffer[strlen(buffer)-1] = buffer[strlen(buffer)];
  }
  return strdup(buffer);
}

void activity(int pid){
    int writefifo = open(fileName(pid), O_WRONLY);

    FILE *fp = fopen("dados.csv", "r");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    char linha[300];
    struct timeval now;
    gettimeofday(&now,NULL);
    while (fgets(linha, 300, fp) != NULL){
        char buffer[300];
        int pid_atual;
        char msg[100];
        long int st_sec, st_milisec, end_sec, end_milisec;
        sscanf(linha,"%d,%[^,],%ld,%ld,%ld,%ld", &pid_atual, msg, &st_sec, &st_milisec, &end_sec, &end_milisec);
        if (end_sec < 0){
            int exectime = getExecutionTime(st_sec, st_milisec, now.tv_sec, now.tv_usec);
            snprintf(buffer, sizeof(buffer), "%03ld%d %s %d ms\n", digitCount(pid_atual) + 1 + strlen(msg) + 1 + digitCount(exectime) + 5,pid_atual, msg, exectime);
            write(writefifo, buffer, strlen(buffer) + 1);
        }
    }
    fclose(fp);
    close(writefifo);
}

int get_pid(int fifo, int tamanho){
    char pid[tamanho];
    int n = read(fifo, pid, tamanho);
    pid[n] = '\0';
    return atoi(pid);
}

//refazer pq so executa um programa por vez
void execute(int fifo, char buffer[]){
    
    //trocar o 10 para uma funçao que conta o numero de espaços
    char *argv[10];
    int i = 0;
    char *token = strtok(buffer, " ");
    while(token != NULL){
        
        argv[i++] = token;
        token = strtok(NULL, " ");
        
    }
    argv[i++] = NULL;

    pid_t filho = fork();
    if(filho == 0){
        dup2(fifo, 1);//mudar de print do ecra para print no fifo
        execvp(argv[0], argv);//executar os comandos
    }
}

int main(){
    int f;
    if ((f = mkfifo(PIPEGLOBAL, 0666)) < 0) puts("ficheiro ja existe!");
    puts("Servidor aberto!!");

    //abrir o pipe para leitura
    int fifo = open(PIPEGLOBAL, O_RDWR);

    //variaveis
    char option[2], buffer[BUFFER_SIZE], message[MAX_ARGS_CARACTERS];
    int NbytesRead, OP, tamanho, pid;
    long int start_sec, start_milisec, end_sec, end_milisec;

    while((NbytesRead = read(fifo, option, 1)) > 0){   
        option[NbytesRead] = '\0';
        OP = atoi(option);
        if(OP == 1 || OP == 2){
            puts("Nova Conexão!!");
            tamanho = messageSize(fifo);
            NbytesRead = read(fifo, buffer, tamanho);
            buffer[NbytesRead] = '\0';

            pid_t filho = fork();
            if(filho == 0){

                sscanf(buffer, "%d,%ld.%ld,%[^;]",&pid, &start_sec, &start_milisec, message);
                writeLine(pid, message, start_sec, start_milisec);

                //abrir o pipe de escrita
                int writefifo = open(fileName(pid), O_WRONLY);

                if(OP == 1){//execuçao unica       
                    
                    printf("PID %d A EXECUTAR!!!\n",pid);
                    execute(pid, message);
                    
                }else{//pipeline

                    char *token = strtok(message, "|");
                    int count = 0;
                    while(token != NULL){
                        count++;
                        int filho = fork();
                        if(filho == 0){
                            printf("PID %d A EXECUTAR!!!\n",pid);
                            execute(writefifo, token);
                           
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
                close(writefifo);
                _exit(1);
            }
        }else if(OP == 3){
            puts("STATUS");
            tamanho = messageSize(fifo);
            int pid = get_pid(fifo, tamanho);
            activity(pid);
            printFile();//ver o q esta escrito no file
        }else if(OP == 9){
            puts("FIM de Conexão!!"); //alguam coisa foi colocada no pipe
            tamanho = messageSize(fifo);
            NbytesRead = read(fifo, buffer, tamanho);
            buffer[NbytesRead] = '\0';

            pid_t filho = fork();
            if(filho == 0){
                sscanf(buffer, "%d,%ld.%ld",&pid, &end_sec, &end_milisec);

                updateLine(pid, end_sec, end_milisec);

                printf("PID %d TERMINOU!!\nEnded in %ld ms\n", pid, getExecutionTime(start_sec, start_milisec, end_sec, end_milisec));
            }
        }
        //printFile();
    }
    close(fifo);

    return 0;
}
/*#include <stdio.h>
#include <stdlib.h>

int main() {
    char nome[50];
    int idade;
    float altura;

    // obter dados do usuário
    printf("Nome: ");
    scanf("%s", nome);
    printf("Idade: ");
    scanf("%d", &idade);
    printf("Altura: ");
    scanf("%f", &altura);

    // abrir arquivo
    FILE *fp;
    fp = fopen("dados.csv", "w");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    // escrever dados no arquivo
    fprintf(fp, "%s,%d,%f\n", nome, idade, altura);

    // fechar arquivo
    fclose(fp);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char nome[50];
    int idade;
    float altura;

    // obter dados do usuário
    printf("Nome: ");
    scanf("%s", nome);
    printf("Idade: ");
    scanf("%d", &idade);
    printf("Altura: ");
    scanf("%f", &altura);

    // abrir arquivo
    FILE *fp;
    fp = fopen("dados.csv", "r+");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    // encontrar e sobrescrever linha desejada
    char linha[100];
    int linha_desejada = 2;
    int linha_atual = 1;
    while (fgets(linha, 100, fp) != NULL) {
        if (linha_atual == linha_desejada) {
            // sobrescrever linha desejada
            fseek(fp, -(strlen(linha)), SEEK_CUR);
            fprintf(fp, "%s,%d,%f\n", nome, idade, altura);
            break;
        }
        linha_atual++;
    }

    // fechar arquivo
    fclose(fp);

    return 0;
}

*/

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