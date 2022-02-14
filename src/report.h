#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "sensor.h"
#include "config.h"
#include "map.h"

#ifndef REPORT_H
#define REPORT_H

struct report_entry {
    char *identifier;
    struct perf_read_format *perf_buffer;
    struct perf_read_format *rapl_buffer;
    unsigned long long elapsedTime;
};

#define SIZE_OF_REPORT 1024

struct report_entry report[SIZE_OF_REPORT];

int cursor_report_entry;

int 
report_store(const char *identifier, struct perf_read_format *perf_buffer,  struct perf_read_format *rapl_buffer, clock_t ending_time);

int
report_write(const char* pathname, struct config *config_perf, struct config *config_rapl);

int 
report_write_entry(struct report_entry entry, FILE *fptr, struct config *config_perf, struct config *config_rapl);

#endif