#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sensor.h"
#include "config.h"

#ifndef REPORT_H
#define REPORT_H

struct entry {
    char *identifier;
    struct perf_read_format *perf_buffer;
    struct perf_read_format *rapl_buffer;
};

#define nb_entries 64

struct entry entries[nb_entries];

int current_nb;

int
report_init();

int
report_store(const char* identifier, struct perf_read_format *perf_buffer, struct perf_read_format *rapl_buffer);

int
report_write(const char* pathname, struct config *config_perf, struct config *config_rapl);

int 
report_write_entry(struct entry entry, FILE *fptr, struct config *config_perf, struct config *config_rapl);

#endif