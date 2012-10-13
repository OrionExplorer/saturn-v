/*******************************************************************

Projekt Voyager 7 Board Computer

Plik: server_files_io.c
Przeznaczenie:
Zbi�r funkcji przeznaczonych do obs�ugi plik�w i katalog�w

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/server_files_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif

/*
get_app_path( void )
- zwraca ci�g znak�w - folder startowy aplikacji */
char* get_app_path( void ) {
	static char buf[ MAX_PATH_LENGTH ];
	if( getcwd( buf, MAX_PATH_LENGTH ) ) {
		return strncat( buf, SLASH, MAX_PATH_LENGTH );
	} else {
		return "";
	}
}

/*
directory_exists( const char *path )
@path - �cie�ka, kt�ra ma zosta� sprawdzona
- funkcja pr�buje ustawi� katalog roboczy na �cie�k� podan� w zmiennej path
- zwraca int, gdzie 0 = �cie�ka nie istnieje, 1 = �cie�ka istnieje */
short directory_exists( const char *path ) {
	if( chdir( path ) == 0 ) {
		return 1;
	} else {
		return 0;
	}
}

/*
file_get_name( const char *full_filename )
@full_filename - pe�na nazwa pliku ( +�cie�ka )
- pobiera nazw� pliku z pe�nej nazwy ( +�cie�ka )
- zwraca char *z nazw� pliku */
char* file_get_name( const char *full_filename ) {
	char *filename;

	if( strstr( full_filename, SLASH ) ) {
		filename = ( char* )strrchr( full_filename, C_SLASH );
	} else {
		filename = ( char* )strrchr( full_filename, '/' );
	}

	/*Usuni�cie znaku "\" z pocz�tku filename */
	if( filename ) {
		return ++filename;
	}

	return "";
}

/*
file_get_ext( const char *filename )
@filename - nazwa pliku lub pe�na nazwa pliku ( +�cie�ka )
- pobiera rozszerzenie z podanej nazwy pliku
- zwraca char *z rozszerzeniem */
char* file_get_ext( const char *filename ) {
	char* file_ext = strrchr( file_get_name( filename ), '.' );

	if( !file_ext ) {
		return "";
	}
	return file_ext;
}

/*
file_exists( const char *filename )
@filename - nazwa szukanego pliku
- zwraca int, gdzie 1 = znaleziono plik. */
short file_exists( const char *filename ) {
	FILE *resource;	/* Uchwyt do pliku */

	if( ( resource = fopen( filename, READ_BINARY ) ) ) {
		/* Uda�o si� otworzy� plik = istnieje */
		fclose( resource );
		return 1;
	} else {
		/* Nie istnieje */
		return 0;
	}
}

/*
file_extract_path( const char *full_filename, char delim )
@full_filename - �cie�ka dost�pu do pliku, z kt�rej b�dzie pobrana sama �cie�ka
@delim - znak, od kt�rego ma zosta� "obci�ta" �cie�ka
- zwraca ci�g znak�w, kt�ry jest wyci�t� �cie�k� z pe�nej �cie�ki.
Rezultat nale�y p�niej zwolni� poprzez funkcj� free(). */
void file_extract_path(char *full_filename, char delim)
{
	int i = strlen(full_filename);

	if(i > 0) {
		while(--i) {
			if(full_filename[ i ] == delim) {
				full_filename[ i+1 ] = '\0';
			}
		}
	}
}
