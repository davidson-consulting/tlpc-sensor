#include "report.h"

int
report_init() {
    entries = (struct entry *) malloc(nb_entries * sizeof(struct entry));
    current_nb = 0;
    return 0;
}

int
report_store(const char* identifier, struct perf_read_format *perf_buffer, struct perf_read_format *rapl_buffer) {
    entries[current_nb].identifier = identifier;
    entries[current_nb].values.perf_buffer = *perf_buffer;
    entries[current_nb].values.rapl_buffer = *rapl_buffer;
    current_nb++;
    return 0;
}

int
report_write(const char* pathname, struct config *config_perf, struct config *config_rapl) {
    FILE *fptr = fopen(pathname == NULL ? "./report.json" : pathname, "w");
    fprintf(fptr, "{\n");
    for (int e = 0 ; e < current_nb - 1 ; e++) {
        report_write_entry(entries[e], fptr, config_perf, config_rapl);
        fprintf(fptr, "\t},\n");
    }
    report_write_entry(entries[current_nb - 1], fptr, config_perf, config_rapl);
    fprintf(fptr, "\t}\n}");
    fclose(fptr);
    return 0;
}

int report_write_entry(struct entry entry, FILE *fptr,
                       struct config *config_perf, struct config *config_rapl) {
    const char* identifier = entry.identifier;
    struct perf_read_format perf_buffer = entry.values.perf_buffer;
    struct perf_read_format rapl_buffer = entry.values.perf_buffer;
    fprintf(fptr, "\t%s: {\n", identifier);
    for (int i = 0 ; i < config_rapl->nb_counter; i++) {
        fprintf(fptr, "\t\t\"%s\":%ld,\n", config_rapl->counters_names[i].value, rapl_buffer.values[i].value);
    }   
    for (int i = 0 ; i < config_perf->nb_counter - 1; i++) {
        fprintf(fptr, "\t\t\"%s\":%ld,\n", config_perf->counters_names[i].value, perf_buffer.values[i].value);
    }   
    fprintf(fptr, "\t\t\"%s\":%ld\n", config_perf->counters_names[config_perf->nb_counter - 1].value, perf_buffer.values[config_perf->nb_counter - 1].value);
    return 0;
}