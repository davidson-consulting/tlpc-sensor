#include <unistd.h>
#include <string.h>
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
