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

int numberVirgulas(char* string){
  int c=0;
  for(int i=0;i<strlen(string);i++){
    if(string[i] == ','){
      c++;
    }
  }
  return c;
}

int* arraypid(char* msg, int n) {
  int* array = malloc(n+1 * sizeof(int));
  int i = 0;
  char* aux = strtok(msg, ",");
  array[i] = atoi(aux);
  while (aux != NULL) {
    aux = strtok(NULL, ",");
    if (aux != NULL) {
      i++;
      array[i] = atoi(aux);
    }
  }
  return array; 
}

void timestats(int pid_tracer, char* caminho, int* pids){
    int writefifo = open(fileName(pid_tracer), O_WRONLY);

    int i = 0;
    while(pids[i]){
        FILE* fp = fopen(fileWpath(pids[i], caminho), "r");
        char linha[300], msg[200], buffer[200];
        int pid, time;
        fgets(linha, 300, fp);
        sscanf(linha, "%d,%[^,],%d", &pid, msg, &time);
        snprintf(buffer, sizeof(buffer), "%03d%d ms\n", digitCount(time) + 4, time);
        write(writefifo, buffer, strlen(buffer) + 1);
        i++;
        fclose(fp);
    }
    close(writefifo);
}

void procuracomando(int pid_tracer, char* caminho, char* cmd, int* pids){
    int writefifo = open(fileName(pid_tracer), O_WRONLY);
    int count = 0, i = 0, pid, time;
    char linha[300], msg[200], buffer[200];

    while(pids[i]){
        FILE* fp = fopen(fileWpath(pids[i], caminho), "r");
        fgets(linha, 300, fp);
        sscanf(linha, "%d,%[^,],%d", &pid, msg, &time);
        if(strcmp(cmd, msg) == 0)count++;
        i++;
        fclose(fp);
    }

    snprintf(buffer, sizeof(buffer), "%s was executed %d times\n", cmd, count);
    write(writefifo, buffer, strlen(buffer) + 1);
    close(writefifo);
}

void statsuniq(int pid_tracer, char* caminho, int* pids){
    int writefifo = open(fileName(pid_tracer), O_WRONLY);

    int i = 0;
    while(pids[i]){
        FILE* fp = fopen(fileWpath(pids[i], caminho), "r");
        char linha[300], msg[200], buffer[300];
        int pid, time;
        fgets(linha, 300, fp);
        sscanf(linha, "%d,%[^,],%d", &pid, msg, &time);
        snprintf(buffer, sizeof(buffer), "%03ld%s\n", strlen(msg) + 2, msg);
        write(writefifo, buffer, strlen(buffer) + 1);
        i++;
        fclose(fp);
    }
    close(writefifo);
}


void execute(int fifo, char buffer[]){
    
    char *argv[numberSpaces(buffer) + 1];
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

int main(int argc, char* argv[]){

    if(argc < 3){
        puts("ERROR few arguments!!!");
        return -1;
    }
    char* caminho = argv[1];
    char* tempfile = argv[2];
    if(openFile(tempfile) < 0) return -1;

    int f;
    if ((f = mkfifo(PIPEGLOBAL, 0666)) < 0) puts("ficheiro ja existe!");
    if ((f = mkfifo(PIPEGLOBAL2, 0666)) < 0) puts("ficheiro ja existe!");
    puts("Servidor aberto!!");

    //variaveis
    char option[2], buffer[BUFFER_SIZE], message[MAX_ARGS_CARACTERS];
    int NbytesRead, OP, tamanho;

    int doubleprocess = fork();
    if(doubleprocess == 0){
        int fifo = open(PIPEGLOBAL, O_RDWR);
        while((NbytesRead = read(fifo, option, 1)) > 0){   
            option[NbytesRead] = '\0';
            OP = atoi(option);
            if(OP == 1 || OP == 2){
                puts("Nova Conexão!!");
                tamanho = messageSize(fifo);
                NbytesRead = read(fifo, buffer, tamanho);
                buffer[NbytesRead] = '\0';

                int pid;
                long int start_sec, start_milisec;
                sscanf(buffer, "%d,%ld.%ld,%[^;]",&pid, &start_sec, &start_milisec, message);
                writeLine(pid, tempfile, cleanArguments(message), start_sec, start_milisec);

                pid_t filho = fork();
                if(filho == 0){
                    //abrir o pipe de escrita
                    int writefifo = open(fileName(pid), O_WRONLY);

                    if(OP == 1){//execuçao unica       
                        
                        printf("PID %d A EXECUTAR!!!\n",pid);
                        execute(writefifo, message);
                        
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
                
                activity(pid, tempfile);
                //printFile(datafile);//ver o q esta escrito no file
                
            }else if(OP == 9){
                puts("FIM de Conexão!!"); //alguam coisa foi colocada no pipe
                tamanho = messageSize(fifo);
                NbytesRead = read(fifo, buffer, tamanho);
                buffer[NbytesRead] = '\0';
                
                int pid;
                long int end_sec, end_milisec;
                sscanf(buffer, "%d,%ld.%ld",&pid, &end_sec, &end_milisec);

                int time = updateLine(pid, tempfile, caminho, end_sec, end_milisec);//return time

                printf("PID %d TERMINOU!!\nEnded in %d ms\n", pid, time);
            }else puts("ERROR");  
        }
        close(fifo);
    }else {
        int fifo = open(PIPEGLOBAL2, O_RDWR);
        while((NbytesRead = read(fifo, option, 1)) > 0){
            option[NbytesRead] = '\0';
            OP = atoi(option);
            if(OP == 4){
                puts("STATS-TIME");
                tamanho = messageSize(fifo);
                NbytesRead = read(fifo, buffer, tamanho);
                buffer[NbytesRead] = '\0';
                int pid_tracer;
                sscanf(buffer, "%d,%[^;]",&pid_tracer, message);
                pid_t filho = fork();
                if(filho == 0){
                    timestats(pid_tracer, caminho, arraypid(message, numberSpaces(message)));
                    _exit(1);
                }
            }else if(OP == 5){
                puts("STATS-COMMAND");
                tamanho = messageSize(fifo);
                NbytesRead = read(fifo, buffer, tamanho);
                buffer[NbytesRead] = '\0';
                int pid_tracer;
                char cmd[20];
                sscanf(buffer, "%d,%[^,],%[^;]",&pid_tracer, cmd, message);
                pid_t filho = fork();
                if(filho == 0){
                    procuracomando(pid_tracer, caminho, cmd, arraypid(message, numberSpaces(message)));
                    _exit(1);
                }
                
            }else if(OP == 6){
                puts("STATS-UNIQ");
                tamanho = messageSize(fifo);
                NbytesRead = read(fifo, buffer, tamanho);
                buffer[NbytesRead] = '\0';
                int pid_tracer;
                sscanf(buffer, "%d,%[^;]",&pid_tracer, message);
                pid_t filho = fork();
                if(filho == 0){
                    statsuniq(pid_tracer, caminho, arraypid(message, numberVirgulas(message)));
                    _exit(1);
                }
            }else puts("ERROR");
        }
        close(fifo);
    }
    return 0;
}
