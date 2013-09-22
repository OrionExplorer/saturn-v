/*******************************************************************

Projekt Saturn V Main Computer

Plik: memory_manager.c

Przeznaczenie:
Funkcje u�atwiaj�ce zarz�dzanie pami�ci�

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/shared.h"
#include "include/memory_manager.h"
#include "include/log.h"
#include <stdio.h>
#include <stdlib.h>

/*
MEMORY_validate( char* ptr, int n )
@ptr - wska�nik do przydzielonej pami�ci �a�cucha
@n - numer porz�dkowy, s�u��cy do p�niejszej analizy problemu
- funkcja sprawdza, czy uda�o si� przydzieli� pami�� dla danego wska�nika, ko�czy program, je�eli wyst�pi� problem */
void MEMORY_validate( char *ptr, int n ) {
	/* B��d alokacji pami�ci, ptr = NULL */
	if( !ptr ) {
		LOG_print( "Error: malloc( %d ).\n", n );
		printf( "Error: malloc( %d ).\n", n );
		exit( EXIT_FAILURE );
	}
	/* Wszystko ok, kontynuuje dzia�anie programu */
}

