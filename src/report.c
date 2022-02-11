#include "report.h"

int
report_write(const char* pathname, struct config *config_perf, struct config *config_rapl) {
    FILE *fptr = fopen(pathname == NULL ? "./report.json" : pathname, "w");
    fprintf(fptr, "{\n");
    for (int k = 0 ; k < size_keyset - 1 ; k++) {
        char *key = keyset[k];
        struct map_entry entry = map_get(key);
        report_write_entry(entry, fptr, config_perf, config_rapl);
        fprintf(fptr, "\t},\n");
        map_remove(key);
    }
    report_write_entry(map_get(keyset[size_keyset - 1]), fptr, config_perf, config_rapl);
    fprintf(fptr, "\t}\n}");
    fclose(fptr);
    size_keyset = 0;
    return 0;
}

int report_write_entry(struct map_entry entry, FILE *fptr,
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