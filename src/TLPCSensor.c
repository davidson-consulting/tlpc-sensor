#include "TLPCSensor.h"

JNIEXPORT jintArray JNICALL Java_fr_davidson_tlpc_sensor_TLPCSensor_sensorStart(JNIEnv *env, jobject o)
{
    (void)o;
    struct config *config_perf = init_config(_nb_perf_counter, _perf_counters_type);
    struct config *config_rapl = init_config(_nb_rapl_counter, _rapl_counters_type);
    int perf_group_leader_fd = -1, rapl_group_leader_fd = -1;
    sensor_init(config_perf, config_rapl, 0, &perf_group_leader_fd, &rapl_group_leader_fd);
    free(config_rapl);
    free(config_perf);
    jint fds_int[2];
    fds_int[0] = (int) perf_group_leader_fd;
    fds_int[1] = (int) rapl_group_leader_fd;
    jintArray fds_array = (*env)->NewIntArray(env, 2);
    (*env)->SetIntArrayRegion(env, fds_array, 0, 2, fds_int);
    sensor_start(rapl_group_leader_fd, perf_group_leader_fd);
    return fds_array;
}

JNIEXPORT jlongArray JNICALL Java_fr_davidson_tlpc_sensor_TLPCSensor_sensorStop(JNIEnv *env, jobject o, int perf_group_leader_fd, int rapl_group_leader_fd)
{
    sensor_stop(perf_group_leader_fd, rapl_group_leader_fd);
    (void)o;
    struct config *config_perf = init_config(_nb_perf_counter, _perf_counters_type);
    struct config *config_rapl = init_config(_nb_rapl_counter, _rapl_counters_type);
    size_t perf_buffer_size = offsetof(struct perf_read_format, values) + sizeof(struct perf_counter_value[(int)config_perf->nb_counter]);
    struct perf_read_format *perf_buffer = (struct perf_read_format *)malloc(perf_buffer_size);
    size_t rapl_buffer_size = offsetof(struct perf_read_format, values) + sizeof(struct perf_counter_value[(int)config_rapl->nb_counter]);
    struct perf_read_format *rapl_buffer = (struct perf_read_format *)malloc(rapl_buffer_size);
    sensor_read(perf_group_leader_fd, rapl_group_leader_fd, perf_buffer, perf_buffer_size, rapl_buffer, rapl_buffer_size);
    sensor_terminate(perf_group_leader_fd, rapl_group_leader_fd);
    const unsigned int nb_configs = _nb_perf_counter + _nb_rapl_counter + 1;
    jlong args[nb_configs];
    for (int i = 0; i < _nb_perf_counter ; i++) {
        args[i] = (long) perf_buffer->values[i].value;
    }
    for (int i = 0; i < _nb_rapl_counter ; i++) {
        args[_nb_perf_counter + i] = (long) rapl_buffer->values[i].value;
    }
    args[nb_configs - 1] = perf_buffer->time_running;
    jlongArray collectedValues = (*env)->NewLongArray(env, nb_configs);
    (*env)->SetLongArrayRegion(env, collectedValues, 0, nb_configs, args);
    free(config_rapl);
    free(config_perf);
    free(rapl_buffer);
    free(perf_buffer);
    return collectedValues;
}

JNIEXPORT jlongArray JNICALL Java_fr_davidson_tlpc_sensor_TLPCSensor_sensorRead(JNIEnv *env, jobject o, int perf_group_leader_fd, int rapl_group_leader_fd)
{
    (void)o;
    struct config *config_perf = init_config(_nb_perf_counter, _perf_counters_type);
    struct config *config_rapl = init_config(_nb_rapl_counter, _rapl_counters_type);
    size_t perf_buffer_size = offsetof(struct perf_read_format, values) + sizeof(struct perf_counter_value[(int)config_perf->nb_counter]);
    struct perf_read_format *perf_buffer = (struct perf_read_format *)malloc(perf_buffer_size);
    size_t rapl_buffer_size = offsetof(struct perf_read_format, values) + sizeof(struct perf_counter_value[(int)config_rapl->nb_counter]);
    struct perf_read_format *rapl_buffer = (struct perf_read_format *)malloc(rapl_buffer_size);
    sensor_read(perf_group_leader_fd, rapl_group_leader_fd, perf_buffer, perf_buffer_size, rapl_buffer, rapl_buffer_size);
    const unsigned int nb_configs = _nb_perf_counter + _nb_rapl_counter + 1;
    jlong args[nb_configs];
    for (int i = 0; i < _nb_perf_counter ; i++) {
        args[i] = (long) perf_buffer->values[i].value;
    }
    for (int i = 0; i < _nb_rapl_counter ; i++) {
        args[_nb_perf_counter + i] = (long) rapl_buffer->values[i].value;
    }
    args[nb_configs - 1] = perf_buffer->time_running;
    jlongArray collectedValues = (*env)->NewLongArray(env, nb_configs);
    (*env)->SetLongArrayRegion(env, collectedValues, 0, nb_configs, args);
    free(config_rapl);
    free(config_perf);
    free(rapl_buffer);
    free(perf_buffer);
    return collectedValues;
}