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
#include <errno.h>

#ifndef PERF_H
#define PERF_H

int
perf_initialize();

int
perf_init_event_attr(char* event_name, struct perf_event_attr* attr);

int
perf_open_event(struct perf_event_attr* attr, int pid, int cpu, int group_fd, int flags);

int
perf_terminate();

#endif