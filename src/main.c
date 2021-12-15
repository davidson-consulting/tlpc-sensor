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
#include "server.h"
#include "config.h"

#define nb_perf_counter 2

char* perf_counters_type[nb_perf_counter] = {
    "INSTRUCTIONS_RETIRED",
    "CYCLES"
};

int
main(int argc, char **argv) {
    // making gcc happy
    printf("%d %s\n", argc, argv[0]);
    struct config *config = init_config(nb_perf_counter, perf_counters_type);
    server_run(config);
    return EXIT_SUCCESS;
}