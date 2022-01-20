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

#define nb_perf_counter 3

char* perf_counters_type[nb_perf_counter] = {
    "INSTRUCTIONS_RETIRED",
    "LLC_MISSES",
    "CYCLES"
};

#define nb_rapl_counter 1

char* rapl_counters_type[nb_rapl_counter] = {
    "RAPL_ENERGY_PKG"
};

int
main(int argc, char **argv) {
    printf("%d %s\n", argc, argv[0]);
    struct config *config_perf = init_config(nb_perf_counter, perf_counters_type);
    struct config *config_rapl = init_config(nb_rapl_counter, rapl_counters_type);
    server_run(config_perf, config_rapl);
    return EXIT_SUCCESS;
}