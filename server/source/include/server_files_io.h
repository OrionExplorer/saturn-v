/*******************************************************************

Projekt Saturn V Main Computer

Plik: server_files_io.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef SERVER_FILES_IO_H
#define SERVER_FILES_IO_H

#include "server_shared.h"

char*	get_app_path( void );
char*	file_get_ext( const char *filename );
char*	file_get_name( const char *full_filename );
void	file_extract_path( char *full_filename, char delim );
short	directory_exists( const char *path );
short	file_exists( const char *filename );

#endif
