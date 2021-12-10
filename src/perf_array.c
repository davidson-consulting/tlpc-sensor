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


#define nb_configs 6

const int counters_type[nb_configs] = {
    PERF_COUNT_HW_INSTRUCTIONS,
    PERF_COUNT_HW_REF_CPU_CYCLES,
    PERF_COUNT_HW_CACHE_REFERENCES,
    PERF_COUNT_HW_CACHE_MISSES,
    PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
    PERF_COUNT_HW_BRANCH_MISSES
};

const char* labels_counters[nb_configs] = {
    "instructions",
    "cycles",
    "cache-reference",
    "cache-misses",
    "branches",
    "branch-misses"
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

    struct perf_event_attr pe_leader;
    memset(&pe_leader, 0, sizeof(pe_leader));
    pe_leader.type = PERF_TYPE_HARDWARE;
    pe_leader.config = counters_type[0];
    pe_leader.size = sizeof(pe_leader);
    pe_leader.disabled = 1;
    pe_leader.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING | PERF_FORMAT_GROUP;
    pe_leader.exclude_kernel = 1;
    pe_leader.exclude_hv = 1;
    int group_fd_leader = perf_event_open(&pe_leader, 0, -1, -1, 0);
    if (group_fd_leader == -1) {
        fprintf(stderr, "Error opening leader %llx\n", pe_leader.config);
        exit(EXIT_FAILURE);
    }
    
    for (int i = 1 ; i < nb_configs ; i++) {
        struct perf_event_attr pe;
        memset(&pe, 0, sizeof(pe));
        pe.type = PERF_TYPE_HARDWARE;
        pe.config = counters_type[i];
        pe.size = sizeof(pe);
        pe.disabled = 0;
        pe.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING | PERF_FORMAT_GROUP;
        pe.exclude_kernel = 1;
        pe.exclude_hv = 1;
        int fd = perf_event_open(&pe, 0, -1, group_fd_leader, 0);
        if (fd == -1) {
            fprintf(stderr, "Error opening %llx\n", pe.config);
            exit(EXIT_FAILURE);
        }
    }
    
    ioctl(group_fd_leader, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
    ioctl(group_fd_leader, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
    printf("Measuring instruction count for this printf\n");
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
}