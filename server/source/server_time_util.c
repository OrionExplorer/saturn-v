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
get_actual_time_gmt()
- pobiera aktualny czas
- zwraca char *z aktualnym czasem w formacie GMT */
char* get_actual_time( void ) {
	static char s[ TIME_BUFF_SIZE ];
	struct tm tim;
	time_t now;

	now = time( NULL );
	tim = *( localtime( &now ) );
	strftime( s, TIME_BUFF_SIZE, DATETIME, &tim );

	return ( ( char* )&s );
}

/*void c_sleep(int delay)
{
	double t1 = clock(), t2;

	fflush(stdout);

	do {
		t2 = clock();
	} while( ( (t2 - t1) / CLK_TCK ) < delay );
}*/

long int get_current_epoch( void ) {
	time_t now;

	now = time( NULL );

	return ( long int )now;
}

char* seconds_to_hms( const long double seconds ) {
	return "";
}
