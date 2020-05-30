CC=gcc
#CFLAGS=-std=c99 -fexpensive-optimizations -Os -O3 -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Wmain -w -Wextra -Wall  -Os -O3 -O2 -O1 -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Wmain -pedantic-errors -pedantic -w -Wfatal-errors -Wextra -Wall -g3
CFLAGS=-std=c11 -fexpensive-optimizations -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Wmain -pedantic-errors -pedantic -w -Wfatal-errors -Wextra -Wall -g3 -O0
LIBS=-lm -lpthread
LNK=gcc

ifeq ($(OS),Windows_NT)
	LIBS += -lws2_32
	CC=mingw32-gcc
	LNK=mingw32-g++
endif

all: saturn-v

saturn-v: auto_pilot.o communication.o telemetry.o base64.o core.o time_util.o celestial_objects.o file_util.o sha1.o cJSON.o log.o spacecraft_components.o cWebSockets.o memory_manager.o saturn-v.o socket_io.o main_computer.o string_util.o physics.o
	$(LNK) auto_pilot.o communication.o telemetry.o base64.o core.o time_util.o celestial_objects.o file_util.o sha1.o cJSON.o log.o spacecraft_components.o cWebSockets.o memory_manager.o saturn-v.o socket_io.o main_computer.o string_util.o physics.o -o saturn-v $(LIBS)

saturn-v.o: source/saturn-v.c
	$(CC) -c source/saturn-v.c $(CFLAGS)

auto_pilot.o: source/auto_pilot.c
	$(CC) -c source/auto_pilot.c $(CFLAGS)

communication.o: source/communication.c
	$(CC) -c source/communication.c $(CFLAGS)

telemetry.o: source/telemetry.c
	$(CC) -c source/telemetry.c $(CFLAGS)

base64.o: source/base64.c
	$(CC) -c source/base64.c $(CFLAGS)

physics.o: source/physics.c
	$(CC) -c source/physics.c $(CFLAGS)

core.o: source/core.c
	$(CC) -c source/core.c $(CFLAGS)

time_util.o: source/time_util.c
	$(CC) -c source/time_util.c $(CFLAGS)

celestial_objects.o: source/celestial_objects.c
	$(CC) -c source/celestial_objects.c $(CFLAGS)

file_util.o: source/file_util.c
	$(CC) -c source/file_util.c $(CFLAGS)

sha1.o: source/sha1.c
	$(CC) -c source/sha1.c $(CFLAGS)

cJSON.o: source/cJSON.c
	$(CC) -c source/cJSON.c $(CFLAGS)

log.o: source/log.c
	$(CC) -c source/log.c $(CFLAGS)

spacecraft_components.o: source/spacecraft_components.c
	$(CC) -c source/spacecraft_components.c $(CFLAGS)

cWebSockets.o: source/cWebSockets.c
	$(CC) -c source/cWebSockets.c $(CFLAGS)

memory_manager.o: source/memory_manager.c
	$(CC) -c source/memory_manager.c $(CFLAGS)

socket_io.o: source/socket_io.c
	$(CC) -c source/socket_io.c $(CFLAGS)

main_computer.o: source/main_computer.c
	$(CC) -c source/main_computer.c $(CFLAGS)

string_util.o: source/string_util.c
	$(CC) -c source/string_util.c $(CFLAGS)

clean:
	rm *.o
