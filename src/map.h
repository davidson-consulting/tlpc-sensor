#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

#ifndef MAP_H
#define MAP_H

struct group_leaders_fd {
    int rapl_group_leader_fd;
    int perf_group_leader_fd;
};

struct map_group_leader_fd_by_id {
    char *identifier;
    struct group_leaders_fd group_leaders;
};

#define SIZE_OF_MAP 1024

struct map_group_leader_fd_by_id map[SIZE_OF_MAP];

int
map_contains(const char *identifier);

struct group_leaders_fd 
map_get(const char *identifier);

int
map_put(const char *identifier,  int rapl_group_leader_fd, int perf_group_leader_fd);

int
map_remove(const char *identifier);

int 
map_get_hash(const char *identifier);

#endif