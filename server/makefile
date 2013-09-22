CC=gcc 
CFLAGS=-std=c99 -g -pedantic -Wall
LIBS =-lm -lpthread

ifeq ($(OS),Windows_NT)
	LIBS += -lws2_32
endif

all: saturn-v

saturn-v: auto_pilot.o communication.o telemetry.o base64.o core.o time_util.o celestial_objects.o file_util.o sha1.o cJSON.o log.o spacecraft_components.o cWebSockets.o memory_manager.o saturn-v.o socket_io.o main_computer.o string_util.o physics.o
	gcc auto_pilot.o communication.o telemetry.o base64.o core.o time_util.o celestial_objects.o file_util.o sha1.o cJSON.o log.o spacecraft_components.o cWebSockets.o memory_manager.o saturn-v.o socket_io.o main_computer.o string_util.o physics.o -o saturn-v $(LIBS)

saturn-v.o: source/saturn-v.c
	gcc -c source/saturn-v.c $(CFLAGS)

auto_pilot.o: source/auto_pilot.c
	gcc -c source/auto_pilot.c $(CFLAGS)

communication.o: source/communication.c
	gcc -c source/communication.c $(CFLAGS)

telemetry.o: source/telemetry.c
	gcc -c source/telemetry.c $(CFLAGS)

base64.o: source/base64.c
	gcc -c source/base64.c $(CFLAGS)

physics.o: source/physics.c
	gcc -c source/physics.c $(CFLAGS)

core.o: source/core.c
	gcc -c source/core.c $(CFLAGS)

time_util.o: source/time_util.c
	gcc -c source/time_util.c $(CFLAGS)

celestial_objects.o: source/celestial_objects.c
	gcc -c source/celestial_objects.c $(CFLAGS)

file_util.o: source/file_util.c
	gcc -c source/file_util.c $(CFLAGS)

sha1.o: source/sha1.c
	gcc -c source/sha1.c $(CFLAGS)

cJSON.o: source/cJSON.c
	gcc -c source/cJSON.c $(CFLAGS)

log.o: source/log.c
	gcc -c source/log.c $(CFLAGS)

spacecraft_components.o: source/spacecraft_components.c
	gcc -c source/spacecraft_components.c $(CFLAGS)

cWebSockets.o: source/cWebSockets.c
	gcc -c source/cWebSockets.c $(CFLAGS)

memory_manager.o: source/memory_manager.c
	gcc -c source/memory_manager.c $(CFLAGS)

socket_io.o: source/socket_io.c
	gcc -c source/socket_io.c $(CFLAGS)

main_computer.o: source/main_computer.c
	gcc -c source/main_computer.c $(CFLAGS)

string_util.o: source/string_util.c
	gcc -c source/string_util.c $(CFLAGS)

clean:
	rm *.o
