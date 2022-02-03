#include "sensor.h"

int
sensor_init_perf_read_format(int nb_counter, struct perf_read_format *buffer) {
    size_t buffer_size = offsetof(struct perf_read_format, values) + sizeof(struct perf_counter_value[nb_counter]);
    buffer = (struct perf_read_format *) malloc(buffer_size);
    (void)buffer;
    return buffer_size;
}

int sensor_init(struct config *config_perf, struct config *config_rapl, pid_t pid, const char *identifier) {
    perf_initialize();
    if (map_contains(identifier)) {
        return -1;
    }
    int perf_group_leader_fd = -1;
    for (int i = 0 ; i < config_perf->nb_counter ; i++) {
        printf("Initialize %s (%d)\n", config_perf->counters_names[i].value, pid);
        struct perf_event_attr attr = {0};
        perf_init_event_attr(config_perf->counters_names[i].value, &attr);
        perf_group_leader_fd = perf_open_event(&attr, pid, -1, perf_group_leader_fd, 0);
    }
    int rapl_group_leader_fd = -1;
    for (int i = 0 ; i < config_rapl->nb_counter ; i++) {
        printf("Initialize %s (%d)\n", config_rapl->counters_names[i].value, pid);
        struct perf_event_attr attr = {0};
        perf_init_event_attr(config_rapl->counters_names[i].value, &attr);
        rapl_group_leader_fd = perf_open_event(&attr, -1, 0, rapl_group_leader_fd, 0);
    }
    map_put(identifier, rapl_group_leader_fd, perf_group_leader_fd);
    return 0;
}

int 
sensor_start(const char *identifier) {
    struct group_leaders_fd leaders = map_get(identifier);
    int perf_group_leader_fd = leaders.perf_group_leader_fd;
    int rapl_group_leader_fd = leaders.rapl_group_leader_fd;
    if (ioctl(rapl_group_leader_fd, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP) != 0) {
        printf("Something went wrong when reseting rapl group leader fd %d", rapl_group_leader_fd);
        exit(EXIT_FAILURE);
    }
    if (ioctl(perf_group_leader_fd, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP) != 0) {
        printf("Something went wrong when reseting perf group leader fd %d", perf_group_leader_fd);
        exit(EXIT_FAILURE);
    }
    if (ioctl(rapl_group_leader_fd, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP) != 0) {
        printf("Something went wrong when enabling rapl group leader fd %d", rapl_group_leader_fd);
        exit(EXIT_FAILURE);
    }
    if (ioctl(perf_group_leader_fd, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP) != 0) {
        printf("Something went wrong when enabling perf group leader fd %d", perf_group_leader_fd);
        exit(EXIT_FAILURE);
    }
    return 0;
}

int 
sensor_stop(const char *identifier) {
    struct group_leaders_fd leaders = map_get(identifier);
    int perf_group_leader_fd = leaders.perf_group_leader_fd;
    int rapl_group_leader_fd = leaders.rapl_group_leader_fd;
    ioctl(rapl_group_leader_fd, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
    return ioctl(perf_group_leader_fd, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
}

int 
sensor_terminate(const char *identifier) {
    struct group_leaders_fd leaders = map_get(identifier);
    int perf_group_leader_fd = leaders.perf_group_leader_fd;
    int rapl_group_leader_fd = leaders.rapl_group_leader_fd;
    perf_terminate();
    close(rapl_group_leader_fd);
    close(perf_group_leader_fd);
    return 0;
}

int 
sensor_read(const char *identifier,
            struct perf_read_format *perf_buffer, size_t perf_buffer_size, 
            struct perf_read_format *rapl_buffer, size_t rapl_buffer_size) {
    struct group_leaders_fd leaders = map_get(identifier);
    int perf_group_leader_fd = leaders.perf_group_leader_fd;
    int rapl_group_leader_fd = leaders.rapl_group_leader_fd; 
    long unsigned int nb_read = read(perf_group_leader_fd, perf_buffer, perf_buffer_size);
    if (nb_read != perf_buffer_size) {
        printf("Error read %ld while should have read %ld\n", nb_read, perf_buffer_size);
    }
    nb_read = read(rapl_group_leader_fd, rapl_buffer, rapl_buffer_size);
    if (nb_read != rapl_buffer_size) {
        printf("Error read %ld while should have read %ld\n", nb_read, rapl_buffer_size);
    }
    return nb_read;
}