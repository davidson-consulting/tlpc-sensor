#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
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

#include "config.h"
#include "sensor.h"
#include "perf.h"
#include "report.h"
#include "map.h"

#ifndef SERVER_H
#define SERVER_H

int server_init();

int server_run(struct config *config_perf, struct config *config_rapl);

int server_start_sensor(
    int socket_desc, 
    struct config *config_perf, 
    struct config *config_rapl, 
    struct sockaddr_in client_addr, 
    socklen_t client_struct_length
);

int server_stop_sensor(
    int socket_desc, 
    struct config *config_perf, 
    struct config *config_rapl, 
    struct sockaddr_in client_addr, 
    socklen_t client_struct_length
);

int server_report(
    int socket_desc, 
    struct config *config_perf, 
    struct config *config_rapl, 
    struct sockaddr_in client_addr, 
    socklen_t client_struct_length
);

int server_stop(int socket_desc);

#endif