#include <unistd.h>
#include <string.h>

#ifndef CONFIG_H
#define CONFIG_H

struct counter_name {
    char *value;
};

struct config {
    int nb_counter;
    struct counter_name counters_names[];   
};

struct config *init_config(int nb_counter, char* counters_names[]);

#endif
