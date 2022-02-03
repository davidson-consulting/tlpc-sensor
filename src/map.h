#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <time.h>

#ifndef MAP_H
#define MAP_H

struct value {
    int rapl_group_leader_fd;
    int perf_group_leader_fd;
    clock_t starting_time;
};

struct entry {
    char *identifier;
    struct value group_leaders;
};

#define SIZE_OF_MAP 1024

struct entry map[SIZE_OF_MAP];

int
map_contains(const char *identifier);

struct value 
map_get(const char *identifier);

int
map_put(const char *identifier,  int rapl_group_leader_fd, int perf_group_leader_fd);

int
map_remove(const char *identifier);

int 
map_get_hash(const char *identifier);

#endif