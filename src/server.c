#include "server.h"

int
server_init() {
    struct sockaddr_in server_addr;
    int socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    return socket_desc;   
}

int 
server_run(struct config *config_perf, struct config *config_rapl) {
    int socket_desc = server_init();
    struct sockaddr_in client_addr;
    char client_message[2000];
    socklen_t client_struct_length = sizeof(client_addr);
    while (true) {
        memset(client_message, '\0', sizeof(client_message));
        if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
            (struct sockaddr*)&client_addr, &client_struct_length) < 0){
            printf("Couldn't receive\n");
            return -1;
        }
        char *cursor = strtok(client_message, " ");
        if (strcmp("start", cursor) == 0) {
            server_start_sensor(socket_desc, config_perf, config_rapl, client_addr, client_struct_length);
        } else if (strcmp("stop", cursor) == 0) {
            server_stop_sensor(socket_desc, config_perf, config_rapl, client_addr, client_struct_length);
        } else if (strcmp("report", cursor) == 0) {
            server_report(socket_desc, config_perf, config_rapl, client_addr, client_struct_length);
        } else {
            printf("%s is not a valid command\n", client_message);
        }
    }
    server_stop(socket_desc);
    return 0;
}

int
server_start_sensor(int socket_desc, struct config *config_perf, struct config *config_rapl, struct sockaddr_in client_addr, socklen_t client_struct_length) {
    char *pid_str = strtok(NULL, " ");
    pid_t pid = atoi(pid_str);
    sensor_init(config_perf, config_rapl, pid);
    sensor_start();
    if (sendto(socket_desc, "ACK", 3, 0,
        (struct sockaddr*)&client_addr, client_struct_length) < 0){
        printf("Can't send\n");
        return -1;
    }
    return 0;
}

int
server_stop_sensor(int socket_desc, struct config *config_perf, struct config *config_rapl, struct sockaddr_in client_addr, socklen_t client_struct_length) {
    sensor_stop();

    // TODO: to be replaced by the code block below
    size_t perf_buffer_size = offsetof(struct perf_read_format, values) + sizeof(struct perf_counter_value[(int)config_perf->nb_counter]);
    struct perf_read_format *perf_buffer = (struct perf_read_format *) malloc(perf_buffer_size);
    size_t rapl_buffer_size = offsetof(struct perf_read_format, values) + sizeof(struct perf_counter_value[(int)config_rapl->nb_counter]);
    struct perf_read_format *rapl_buffer = (struct perf_read_format *) malloc(rapl_buffer_size);
    sensor_read(perf_buffer, perf_buffer_size, rapl_buffer, rapl_buffer_size);
    report_store(strtok(NULL, " "), perf_buffer, rapl_buffer);

    // TODO: here, I wanted to use a function to avoid redundancies, but GCC tells me: "*** stack smashing detected ***: terminated"
    // struct perf_read_format perf_buffer;
    // size_t perf_buffer_size = sensor_init_perf_read_format(config_perf->nb_counter, &perf_buffer);
    // struct perf_read_format rapl_buffer;
    // size_t rapl_buffer_size = sensor_init_perf_read_format(config_rapl->nb_counter, &rapl_buffer);
    // sensor_read(&perf_buffer, perf_buffer_size, &rapl_buffer, rapl_buffer_size);
    // report_store(strtok(NULL, " "), &perf_buffer, &rapl_buffer);

    sensor_terminate();
    if (sendto(socket_desc, "ACK", 3, 0,
                (struct sockaddr*)&client_addr, client_struct_length) < 0){
        printf("Can't send\n");
        return -1;
    }
    return 0;
}

int server_report(int socket_desc, struct config *config_perf, struct config *config_rapl, struct sockaddr_in client_addr, socklen_t client_struct_length) {
    report_write(strtok(NULL, " "), config_perf, config_rapl);
    if (sendto(socket_desc, "ACK", 3, 0,
                (struct sockaddr*)&client_addr, client_struct_length) < 0){
        printf("Can't send\n");
        return -1;
    }
    return 0;
}

int
server_stop(int socket_desc) {
    close(socket_desc);
    return 0;   
}