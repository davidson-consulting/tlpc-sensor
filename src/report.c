#include "report.h"

int
report_write(struct config *config_perf, struct config *config_rapl, 
             struct perf_read_format *perf_buffer, struct perf_read_format *rapl_buffer) {
    FILE *fptr = fopen("./report.json","w");
    fprintf(fptr, "{\n");
     for (int i = 0 ; i < config_rapl->nb_counter; i++) {
        fprintf(fptr, "\t\"%s\":%ld,\n", config_rapl->counters_names[i].value, rapl_buffer->values[i].value);
    }   
    for (int i = 0 ; i < config_perf->nb_counter - 1; i++) {
        fprintf(fptr, "\t\"%s\":%ld,\n", config_perf->counters_names[i].value, perf_buffer->values[i].value);
    }   
    fprintf(fptr, "\t\"%s\":%ld\n", config_perf->counters_names[config_perf->nb_counter - 1].value, perf_buffer->values[config_perf->nb_counter - 1].value);
    fprintf(fptr, "}");
    fclose(fptr);
    return 0;
}