#include <sys/stat.h>

#include <unistd.h>
#include "mensagem/mensagem.h"
#include "fileWritting/fileWritting.h"
#include "auxFunc/auxFunc.h"
#include "hTable/hTable.h"

void timestats(int pid_tracer, char* caminho, int* pids){
    int writefifo = open(fileName(pid_tracer), O_WRONLY);
    char buffer[BUFFER_SIZE];
    int i = 0, count = 0;
    while(pids[i]){
        int fp = open(fileWpath(pids[i], caminho), O_RDONLY);
        if(fp > 0){
            char linha[300], msg[200];
            int pid, time;
            read(fp, linha, 300);
            sscanf(linha, "%d,%[^,],%d", &pid, msg, &time);
            count = count + time;
        }
        i++;
        close(fp);
    }
    snprintf(buffer, sizeof(buffer), "%03dTotal execution time is %d ms\n", digitCount(count) + 30, count);
    write(writefifo, buffer, strlen(buffer) + 1);
    close(writefifo);
}

void procuracomando(int pid_tracer, char* caminho, char* cmd, int* pids){
    int writefifo = open(fileName(pid_tracer), O_WRONLY);
    int count = 0, i = 0, pid, time;
    char linha[300], msg[200], buffer[200];

    while(pids[i]){
        int fp = open(fileWpath(pids[i], caminho), O_RDONLY);
        if(fp > 0){
            read(fp, linha, 300);
            sscanf(linha, "%d,%[^,],%d", &pid, msg, &time);
            if(strcmp(cmd, msg) == 0)count++;
        }
        i++;
        close(fp);
    }

    snprintf(buffer, sizeof(buffer), "%03ld%s was executed %d times\n", digitCount(count) + strlen(cmd) + 23, cmd, count);
    write(writefifo, buffer, strlen(buffer) + 1);
    close(writefifo);
}

void statsuniq(int pid_tracer, char* caminho, int* pids){
    int writefifo = open(fileName(pid_tracer), O_WRONLY);
    char linha[300], msg[200], buffer[300];
    int pid, time, i = 0;
    
    while(pids[i]){
        int fp = open(fileWpath(pids[i], caminho), O_RDONLY);
        if(fp >= 0){
            read(fp, linha, 300);
            sscanf(linha, "%d,%[^,],%d", &pid, msg, &time);
            snprintf(buffer, sizeof(buffer), "%03ld%s\n", strlen(msg) + 2, msg);
            write(writefifo, buffer, strlen(buffer) + 1);
        }
        i++;
        close(fp);
    }
    close(writefifo);
}

int main(int argc, char* argv[]){

    if(argc < 2){
        puts("ERROR few arguments!!!");
        return -1;
    }
    char* caminho = argv[1];
    Cat_Stats status_hTable = create_hashTable();

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
                tamanho = messageSize(fifo);
                NbytesRead = read(fifo, buffer, tamanho);
                buffer[NbytesRead] = '\0';

                int pid;
                long int start_sec, start_milisec;
                sscanf(buffer, "%d,%ld.%ld,%[^;]", &pid, &start_sec, &start_milisec, message);
                char* pid_str = fileName(pid);

                Pid_Stat new_process = create_stat(pid_str, message, start_sec, start_milisec);
                insert_stats(pid_str, new_process, status_hTable);
                printf("PID %d A EXECUTAR!!!\n",pid);
               

            }else if(OP == 3){
                puts("STATUS");
                tamanho = messageSize(fifo);
                int pid = get_pid(fifo, tamanho);
                
                int filho2 = fork();
                if(filho2 == 0){
                    char* pid_str = fileName(pid);
                    int writefifo = open(pid_str, O_WRONLY);

                    dup2(writefifo, 1);
                    percorre_status(status_hTable);
                
                    close(writefifo);
                    _exit(1);
                }
                
            }else if(OP == 9){
                //alguam coisa foi colocada no pipe
                tamanho = messageSize(fifo);
                NbytesRead = read(fifo, buffer, tamanho);
                buffer[NbytesRead] = '\0';
                
                int pid;
                long int end_sec, end_milisec;
                sscanf(buffer, "%d,%ld.%ld",&pid, &end_sec, &end_milisec);
                char* pid_str = fileName(pid);

                Pid_Stat s_pid = search_stat(pid_str, status_hTable);
                int time = create_file(caminho, s_pid, end_sec, end_milisec);//return time
                remove_stats(pid_str, status_hTable);

                printf("PID %d TERMINOU em %d ms\n", pid, time); 

            }else puts("ERROR");  
        }
        close(fifo);
    }else{
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
                    timestats(pid_tracer, caminho, arraypid(message, numberVirgulas(message) + 1));
                    _exit(1);
                }

            }else if(OP == 5){
                puts("STATS-COMMAND");
                tamanho = messageSize(fifo);
                NbytesRead = read(fifo, buffer, tamanho);
                buffer[NbytesRead] = '\0';
                int pid_tracer;
                char cmd[20];
                sscanf(buffer, "%d,%[^,],%[^;]", &pid_tracer, cmd, message);
                pid_t filho = fork();
                if(filho == 0){
                    procuracomando(pid_tracer, caminho, cmd, arraypid(message, numberVirgulas(message) + 1));
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
                    statsuniq(pid_tracer, caminho, arraypid(message, numberVirgulas(message) + 1));
                    _exit(1);
                }
                
            }else puts("ERROR");
        }
        close(fifo);
    }
    return 0;
}
