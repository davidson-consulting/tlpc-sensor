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


int sensor_init(char** event_name, int nb_event);

int sensor_start();

int sensor_stop();

int sensor_terminate();

int sensor_read(struct perf_read_format* buffer, size_t buffer_size);