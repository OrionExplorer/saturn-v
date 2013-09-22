/*******************************************************************

Projekt Saturn V Main Computer

Plik: saturn-v.c

Przeznaczenie:
Uruchomenie funkcji CORE_initialize()

Kompilacja na Win32: -lws2_32

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include <stdio.h>
#include <signal.h>
#include "include/server_core.h"
#include "include/server_socket_io.h"
#include "include/server_log.h"

void app_terminate( void ) {
	LOG_print( "Application terminate command received.\n" );
	printf( "Service is being closed..." );
	SOCKET_stop();
	LOG_print( "Service closed.\n" );
	LOG_save();
	printf( "ok.\n" );
	return;
}

int main( void ) {

	signal( SIGINT, ( sighandler )&app_terminate );
	#ifndef _WIN32
		signal( SIGPIPE, SIG_IGN );
	#endif
	signal( SIGABRT, ( sighandler )&app_terminate );
	signal( SIGTERM, ( sighandler )&app_terminate );

	printf( "%s\n", SERVER_NAME );
	CORE_initialize();
	return 0;
}
