cmake_minimum_required(VERSION 2.8)
project(tlpc-sensor LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wformat -Wformat-security -Werror")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -D_FORTIFY_SOURCE=2")

find_package(PkgConfig)
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

set(SENSOR_SOURCES
    src/main.c
    src/config.h src/config.c
    src/sensor.h src/sensor.c
    src/perf.h src/perf.c
    src/server.h src/server.c
    src/report.h src/report.c
    src/TLPCSensor.h src/TLPCSensor.c
    src/map.h src/map.c
)
set(SENSOR_LIBRARIES pfm)

add_executable(tlpc-sensor "${SENSOR_SOURCES}")
target_link_libraries(tlpc-sensor "${SENSOR_LIBRARIES}")

add_library(perf SHARED ${SENSOR_SOURCES})
target_link_libraries(perf "${SENSOR_LIBRARIES}")