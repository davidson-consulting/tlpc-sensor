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
#include <errno.h>

#include "config.h"
#include "perf.h"
#include "map.h"

#ifndef SENSOR_H
#define SENSOR_H

/*
 * perf_counter_value stores the counter value.
 */
struct perf_counter_value {
    uint64_t value;
};

/*
 * perf_cpu_report stores the events counter value.
 */
struct perf_read_format {
    uint64_t nr;
    uint64_t time_enabled; /* PERF_FORMAT_TOTAL_TIME_ENABLED flag */
    uint64_t time_running; /* PERF_FORMAT_TOTAL_TIME_RUNNING flag */
    struct perf_counter_value values[];
};

int
sensor_init_perf_read_format(int nb_counter, struct perf_read_format *buffer);

int sensor_init(struct config *config_perf, struct config *config_rapl, pid_t pid, const char *identifier);

int sensor_start(const char *identifier);

int sensor_stop(const char *identifier);

int sensor_terminate(const char *identifier);

int sensor_read(const char *identifier, 
                struct perf_read_format *perf_buffer, size_t perf_buffer_size, 
                struct perf_read_format *rapl_buffer, size_t rapl_buffer_size);

#endif