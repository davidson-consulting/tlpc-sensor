#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <time.h>

#ifndef MAP_H
#define MAP_H

struct map_entry {
    char *identifier;
    int rapl_group_leader_fd;
    int perf_group_leader_fd;
    clock_t starting_time;
    struct perf_read_format *perf_buffer;
    struct perf_read_format *rapl_buffer;
    unsigned long long elapsedTime;
};

#define SIZE_OF_MAP 1024

struct map_entry map[SIZE_OF_MAP];

char *keyset[SIZE_OF_MAP];

int size_keyset;

int
map_contains(const char *identifier);

struct map_entry 
map_get(const char *identifier);

int
map_put(const char *identifier,  int rapl_group_leader_fd, int perf_group_leader_fd);

int
map_store(const char* identifier, struct perf_read_format *perf_buffer,  struct perf_read_format *rapl_buffer, clock_t ending_time);

int
map_remove(const char *identifier);

int 
map_get_hash(const char *identifier);

#endif