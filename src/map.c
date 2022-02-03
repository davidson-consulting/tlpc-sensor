#include "map.h"

int
contains(char *identifier) {
    const int hash = get_hash(identifier);
    return strcmp(map[hash].identifier, identifier);
}

struct group_leaders_fd 
get(char *identifier) {
    const int hash = get_hash(identifier);
    return map[hash].group_leaders;
}

int
put(char *identifier,  int rapl_group_leader_fd, int perf_group_leader_fd) {
    if (contains(identifier)) {
        printf("The map contains already an association for the key %s\n", identifier);
        return 1;
    }
    const int hash = get_hash(identifier);
    map[hash].identifier = identifier;
    map[hash].group_leaders.perf_group_leader_fd = perf_group_leader_fd;
    map[hash].group_leaders.rapl_group_leader_fd = rapl_group_leader_fd;
    return 0;
}

int
remove (char *identifier) {
    if (!contains(identifier)) {
        printf("The map does not contain an association for the key %s\n", identifier);
        return 1;
    }
    const int hash = get_hash(identifier);
    map[hash].identifier = "";
    return 0;
}

int 
get_hash(char *identifier) {
    unsigned long hash = 5381;
    int c;
    while (c = *identifier++) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}