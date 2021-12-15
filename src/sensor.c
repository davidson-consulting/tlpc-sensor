#include "sensor.h"
#include "perf.h"

static int group_leader_fd;

int sensor_init(struct perf_counter_name perf_counters_names[], int nb_event, pid_t pid) {
    perf_initialize();
    group_leader_fd = -1;
    for (int i = 0 ; i < nb_event ; i++) {
        printf("Initialize %s%d\n", perf_counters_names[i].value, pid);
        struct perf_event_attr attr = {0};
        perf_init_event_attr(perf_counters_names[i].value, &attr);
        group_leader_fd = perf_open_event(&attr, pid, -1, group_leader_fd, 0);
    }
    return 0;
}

int sensor_start() {
    if (ioctl(group_leader_fd, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP) != 0) {
        printf("Something went wrong when reseting group leader fd %d", group_leader_fd);
        exit(EXIT_FAILURE);
    }
    if (ioctl(group_leader_fd, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP) != 0) {
        printf("Something went wrong when enabling group leader fd %d", group_leader_fd);
        exit(EXIT_FAILURE);
    }
    return 0;
}

int sensor_stop() {
    return ioctl(group_leader_fd, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
}

int sensor_terminate() {
    perf_terminate();
    close(group_leader_fd);
    return 0;
}

int sensor_read(struct perf_read_format* buffer, size_t buffer_size) {
    return read(group_leader_fd, buffer, buffer_size);
}