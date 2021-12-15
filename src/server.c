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

#include "server.h"
#include "sensor.h"
#include "perf.h"

int socket_desc;

int
server_init() {
    struct sockaddr_in server_addr;
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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
    return 0;   
}

int 
server_run(struct config *config) {

    struct sockaddr_in client_addr;
    char server_message[2000], client_message[2000];
    socklen_t client_struct_length = sizeof(client_addr);
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    printf("Listening for incoming messages...\n\n");
    while (true) {
        if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
            (struct sockaddr*)&client_addr, &client_struct_length) < 0){
            printf("Couldn't receive\n");
            return -1;
        }
        printf("Received message from IP: %s and port: %i\n",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        printf("%s\n", client_message);
        char *cursor = strtok(client_message, " ");
        if (strcmp("start", cursor) == 0) {
            char *pid_str = strtok(NULL, " ");
            pid_t pid = atoi(pid_str);
            printf("start sensor for %d\n", pid);
            memset(client_message, '\0', sizeof(client_message));
            sensor_init(config->perf_counters_names, config->nb_counter, pid);
            sensor_start();
             if (sendto(socket_desc, "ACK", 3, 0,
                (struct sockaddr*)&client_addr, client_struct_length) < 0){
                printf("Can't send\n");
                return -1;
            }
            continue;
        } 
        if (strcmp("stop", cursor) == 0) {
            printf("stop sensor\n");
            memset(client_message, '\0', sizeof(client_message));
            sensor_stop();

            size_t perf_read_buffer_size;
            struct perf_read_format *perf_read_buffer = NULL;
            perf_read_buffer_size = offsetof(struct perf_read_format, values) + sizeof(struct perf_counter_value[(int)config->nb_counter]);
            perf_read_buffer = (struct perf_read_format *) malloc(perf_read_buffer_size);

            sensor_read(perf_read_buffer, perf_read_buffer_size);

            printf("nb counter %ld\n", perf_read_buffer->nr);
            printf("enable time %ld\n", perf_read_buffer->time_enabled);
            printf("running time %ld\n", perf_read_buffer->time_running);
            for (int i = 0 ; i < config->nb_counter; i++) {
                printf("%s %ld\n", config->perf_counters_names[i].value, perf_read_buffer->values[i].value);
            }
            sensor_terminate();
            if (sendto(socket_desc, "ACK", 3, 0,
                (struct sockaddr*)&client_addr, client_struct_length) < 0){
                printf("Can't send\n");
                return -1;
            }
            continue;
        }
        printf("%s is not a valid command\n", client_message);
    }
    return 0;
}

int
server_stop() {
    close(socket_desc);
    return 0;   
}