/*******************************************************************

Projekt Saturn V Main Computer

Plik: server_time_util.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef SERVER_TIME_UTIL_H
#define SERVER_TIME_UTIL_H

char*			get_actual_time( void );
void			c_sleep( int delay );
long int		get_current_epoch( void );
char*			seconds_to_hms( const long double seconds );

#endif
