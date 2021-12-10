#include "perf.h"

int
perf_initialize() {
    if (pfm_initialize() != PFM_SUCCESS) {
        fprintf(stderr, "Error when initializing pfm\n");
        exit(EXIT_FAILURE);
    }
    return PFM_SUCCESS;
}

int
perf_init_event_attr(char* event_name, struct perf_event_attr* attr) {
    attr->size = sizeof(*attr);
    attr->disabled = 1;
    attr->read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING | PERF_FORMAT_GROUP;
    pfm_perf_encode_arg_t arg = {0};
    arg.size = sizeof(pfm_perf_encode_arg_t);
    arg.attr = attr;
    if (pfm_get_os_event_encoding(event_name, PFM_PLM0 | PFM_PLM3, PFM_OS_PERF_EVENT_EXT, &arg)  != PFM_SUCCESS) {
        fprintf(stderr, "Error when getting event encoding %s\n", event_name);
        exit(EXIT_FAILURE);
    }
    return PFM_SUCCESS;
}

int
perf_open_event(struct perf_event_attr* attr, int pid, int cpu, int group_fd, int flags) {
    int fd = perf_event_open(attr, pid, cpu, group_fd, flags);
    if (fd == -1) {
        fprintf(stderr, "Error opening fd: %d, %d, %d, %d\n", pid, cpu, group_fd, flags);
        exit(EXIT_FAILURE);
    }
    return group_fd == -1 ? fd : group_fd;
}

int
perf_terminate() {
    pfm_terminate();
    return 0;
}