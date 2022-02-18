#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

#ifndef MAP_H
#define MAP_H

struct map_entry {
    char *identifier;
    int rapl_group_leader_fd;
    int perf_group_leader_fd;
};

#define SIZE_OF_MAP 655360

struct map_entry map[SIZE_OF_MAP];

int
map_contains(const char *identifier);

struct map_entry 
map_get(const char *identifier);

int
map_put(const char *identifier,  int perf_group_leader_fd, int rapl_group_leader_fd);

int
map_remove(const char *identifier);

int 
map_get_hash(const char *identifier);

#endif