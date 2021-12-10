#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <linux/hw_breakpoint.h>
#include <sys/syscall.h>
#include <fcntl.h>

#include <perfmon/pfmlib_perf_event.h>

#define nb_configs 1

const int counters_type[nb_configs] = {
    PERF_COUNT_HW_INSTRUCTIONS,
    //PERF_COUNT_HW_REF_CPU_CYCLES//,
    // PERF_COUNT_HW_CACHE_REFERENCES,
    // PERF_COUNT_HW_CACHE_MISSES,
    // PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
    // PERF_COUNT_HW_BRANCH_MISSES
};

const char* labels_counters[nb_configs] = {
    "energy"
    // "instructions",
    // "cycles"//,
    // "cache-reference",
    // "cache-misses",
    // "branches",
    // "branch-misses"
};

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
    struct perf_counter_value values[nb_configs];
};

int 
main(int argc, char **argv)
{
    // making gcc happy
    printf("%d %s", argc, argv[0]);

    if (pfm_initialize() != PFM_SUCCESS) {
        fprintf(stderr, "Error when initializing pfm\n");
        exit(EXIT_FAILURE);
    }

    // perf counters
    struct perf_event_attr pe_leader = {0};
    pe_leader.size = sizeof(pe_leader);
    pe_leader.disabled = 1;
    pe_leader.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING | PERF_FORMAT_GROUP;
    
    pfm_perf_encode_arg_t arg = {0};
    arg.size = sizeof(pfm_perf_encode_arg_t);
    arg.attr = &pe_leader;

    if (pfm_get_os_event_encoding("RAPL_ENERGY_PKG", PFM_PLM0 | PFM_PLM3, PFM_OS_PERF_EVENT_EXT, &arg) != PFM_SUCCESS) {
        fprintf(stderr, "Error converting rapl event \n");
        exit(EXIT_FAILURE);
    }

    int group_fd_leader = perf_event_open(&pe_leader, -1, 0, -1, 0);
    if (group_fd_leader == -1) {
        fprintf(stderr, "Error opening leader %llx\n", pe_leader.config);
        exit(EXIT_FAILURE);
    }
    
    // for (int i = 0 ; i < 1 ; i++) {
    //     struct perf_event_attr pe = {0};
    //     pe.size = sizeof(pe);
    //     pe.disabled = 0;
    //     pe.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING | PERF_FORMAT_GROUP;

    //     pfm_perf_encode_arg_t arg = {0};
    //     arg.size = sizeof(pfm_perf_encode_arg_t);
    //     arg.attr = &pe;

    //     if (pfm> -1) {
    //         fprintf(stderr, "Error opening %llx\n", pe.config);
    //         exit(EXIT_FAILURE);
    //     }
    // }

    ioctl(group_fd_leader, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
    ioctl(group_fd_leader, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
    for (int i = 0 ; i < 1000000 ; i++) {
        printf("%d\n", i);    
    }
    ioctl(group_fd_leader, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

    struct perf_read_format buffer;
    int len = read(group_fd_leader, &buffer, sizeof(buffer));
    if (len < 0) {
        fprintf(stderr, "%d\n", len);
        exit(EXIT_FAILURE);
    }
    printf("%ld %ld %ld\n", buffer.nr, buffer.time_enabled, buffer.time_running);
    for (int i = 0 ; i < nb_configs ; i++) {
        printf("%ld %s\n", buffer.values[i].value, labels_counters[i]);
    }
    
    close(group_fd_leader);
    pfm_terminate();
    exit(EXIT_SUCCESS);
}