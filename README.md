# Time-Lapse Performance Counters (TLPC) Sensor

TLPC-sensor aims at providing an API to measure various Performance Counters, such as the number of executed intructions and cycles, or energy consumption (with RAPL) in a time-lapse, designed by the call of the function `start()` and `stop()`.

## Install

You need to install [libpfm4](https://github.com/gfieni/libpfm4)
Then, you can run:
```sh
git clone http://github.com/davidson-consulting/tlpc-sensor.git
cd tlpc-sensor && mkdir build && cd build
cmake ..
make
./tlpc-sensor
```

You need sudo priviliege and run `echo -1 > /proc/sys/kernel/perf_event_paranoid`.

## How to use

:construction:

## Running example

After installing and building `tlpc-sensor`, run it as follow:

```sh
$ ./tlpc-sensor
1 ./tlpc-sensor
Socket created successfully
```

Then, go to the `examples` folder and run:

```sh
make all
```

And run the c version of the client:
```sh
./client
Socket created successfully
start 467272 
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23 ...
```

where the number after `start` is the PID of the process.

On the server side, you should see something like:

```sh
Initialize INSTRUCTIONS_RETIRED (467272)
Initialize LLC_MISSES (467272)
Initialize CYCLES (467272)
Initialize RAPL_ENERGY_PKG (467272)
Initialize INSTRUCTIONS_RETIRED (467272)
Initialize LLC_MISSES (467272)
Initialize CYCLES (467272)
Initialize RAPL_ENERGY_PKG (467272)
```

Then, in `build/report_c.json`, you can observe the result:

```json
{
	"loop1": {
		"RAPL_ENERGY_PKG":15827730432,
		"INSTRUCTIONS_RETIRED":196027600,
		"LLC_MISSES":116295,
		"CYCLES":200173862
	},
	"loop2": {
		"RAPL_ENERGY_PKG":23072342016,
		"INSTRUCTIONS_RETIRED":195278548,
		"LLC_MISSES":94595,
		"CYCLES":201935383
	},
	"main": {
		"RAPL_ENERGY_PKG":62876811264,
		"INSTRUCTIONS_RETIRED":788001573,
		"LLC_MISSES":314443,
		"CYCLES":813863986
	}
}
```

The records `loop1` and `loop2` are successive loops of `10000` iterations and the record `main` wrap both loops.
Here, the consumption of `main` should be way more high than the two others, while the records of the loops should be similar.

You can play with the number of iterations of the loop to see the impact on the counters.

## Contributing

If you have any questions, remarks, suggestions or bug reports, please do not hesitate to open an issue.
TLPC-sensor is licensed under GNU GPL.
Contributions and pull requests are very welcome :smiley:.
