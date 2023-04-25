#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mensagem/mensagem.h"
#include "fileWritting/fileWritting.h"

bool isArgument(char *arg){
    if(arg[0] == '-') return false;
    else return true;
}
void activity(int pid){
    int writefifo = open(fileName(pid), O_WRONLY);

    FILE *fp = fopen("dados.csv", "a");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    char linha[300];
    int pid_atual;
    char msg[100];
    long int st_sec, st_milisec, end_sec, end_milisec;
    while (fgets(linha, 300, fp) != NULL){
        sscanf(linha,"%d,%[^,],%ld,%ld,%ld,%ld", &pid_atual, msg, &st_sec, &st_milisec, &end_sec, &end_milisec);
        if (end_milisec < 0 && end_sec < 0){
            write(writefifo, msg, strlen(msg) + 1);
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
        if(OP == 1){
            puts("Nova Conexão!!");
            tamanho = messageSize(fifo);
            NbytesRead = read(fifo, buffer, tamanho);
            buffer[NbytesRead] = '\0';

            pid_t filho = fork();
            if(filho == 0){ 
                sscanf(buffer, "%d,%ld.%ld,%[^;]",&pid, &start_sec, &start_milisec, message);

                writeLine(pid, message, start_sec, start_milisec);
              
                printf("PID %d A EXECUTAR!!!\n",pid);
                execute(pid, message);
            }
        }else if(OP == 2){
            puts("STATUS");
            tamanho = messageSize(fifo);
            int pid = get_pid(fifo, tamanho);
            activity(pid);

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
        printFile();
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