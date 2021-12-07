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
    PERF_COUNT_HW_CPU_CYCLES,
    PERF_COUNT_HW_CACHE_REFERENCES,
    PERF_COUNT_HW_CACHE_MISSES,
    PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
    PERF_COUNT_HW_BRANCH_MISSES,
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
    struct perf_counter_value values[];
};

int 
main(int argc, char **argv)
{
    int perf_flags = 0;
    struct perf_event_attr pe;
    
    memset(&pe, 0, sizeof(pe));
    pe.type = PERF_TYPE_HARDWARE;
    pe.config = PERF_COUNT_HW_CPU_CYCLES;
    pe.size = sizeof(pe);
    pe.disabled = 1;
    pe.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING | PERF_FORMAT_GROUP;
    pe.exclude_kernel = 1;
    pe.exclude_hv = 1;

    //  static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags)
    int fd_leader = perf_event_open(&pe, 0, -1, -1, perf_flags);
    if (fd_leader == -1) {
        fprintf(stderr, "Error opening leader %llx\n", pe.config);
        exit(EXIT_FAILURE);
    }

    struct perf_event_attr pe_instr;
    memset(&pe_instr, 0, sizeof(pe_instr));
    pe_instr.type = PERF_TYPE_HARDWARE;
    pe_instr.config = PERF_COUNT_HW_INSTRUCTIONS;
    pe_instr.size = sizeof(pe);
    pe_instr.disabled = 0;
    pe_instr.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING | PERF_FORMAT_GROUP;
    pe_instr.exclude_kernel = 1;
    pe_instr.exclude_hv = 1;
    
    int fd_instr = perf_event_open(&pe_instr, 0, -1, fd_leader, perf_flags);
    if (fd_instr == -1) {
        fprintf(stderr, "Error opening instruction %llx\n", pe_instr.config);
        exit(EXIT_FAILURE);
    }

    int errno = ioctl(fd_leader, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
    printf("%d\n", errno);
    errno = ioctl(fd_leader, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
    printf("%d\n", errno);
    printf("Measuring instruction count for this printf\n");
    errno = ioctl(fd_leader, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
    printf("%d\n", errno);

    struct perf_read_format buffer;
     memset(&pe_instr, 0, sizeof(pe_instr));
    int len = read(fd_leader, &buffer, sizeof(buffer));
    if (len < 0) {
        fprintf(stderr, "%d\n", len);
        exit(EXIT_FAILURE);
    }
    printf("%ld %ld %ld\n", buffer.nr, buffer.time_enabled, buffer.time_running);
    printf("%ld cycles\n", &buffer.values[0].value);
    printf("%ld instructions\n", &buffer.values[1].value);
    
    close(fd_leader);
    close(fd_instr);
}