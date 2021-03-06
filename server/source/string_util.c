/*******************************************************************

Projekt Saturn V Main Computer

Plik: string_util.c

Przeznaczenie:
Zbi�r funkcji rozszerzaj�cych dzia�ania na ci�gach znak�w

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/string_util.h"
#include "include/shared.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int xdigit( char digit ){
	int val;

	if( '0' <= digit && digit <= '9' ) {
		val = digit -'0';
	} else if( 'a' <= digit && digit <= 'f' ) {
		val = digit -'a'+10;
	} else if( 'A' <= digit && digit <= 'F' ) {
		val = digit -'A'+10;
	} else {
		val = -1;
	}

	return val;
}

int xstr2str( char *buf, unsigned bufsize, const char *in ){
	unsigned inlen;
	unsigned i, j;

	if( !in ) {
		return -1;
	}

	inlen = strlen( in );
	if( inlen % 2 != 0 ) {
		return -2;
	}

	for( i = 0; i < inlen; i++ ) {
		if( xdigit( in[i] ) < 0 ) {
			return -3;
		}
	}

	if( !buf || bufsize < inlen / 2 + 1 ) {
		return -4;
	}

	for( i = 0, j = 0; i < inlen; i += 2, j++ ) {
		buf[ j ] = xdigit( in[ i ] ) * 16 + xdigit( in[ i+1 ]);
	}

	buf[ inlen/2 ] = '\0';

	return inlen / 2+1;
}

/*
strdelbslash( char *s1 )
@s1 - ci�g znak�w, z kt�rego b�d� usuni�te podw�jne backslashe
- usuwa z ci�gu znak�w s1 podw�jne backslashe*/
void strdelbslash( char *s1 ) {
	char *c1;

#ifdef _WIN32
	while( ( c1 = strstr(  s1, "\\\\"  ) ) ) {
#else
	while( ( c1 = strstr(  s1, "//"  ) ) ) {
#endif
		strcpy( c1 + 1, c1 + 2 );
	}
}

/*
strdelstr( const char *s1, const char s2 )
@s1 - ci�g znak�w, z kt�rego b�d� usuwane znaki s2
@s2 - ci�g znak�w, kt�ry ma zosta� usuni�ty z ci�gu s1
- funkcja usuwa ci�g znak�w s2 z ci�gu s1*/
void strdelstr( char *s1, const char* s2 )
{
	int t_len = strlen( s2 );

	/* Je�eli ci�g s2 nie wyst�puje w ci�gu s1 */
	if( !strstr( s1, s2 ) ) {
		return;
	}

	while( ( s1 = strstr( s1, s2 ) ) ) {
		memmove( s1, s1 + t_len, 1 + strlen( s1 + t_len ) );
	}
}

/*
strrepchar( char *s1, char c1, char c2 )
@s1 - ci�g znak�w, w kt�rym ma nast�pi� zmiana
@c1 - znak, kt�ry ma zosta� w ci�gu s1 zast�piony
@c2 - znak, kt�rym ma zosta� zast�piony znak c1 w ci�gu s1
- funkcja zamienia pojedyncze znaki bezpo�rednio w obiekcie, na kt�ry wskazuje s1. */
void strrepchar( char *s1, char c1, char c2 ) {
	int len = strlen( s1 );

	if( len <= 1 ) {
		return;
	}

	while( len > -1 ) {
		if( s1[ len ] == c1 ) {
			s1[ len ] = c2;
		}
		len--;
	}
}

/*
strpos( char *where_, char *what )
@where_ - ci�g znak�w, na kt�rym b�dzie przeprowadzona operacja wyszukiwania ci�gu what
@what - ci�g znak�w, kt�ry b�dzie wyszukany w ci�gu where_
- zwraca int, kt�ry wskazuje na pozycj� ci�gu what w ci�gu where_. Zwraca -1, gdy ci�gu nie znaleziono */
int strpos( const char *s1, const char *s2 ) {
	char *p = ( char* )strstr( s1, s2 );
	if( p ) {
		return p - s1;
	}
	return -1;
}

/*#include "external\md5\md5.h"
text_md5( const char *sz_Text )
@sz_Text - tre��, z kt�rej ma zosta� wyliczony MD5
- zwraca char *z wyliczonego MD5 zmiennej sz_Text
char *text_md5( const char *sz_Text )
{
md5_state_t state;
md5_byte_t digest[ 16[ ;
char hex_output[ 16*2 + 1[ ;
static char sz_md5Result[ 128[ ;
int di;

md5_init( &state );
md5_append( &state, ( const md5_byte_t * )sz_Text, strlen( sz_Text ) );
md5_finish( &state, digest );

for ( di = 0; di < 16; ++di )
sprintf( hex_output + di  *2, "%02x", digest[ di[  );

strncpy( sz_md5Result, hex_output, STD_BUFF_SIZE );

return ( ( char* )&sz_md5Result );
}*/
