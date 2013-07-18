CC=gcc 
CFLAGS=--std=c99 -g -pedantic -Wall -lm
LIBS =-lm -lpthread

all: saturn-v

saturn-v: auto_pilot.o server_communication.o server_telemetry.o base64.o server_core.o server_time_util.o celestial_objects.o server_files_io.o sha1.o cJSON.o server_log.o spacecraft_components.o cWebSockets.o server_mem_manager.o voyager7main.o server_socket_io.o main_computer.o server_strings_util.o
	gcc auto_pilot.o server_communication.o server_telemetry.o base64.o server_core.o server_time_util.o celestial_objects.o server_files_io.o sha1.o cJSON.o server_log.o spacecraft_components.o cWebSockets.o server_mem_manager.o voyager7main.o server_socket_io.o main_computer.o server_strings_util.o -o saturn-v $(LIBS)

voyager7main.o: source/voyager7main.c
	gcc -c source/voyager7main.c $(LIBS)

auto_pilot.o: source/auto_pilot.c
	gcc -c source/auto_pilot.c -lm $(LIBS)

server_communication.o: source/server_communication.c
	gcc -c source/server_communication.c $(LIBS)

server_telemetry.o: source/server_telemetry.c
	gcc -c source/server_telemetry.c $(LIBS)

base64.o: source/base64.c
	gcc -c source/base64.c $(LIBS)

server_core.o: source/server_core.c
	gcc -c source/server_core.c $(LIBS)

server_time_util.o: source/server_time_util.c
	gcc -c source/server_time_util.c $(LIBS)

celestial_objects.o: source/celestial_objects.c
	gcc -c source/celestial_objects.c $(LIBS)

server_files_io.o: source/server_files_io.c
	gcc -c source/server_files_io.c $(LIBS)

sha1.o: source/sha1.c
	gcc -c source/sha1.c $(LIBS)

cJSON.o: source/cJSON.c
	gcc -c source/cJSON.c $(LIBS)

server_log.o: source/server_log.c
	gcc -c source/server_log.c $(LIBS)

spacecraft_components.o: source/spacecraft_components.c
	gcc -c source/spacecraft_components.c $(LIBS)

cWebSockets.o: source/cWebSockets.c
	gcc -c source/cWebSockets.c $(LIBS)

server_mem_manager.o: source/server_mem_manager.c
	gcc -c source/server_mem_manager.c $(LIBS)

server_socket_io.o: source/server_socket_io.c
	gcc -c source/server_socket_io.c $(LIBS)

main_computer.o: source/main_computer.c
	gcc -c source/main_computer.c -lm $(LIBS)

server_strings_util.o: source/server_strings_util.c
	gcc -c source/server_strings_util.c $(LIBS)

clean:
	rm *.o