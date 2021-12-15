#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <linux/hw_breakpoint.h>
#include <sys/syscall.h>
#include <perfmon/pfmlib_perf_event.h>
#include <fcntl.h>
#include <stddef.h>

#include "config.h"

struct config* init_config(int nb_counter, char* counter_names[]) {
    size_t configuration_size;
    struct config *config = NULL;
    configuration_size = offsetof(struct config, perf_counters_names) + sizeof(struct perf_counter_name[(int)nb_counter]);
    config = (struct config *) malloc(configuration_size);
    config->nb_counter = nb_counter;
    for (int i = 0 ; i < nb_counter ; i++) {
        config->perf_counters_names[i].value = counter_names[i];
    }
    return config;
}