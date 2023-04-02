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
        int fifo = open("logs",O_WRONLY), n = 1;
        puts("Novo cliente !");
        char buffer[1024];
        int i = 2;

        char str_len[512];
        sprintf(str_len,"%4ld",strlen(argv[n])+1);
        strcat(str_len, argv[n]);
        write(fifo,str_len,strlen(str_len)+1);
        n++;
        
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