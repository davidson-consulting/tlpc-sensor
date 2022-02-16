all: clean hw c java

hw:
	gcc -ggdb3 -O0 -std=c11 -Wall -Wextra -pedantic heavy_workload.c -o heavy_workload

c:
	gcc -ggdb3 -O0 -std=c11 -Wall -Wextra -pedantic client.c -o client

java:
	javac TLPCSensor.java

clean:
	rm -f client heavy_workload TLPCSensor.class