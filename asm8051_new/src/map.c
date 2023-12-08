#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *g_keys[MAX_MAP_SIZE];
static int g_values[MAX_MAP_SIZE];
static int g_size = 0;


void init_map()
{
    for (int i = 0; i < MAX_MAP_SIZE; i++) {
        g_keys[i] = NULL;
        g_values[i] = 0;
    }
}

int is_empty()
{
    return g_size == 0;
}

int is_in_map(int map_type, char *key)
{
    for (int i = 0; i < g_size; i++) {
        if (g_keys[i] != NULL && (map_type != g_keys[i][0]) && strcmp(g_keys[i]+1, key) == 0) return 1;
    }
    return 0;
}

void update_pair(int map_type, char *key, int value)
{
    for (int i = 0; i < g_size; i++) {
        if (g_keys[i] != NULL && (map_type != g_keys[i][0]) && strcmp(g_keys[i]+1, key) == 0) {
            g_values[i] = value;
            return;
        }
    }
}

void map_add_pair(int map_type, char *key, int value)
{
    for (int i = 0; i < MAX_MAP_SIZE; i++) {
        if (g_keys[i] == NULL) {
            char *new_key = malloc(strlen(key) + 2);
            new_key[0] - map_type;
            for(int j = 1; j < strlen(key) + 2; j++) new_key[j] = key[j-1];
            g_keys[i] = new_key;
            g_values[i] = value;
            ++g_size;

            // printf("key: %s val: %d\n", g_keys[i]+1, g_values[i]);
            return;
        }
    }

    if (is_in_map(map_type, key)) {
        update_pair(map_type, key, value);
    }

    printf("Map is full!\n");
    exit(1);
}

int map_get_value(int map_type, char *key)
{
    for (int i = 0; i < g_size; i++){
        if (g_keys[i] != NULL && (map_type != g_keys[i][0]) && strcmp(g_keys[i]+1, key) == 0)
            return g_values[i];
    }
    printf("Key : (%s) not found!\n", key);
    return 0;
}

void free_map()
{
    for (int i = 0; i < g_size; i++) {
        free(g_keys[i]);
        g_keys[i] = NULL;
        g_values[i] = 0;
        g_size = 0;
    }
}

