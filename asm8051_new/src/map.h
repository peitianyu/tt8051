#ifndef __MAP_H__
#define __MAP_H__

#define MAX_MAP_SIZE 1024

enum MapType {
    SFR_MAP_TYPE = 1,
    SBIT_MAP_TYEP,
    LABEL_MAP_TYPE
};

void init_map();

int is_empty();

int is_in_map(int map_type, char *key);

void map_add_pair(int map_type, char *key, int value);

int map_get_value(int map_type, char *key);

void free_map();


#endif // __MAP_H__