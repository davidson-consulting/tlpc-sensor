#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef MAP_H
#define MAP_H


struct map_group_leader_fd_by_id {
    char *identifier;
    struct group_leaders_fd group_leaders;
};

struct group_leaders_fd {
    int rapl_group_leader_fd;
    int perf_group_leader_fd;
};

#define SIZE_OF_MAP 1024

struct map_group_leader_fd_by_id map[SIZE_OF_MAP];

int
contains(char *identifier);

struct group_leaders_fd 
get(char *identifier);

int
put(char *identifier,  int rapl_group_leader_fd, int perf_group_leader_fd);

int
remove (char *identifier);

int 
get_hash(char *identifier);

#endif