#include "sensor.h"
#include "perf.h"

static int group_leader_fd;

int sensor_init(char** event_names, int nb_event) {
    perf_initialize();
    group_leader_fd = -1;
    for (int i = 0 ; i < nb_event ; i++) {
        printf("Initialize %s\n", event_names[i]);
        struct perf_event_attr attr = {0};
        perf_init_event_attr(event_names[i], &attr);
        group_leader_fd = perf_open_event(&attr, 0, -1, group_leader_fd, 0);
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