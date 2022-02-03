#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "sensor.h"
#include "config.h"
#include "map.h"

#ifndef REPORT_H
#define REPORT_H

struct perf_data_by_id {
    char *identifier;
    struct perf_read_format *perf_buffer;
    struct perf_read_format *rapl_buffer;
    unsigned long long elapsedTime;
};

#define nb_entries 64

struct perf_data_by_id entries[nb_entries];

int current_nb;

int
report_init();

int
report_store(
    const char* identifier, 
    struct perf_read_format *perf_buffer, 
    struct perf_read_format *rapl_buffer, 
    clock_t starting_time, 
    clock_t ending_time
);

int
report_write(const char* pathname, struct config *config_perf, struct config *config_rapl);

int 
report_write_entry(struct perf_data_by_id perf_data_by_id, FILE *fptr, struct config *config_perf, struct config *config_rapl);

#endif