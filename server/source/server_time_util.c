/*******************************************************************

Projekt Saturn V Main Computer

Plik: server_time_util.c

Przeznaczenie:
Zbiór funkcji przeznaczonych do zarz¹dzania dat¹ i czasem

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/server_shared.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
TIME_get_gmt_gmt()
- pobiera aktualny czas
- zwraca char *z aktualnym czasem w formacie GMT */
char* TIME_get_gmt( void ) {
	static char s[ TIME_BUFF_SIZE ];
	struct tm tim;
	time_t now;

	now = time( NULL );
	tim = *( gmtime( &now ) );
	strftime( s, TIME_BUFF_SIZE, DATETIME, &tim );

	return ( ( char* )&s );
}

long int TIME_get_epoch( void ) {
	time_t now;

	now = time( NULL );

	return ( long int )now;
}