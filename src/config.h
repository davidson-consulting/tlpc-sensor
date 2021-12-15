#include <unistd.h>
#include <string.h>

#ifndef CONFIG_H
#define CONFIG_H

struct perf_counter_name {
    char *value;
};

struct config {
    int nb_counter;
    struct perf_counter_name perf_counters_names[];   
};

struct config *init_config(int nb_counter, char* perf_counters_names[]);

#endif