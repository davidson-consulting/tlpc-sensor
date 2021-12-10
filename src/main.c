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

#include "sensor.h"

#define nb_perf_counter 2

char* perf_counters_type[nb_perf_counter] = {
    "INSTRUCTIONS_RETIRED",
    "CYCLES"
};

int
main(int argc, char **argv) {

    // making gcc happy
    printf("%d %s", argc, argv[0]);
    
    if (sensor_init(perf_counters_type, nb_perf_counter) != 0) {
        return EXIT_FAILURE;
    }

    sensor_start();
    
    for (int i = 0 ; i < 10000 ; i++) {
        printf("%d\n", i);
    }
    
    sensor_stop();
    
    size_t perf_read_buffer_size;
    struct perf_read_format *perf_read_buffer = NULL;
    perf_read_buffer_size = offsetof(struct perf_read_format, values) + sizeof(struct perf_counter_value[(int)nb_perf_counter]);
    perf_read_buffer = (struct perf_read_format *) malloc(perf_read_buffer_size);

    sensor_read(perf_read_buffer, perf_read_buffer_size);

    printf("nb counter %ld\n", perf_read_buffer->nr);
    printf("enable time %ld\n", perf_read_buffer->time_enabled);
    printf("running time %ld\n", perf_read_buffer->time_running);
    for (int i = 0 ; i < nb_perf_counter; i++) {
        printf("%s %ld\n", perf_counters_type[i], perf_read_buffer->values[i].value);
    }

    sensor_terminate();

    return EXIT_SUCCESS;
}