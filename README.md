# Time-Lapse Performance Counters (TLPC) Sensor

TLPC-sensor aims at providing an API to measure various Performance Counters, such as the number of executed intructions and cycles, or energy consumption (with RAPL) in a time-lapse, designated by the call of the function `start()` and `stop()`.

## Install

You need to install [libpfm4](https://github.com/gfieni/libpfm4).

Then, you can run:
```sh
git clone http://github.com/davidson-consulting/tlpc-sensor.git
cd tlpc-sensor && mkdir build && cd build
cmake ..
make
./tlpc-sensor
```

You need sudo priviliege and run `echo -1 > /proc/sys/kernel/perf_event_paranoid`.

## Contributing

If you have any questions, remarks, suggestions or bug reports, please do not hesitate to open an issue.
TLPC-sensor is licensed under GNU GPL.
Contributions and pull requests are very welcome :smiley:.