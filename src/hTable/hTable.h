#ifndef HTABLE_
#define HTABLE_
#include <glib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include "../mensagem/mensagem.h"

typedef struct cat_stats *Cat_Stats;
typedef struct pid_stat *Pid_Stat;

int get_stat_pid(Pid_Stat s);

char* get_stat_message(Pid_Stat s);

long int get_stat_sec(Pid_Stat s);

long int get_stat_milisec(Pid_Stat s);

Pid_Stat search_stat(char* key, Cat_Stats hTable);

Cat_Stats create_hashTable();

Pid_Stat create_stat(char* pid, char* msg, long int st_sec, int st_milisec);

void insert_stats(char* key, Pid_Stat data, Cat_Stats hTable);

void remove_stats(char* key, Cat_Stats hTable);

void print_stats_info(gpointer key, gpointer value, gpointer user_data);

void percorre_status(Cat_Stats hTable);

#endif