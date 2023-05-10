#include "hTable.h"

//struct hash table
struct pid_stat{
    char* pid;
    char* msg;
    long int start_sec;
    long int start_milisec;
};

struct cat_stats {
    GHashTable* stats;
};

int get_stat_pid(Pid_Stat s){
    return atoi(s->pid);
}

char* get_stat_message(Pid_Stat s){
    return strdup(s->msg);
}

long int get_stat_sec(Pid_Stat s){
    return s->start_sec;
}

long int get_stat_milisec(Pid_Stat s){
    return s->start_milisec;
}

Pid_Stat search_stat(char* key, Cat_Stats hTable) {
	return (g_hash_table_lookup(hTable->stats, key));
}

//criar hastables
Cat_Stats create_hashTable() {
    Cat_Stats hTable = malloc(sizeof(struct cat_stats));
    hTable->stats = g_hash_table_new(g_str_hash, g_str_equal);
    return hTable;
}

Pid_Stat create_stat(char* pid, char* msg, long int st_sec, int st_milisec){
	Pid_Stat s = malloc(sizeof(struct pid_stat));

	s->pid = pid;
	s->start_sec = st_sec;
    s->msg = msg;
	s->start_milisec = st_milisec;

	return s;
}

//inserir na hastable
void insert_stats(char* key, Pid_Stat data, Cat_Stats hTable) {
    g_hash_table_insert(hTable->stats, key, data);
}

//remover da hastable
void remove_stats(char* key, Cat_Stats hTable) {
    g_hash_table_remove(hTable->stats, (key));
}

//percorre a hashtable
void print_stats_info(gpointer key, gpointer value, gpointer user_data) {
    Pid_Stat s = (struct pid_stat *) value;

    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    int time = getExecutionTime(s->start_sec, s->start_milisec, current_time.tv_sec, current_time.tv_usec);
    
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer),"%d %s %d ms\n", atoi(s->pid), s->msg, time);
    
    write(1, buffer, strlen(buffer) + 1);
}

void percorre_status(Cat_Stats hTable) {
    g_hash_table_foreach(hTable->stats, (GHFunc) print_stats_info, NULL);
}
