/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class TLPCSensor */

#include "sensor.h"
#include "config.h"
#include "report.h"
#include "map.h"

#ifndef _Included_TLPCSensor
#define _Included_TLPCSensor
#ifdef __cplusplus
extern "C" {
#endif

#define _nb_perf_counter 6

char* _perf_counters_type[_nb_perf_counter] = {
    "INSTRUCTIONS_RETIRED",
    "UNHALTED_REFERENCE_CYCLES",
    "BRANCH_INSTRUCTIONS_RETIRED",
    "MISPREDICTED_BRANCH_RETIRED",
    "LLC_REFERENCES",
    "LLC_MISSES"
};

#define _nb_rapl_counter 1

char* _rapl_counters_type[_nb_rapl_counter] = {
    "RAPL_ENERGY_PKG"
};

/*
 * Class:     TLPCSensor
 * Method:    start
 * Signature: ()V
 */
JNIEXPORT jintArray JNICALL Java_fr_davidson_tlpc_sensor_TLPCSensor_sensorStart
  (JNIEnv *, jobject);

/*
 * Class:     TLPCSensor
 * Method:    stop
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT jlongArray JNICALL Java_fr_davidson_tlpc_sensor_TLPCSensor_sensorStop
  (JNIEnv *, jobject, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
