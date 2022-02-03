#include "TLPCSensor.h"

/*
 * Class:     TLPCSensor
 * Method:    start
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_fr_davidson_tlpc_sensor_TLPCSensor_start(JNIEnv *env, jobject o, jstring identifier)
{
    (void)env;
    (void)o;
    struct config *config_perf = init_config(_nb_perf_counter, _perf_counters_type);
    struct config *config_rapl = init_config(_nb_rapl_counter, _rapl_counters_type);
    const char *parsed_identifier = (*env)->GetStringUTFChars(env, identifier, 0);
    sensor_init(config_perf, config_rapl, 0, parsed_identifier);
    sensor_start(parsed_identifier);
}

/*
 * Class:     TLPCSensor
 * Method:    stop
 * Signature: (String)V
 */
JNIEXPORT void JNICALL Java_fr_davidson_tlpc_sensor_TLPCSensor_stop(JNIEnv *env, jobject o, jstring identifier)
{
    const char *parsed_identifier = (*env)->GetStringUTFChars(env, identifier, 0);
    sensor_stop(parsed_identifier);
    clock_t ending_time = clock();
    (void)o;
    struct config *config_perf = init_config(_nb_perf_counter, _perf_counters_type);
    struct config *config_rapl = init_config(_nb_rapl_counter, _rapl_counters_type);
    size_t perf_buffer_size = offsetof(struct perf_read_format, values) + sizeof(struct perf_counter_value[(int)config_perf->nb_counter]);
    struct perf_read_format *perf_buffer = (struct perf_read_format *)malloc(perf_buffer_size);
    size_t rapl_buffer_size = offsetof(struct perf_read_format, values) + sizeof(struct perf_counter_value[(int)config_rapl->nb_counter]);
    struct perf_read_format *rapl_buffer = (struct perf_read_format *)malloc(rapl_buffer_size);
    sensor_read(parsed_identifier, perf_buffer, perf_buffer_size, rapl_buffer, rapl_buffer_size);
    report_store(parsed_identifier, perf_buffer, rapl_buffer, map_get(parsed_identifier).starting_time, ending_time);
    sensor_terminate(parsed_identifier);
}

/*
 * Class:     TLPCSensor
 * Method:    report
 * Signature: (String)V
 */
JNIEXPORT void JNICALL Java_fr_davidson_tlpc_sensor_TLPCSensor_report(JNIEnv *env, jobject o, jstring pathname)
{
    (void)o;
    struct config *config_perf = init_config(_nb_perf_counter, _perf_counters_type);
    struct config *config_rapl = init_config(_nb_rapl_counter, _rapl_counters_type);
    report_write((*env)->GetStringUTFChars(env, pathname, 0), config_perf, config_rapl);
}
