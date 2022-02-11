#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "sensor.h"
#include "config.h"
#include "map.h"

#ifndef REPORT_H
#define REPORT_H

int
report_write(const char* pathname, struct config *config_perf, struct config *config_rapl);

int 
report_write_entry(struct map_entry entry, FILE *fptr, struct config *config_perf, struct config *config_rapl);

#endif