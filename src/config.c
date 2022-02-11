#include "config.h"

struct config* init_config(int nb_counter, char* counter_names[]) {
    size_t configuration_size;
    struct config *config = NULL;
    configuration_size = offsetof(struct config, counters_names) + sizeof(struct counter_name[(int)nb_counter]);
    config = (struct config *) malloc(configuration_size);
    config->nb_counter = nb_counter;
    for (int i = 0 ; i < nb_counter ; i++) {
        config->counters_names[i].value = counter_names[i];
    }
    return config;
}