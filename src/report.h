#include <stdio.h>

#include "sensor.h"
#include "config.h"

int
report_write(struct config *config_perf, struct config *config_rapl, struct perf_read_format *perf_buffer, struct perf_read_format *rapl_buffer);