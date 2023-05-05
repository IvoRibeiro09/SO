#include <glib.h>

//struct hash table
struct Stat {
	char* pid;
	long int start_sec;
	long int start_milisec;
};

struct Cat_stats {
	GHashTable* stats;
};

Cat_Stats create_hashTable() {
	Cat_Stats hTable = malloc(sizeof(struct cat_stats));
	hTable->stats = g_hash_table_new(g_str_hash, g_str_equal);
	return hTable;
}

Cat_Stats insert_driver(char *key, Stat data, Cat_Stats hTable) {
	g_hash_table_insert(hTable->stat, key, data);
	return hTable;
}
//inserir na hastable

//remover da hashtable
