#include "report.h"

int 
report_store(const char *identifier, struct perf_read_format *perf_buffer,  struct perf_read_format *rapl_buffer, clock_t ending_time) {
    const struct map_entry entry = map_get(identifier);
    report[cursor_report_entry].identifier = (char*) malloc(strlen(identifier) * sizeof(char));
    strcpy(report[cursor_report_entry].identifier, identifier);
    report[cursor_report_entry].perf_buffer = perf_buffer;
    report[cursor_report_entry].rapl_buffer = rapl_buffer;
    clock_t elapsedTime = ending_time - entry.starting_time;
    report[cursor_report_entry].elapsedTime = (unsigned long long)((((double)elapsedTime)/CLOCKS_PER_SEC)*1E9);
    cursor_report_entry++;
    map_remove(identifier);
    return 0;
}

int
report_write(const char* pathname, struct config *config_perf, struct config *config_rapl) {
    FILE *fptr = fopen(pathname == NULL ? "./report.json" : pathname, "w");
    fprintf(fptr, "{\n");
    for (int k = 0 ; k < cursor_report_entry - 1 ; k++) {
        struct report_entry entry = report[k];
        report_write_entry(entry, fptr, config_perf, config_rapl);
        fprintf(fptr, "\t},\n");
    }
    report_write_entry(report[cursor_report_entry - 1], fptr, config_perf, config_rapl);
    fprintf(fptr, "\t}\n}");
    fclose(fptr);
    cursor_report_entry = 0;
    return 0;
}

int report_write_entry(struct report_entry entry, FILE *fptr,
                       struct config *config_perf, struct config *config_rapl) {
    const char* identifier = entry.identifier;
    struct perf_read_format *perf_buffer = entry.perf_buffer;
    struct perf_read_format *rapl_buffer = entry.rapl_buffer;
    fprintf(fptr, "\t\"%s\": {\n", identifier);
    for (int i = 0 ; i < config_rapl->nb_counter; i++) {
        fprintf(fptr, "\t\t\"%s\":%ld,\n", config_rapl->counters_names[i].value, rapl_buffer->values[i].value);
    }   
    for (int i = 0 ; i < config_perf->nb_counter; i++) {
        fprintf(fptr, "\t\t\"%s\":%ld,\n", config_perf->counters_names[i].value, perf_buffer->values[i].value);
    }   
    fprintf(fptr, "\t\t\"%s\":%lld\n", "duration", entry.elapsedTime);
    return 0;
}