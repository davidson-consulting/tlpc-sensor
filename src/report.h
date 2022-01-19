#include <stdio.h>

#include "sensor.h"
#include "config.h"

#ifndef REPORT_H
#define REPORT_H

struct record {
    struct perf_read_format perf_buffer;
    struct perf_read_format rapl_buffer;
};

struct entry {
    char* identifier;
    struct record values;
};

struct entry *entries;

#define nb_entries 64

int current_nb;

int
report_init();

int
report_store(const char* identifier, struct perf_read_format *perf_buffer, struct perf_read_format *rapl_buffer);

int
report_write(const char* pathname, struct config *config_perf, struct config *config_rapl);

int report_write_entry(struct entry entry, FILE *fptr, struct config *config_perf, struct config *config_rapl);

#endif