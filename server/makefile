CC=gcc 
CFLAGS=-std=c99 -g -pedantic -Wall
LIBS =-lm -lpthread

ifeq ($(OS),Windows_NT)
	LIBS += -lws2_32
endif

all: saturn-v

saturn-v: auto_pilot.o server_communication.o server_telemetry.o base64.o server_core.o server_time_util.o celestial_objects.o server_files_io.o sha1.o cJSON.o server_log.o spacecraft_components.o cWebSockets.o server_mem_manager.o saturn-v.o server_socket_io.o main_computer.o server_strings_util.o physics.o
	gcc auto_pilot.o server_communication.o server_telemetry.o base64.o server_core.o server_time_util.o celestial_objects.o server_files_io.o sha1.o cJSON.o server_log.o spacecraft_components.o cWebSockets.o server_mem_manager.o saturn-v.o server_socket_io.o main_computer.o server_strings_util.o physics.o -o saturn-v $(LIBS)

saturn-v.o: source/saturn-v.c
	gcc -c source/saturn-v.c $(CFLAGS)

auto_pilot.o: source/auto_pilot.c
	gcc -c source/auto_pilot.c $(CFLAGS)

server_communication.o: source/server_communication.c
	gcc -c source/server_communication.c $(CFLAGS)

server_telemetry.o: source/server_telemetry.c
	gcc -c source/server_telemetry.c $(CFLAGS)

base64.o: source/base64.c
	gcc -c source/base64.c $(CFLAGS)

physics.o: source/physics.c
	gcc -c source/physics.c $(CFLAGS)

server_core.o: source/server_core.c
	gcc -c source/server_core.c $(CFLAGS)

server_time_util.o: source/server_time_util.c
	gcc -c source/server_time_util.c $(CFLAGS)

celestial_objects.o: source/celestial_objects.c
	gcc -c source/celestial_objects.c $(CFLAGS)

server_files_io.o: source/server_files_io.c
	gcc -c source/server_files_io.c $(CFLAGS)

sha1.o: source/sha1.c
	gcc -c source/sha1.c $(CFLAGS)

cJSON.o: source/cJSON.c
	gcc -c source/cJSON.c $(CFLAGS)

server_log.o: source/server_log.c
	gcc -c source/server_log.c $(CFLAGS)

spacecraft_components.o: source/spacecraft_components.c
	gcc -c source/spacecraft_components.c $(CFLAGS)

cWebSockets.o: source/cWebSockets.c
	gcc -c source/cWebSockets.c $(CFLAGS)

server_mem_manager.o: source/server_mem_manager.c
	gcc -c source/server_mem_manager.c $(CFLAGS)

server_socket_io.o: source/server_socket_io.c
	gcc -c source/server_socket_io.c $(CFLAGS)

main_computer.o: source/main_computer.c
	gcc -c source/main_computer.c $(CFLAGS)

server_strings_util.o: source/server_strings_util.c
	gcc -c source/server_strings_util.c $(CFLAGS)

clean:
	rm *.o
