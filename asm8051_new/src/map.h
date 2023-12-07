#ifndef __MAP_H__
#define __MAP_H__

#define MAX_MAP_SIZE 1024


void init_map();

int is_empty();

int is_in_map(char *key);

void map_add_pair(char *key, int value);

int map_get_value(char *key);

void free_map();


#endif // __MAP_H__