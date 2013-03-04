/*******************************************************************

Projekt Saturn V Main Computer

Plik: server_core.c

Przeznaczenie:
Konfiguracja aplikacji
Ustawienie nas�uchiwania socket�w

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/server_core.h"
#include "include/server_shared.h"
#include "include/server_files_io.h"
#include "include/server_socket_io.h"
#include "include/server_log.h"
#include "include/main_computer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* �cie�ka startowa aplikacji */
char	app_path[ MAX_PATH_LENGTH ];
char	app_auth[ SMALL_BUFF_SIZE ];
/*Pe�na nazwa pliku ( +�cie�ka dost�pu ) "log.txt" */
char	LOG_filename[ MAX_PATH_LENGTH ];

/*Przechowuje informacj� o typach adres�w IP: IPv4 lub IPv6 */
int		ip_proto_ver = -1;

static void		server_log_prepare( void );
static void		server_validate_paths( void );

/*
server_log_prepare()
- sprawdza, czy istnieje folder, w kt�rym przechowywany b�dzie log z dzia�ania aplikacji
- tworzy plik "log.txt" w katalogu, kt�rego nazwa jest aktualn� dat� */
static void server_log_prepare( void ) {
	char *tmp_path = malloc( MAX_PATH_LENGTH_CHAR+1 );

	/*Utworzenie �cie�ki do pliku "log.txt" */
	strncpy( tmp_path, app_path, MAX_PATH_LENGTH );
	strncat( tmp_path, LOGS_PATH, MAX_PATH_LENGTH );

	/*Weryfikacja istnienia �cie�ki do pliku "log.txt" */
	if( directory_exists( tmp_path ) == 0 ) {
		LOG_print( "Creating path: %s...\n", tmp_path );
		if( mkdir( tmp_path, 0777 ) != 0 ) {/*Utworzenie �cie�ki */
			LOG_print( "\t- Error creating path!\n" );
		}

		if( chdir( app_path ) != 0 ) {
			LOG_print( "Error: chdir().\n" );
			return;
		}
	}

	/*Dodanie do utworzonej �cie�ki nazwy pliku "log.txt" */
	strncpy( LOG_filename, tmp_path, MAX_PATH_LENGTH );
	strncat( LOG_filename, "log.txt", MAX_PATH_LENGTH );

	LOG_print( "=======NEW SERVER SESSION [%s]=======\n", get_actual_time() );

	free( tmp_path );
	tmp_path = NULL;
}

/*
server_validate_paths()
- sprawdza, czy istniej� wszystie foldery niezb�dne do poprawnego dzia�ania aplikacji */
static void server_validate_paths( void ) {
	char *tmp_path = malloc( MAX_PATH_LENGTH_CHAR+1 );
	int res = -1;

	/*Przypisanie �cie�ki, z kt�rej uruchamiana jest aplikacja */
	strncpy( tmp_path, app_path, MAX_PATH_LENGTH );

	/*Dopisanie �cie�ki do pliku "log.txt", bez nazwy pliku */
	strncat( tmp_path, LOGS_PATH, MAX_PATH_LENGTH );
	if( directory_exists( tmp_path ) == 0 ) {
		if( mkdir( tmp_path, 0777 ) != 0 ) {/*Utworzenie �cie�ki */
			LOG_print( "Error ( %d ) creating path!\n", res );
		}
		if( chdir( app_path ) != 0 ) {
			LOG_print( "Error: chdir().\n" );
			return;
		}
	}

	/*Patrz opis funkcji server_log_prepare() */
	server_log_prepare();

	free( tmp_path );
	tmp_path = NULL;
}

/*
CORE_load_configuration()
- je�li istnieje, wczytuje plik "server.cfg" i z niego pobiera konfiguracj� dla zmiennych:
+ ip_proto_ver
+ active_port */
short CORE_load_configuration( void ) {

	FILE *cfg_file;
	char buf[ STD_BUFF_SIZE ];
	char configuration_filename[ MAX_PATH_LENGTH ];
	int option;
	char value[ STD_BUFF_SIZE ];

	/* Reset zmiennych */
	ip_proto_ver = 4;
	active_port = DEFAULT_PORT;
	//memset( app_pass, '\0', STD_BUFF_SIZE );

	sprintf( configuration_filename, "%s%s", app_path, "config.dat" );
	cfg_file = fopen( configuration_filename, "rt" );

	if( cfg_file ) {
		while( fgets( buf, STD_BUFF_SIZE, cfg_file ) != NULL ) {
			option = -1;
			if( sscanf( buf, "%d %s", &option, value ) == 2 ) {
				switch( option ){
					case 0 : {
						strncpy( app_auth, value, SMALL_BUFF_SIZE );
						printf("Remote password is: %s.\n", app_auth );
					} break;
				}
			}
		}
		fclose( cfg_file );
	} else {
		printf("\nWARNING: Remote access to this computer is possible without authorization.\n\n");
		return 1;
	}

	if( strlen( app_auth ) == 0 ) {
		printf("\nWARNING: Remote access to this computer is possible without authorization.\n\n");
	}

	return 1;
}

/*
CORE_initialize()
- zast�puje funkcj� main()
- inicjuje zmienne globalne:
+ app_path
- uruchamia procedury konfiguracyjne:
+ CORE_load_configuration()
- uruchamia obs�ug� danych:
+ CORE_start() */
void CORE_initialize( void ) {
	/* Pobranie �cie�ki startowej aplikacji */
	strncpy( app_path, get_app_path(), MAX_PATH_LENGTH );

	/*Patrz opis funkcji server_validate_paths() */
	( void )server_validate_paths();

	LOG_print( "Service start path:\n%s\n", app_path );

	/*Patrz opisy poszczeg�lnych funkcji */
	if( CORE_load_configuration() == 0 ) {
		LOG_print( "Error: unable to load configuration.\n" );
		printf( "Error: unable to load configuration.\n" );
		LOG_save();
		exit( EXIT_FAILURE );
	}

	/* Zapisanie informacji o serwerze do log.txt */
	LOG_save();

	atexit( SOCKET_stop );

	/* Uruchomienie głównego komputera */
	MAIN_COMPUTER_init();
}

