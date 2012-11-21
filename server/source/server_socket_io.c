/*******************************************************************

Projekt voyager7computer

Plik: server_socket_io.c

Przeznaczenie:
Inicjalizacja socketów
Konfiguracja socketów
Odbieranie danych z sieci i przekazanie do interpretacji

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/server_socket_io.h"
#include "include/server_shared.h"
#include "include/server_log.h"
#include "include/server_files_io.h"
#include "include/server_strings_util.h"
#include "include/cWebSockets.h"
#include "include/main_computer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>

/*Sockety */
#ifdef _WIN32
/*Inicjalizacja WinSock */
WSADATA				wsk;
SOCKET				socket_server;
#else
int					socket_server;
#endif

int					addr_size;
int					active_port;
struct sockaddr_in	server_address;
COMMUNICATION_SESSION		communication_session_;
int					i_sac;
fd_set				master;
fd_set				read_fds;
int					fdmax;
int					newfd;
struct hostent		*host;
struct in_addr		addr;

int					http_conn_count = 0;

static void		SOCKET_initialization( void );
static void		SOCKET_prepare( void );
static void		SOCKET_process( int socket_fd );
void			SOCKET_stop( void );

/*
SOCKET_initialization( void )
Inicjalizacja socketa */
static void SOCKET_initialization( void ) {
	LOG_print( "Waiting for socket server initialization..." );
	printf("Initializing COMMUNICATION INTERFACE...");

#ifdef _WIN32
	/*Inicjalizacja WinSock */
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsk ) != 0 ) {
		LOG_print( "\nError creating Winsock.\n" );
		printf( "Error creating Winsock.\n" );
		system( "pause" );
		exit( EXIT_FAILURE );
	}
#endif
	/*Utworzenie socketa nas³uchuj¹cego */
	socket_server = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( socket_server == SOCKET_ERROR ) {
		LOG_print( "Error creating socket.\n" );
		printf( "Error creating socket.\n" );
		SOCKET_stop();
		exit( EXIT_FAILURE );
	}

	if( active_port < 0 || active_port > 65535 ) {
		active_port = DEFAULT_PORT;
	}

	memset( &server_address, 0, sizeof( server_address ) );
	server_address.sin_addr.s_addr = htonl( INADDR_ANY );
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons( ( u_short )active_port );
}

/*
SOCKET_prepare( void )
Nas³uchiwanie w celu odbioru danych od klienta */
static void SOCKET_prepare( void ) {
	unsigned long b = 0;
	int i = 1;
	int wsa_result = 0;

	FD_ZERO( &master );
	FD_ZERO( &read_fds );

#ifndef _WIN32
	setuid( 0 );
	setgid( 0 );
#endif

	if ( bind( socket_server, ( struct sockaddr* )&server_address, sizeof( server_address ) ) == SOCKET_ERROR ) {
		wsa_result = WSAGetLastError();
		LOG_print( "bind() error: %d.\n", wsa_result );
		printf( "bind() error: %d.\n", wsa_result );
		SOCKET_stop();
		exit( EXIT_FAILURE );
	}

	if( setsockopt( socket_server, IPPROTO_TCP, TCP_NODELAY, ( char * )&i, sizeof( int ) ) == SOCKET_ERROR ) {
		wsa_result = WSAGetLastError();
		LOG_print( "setsockopt( TCP_NODELAY ) error: %d.\n", wsa_result );
		printf( "setsockopt( TCP_NODELAY ) error: %d.\n", wsa_result );
	}

	if( setsockopt( socket_server, SOL_SOCKET, SO_REUSEADDR, ( char * )&i, sizeof( int ) ) == SOCKET_ERROR ) {
		wsa_result = WSAGetLastError();
		LOG_print( "setsockopt( SO_REUSEADDR ) error: %d.\n", wsa_result );
		printf( "setsockopt( SO_REUSEADDR ) error: %d.\n", wsa_result );
	}

	/* Ustawienie na non-blocking socket */
	if( fcntl( socket_server, F_SETFL, &b ) == SOCKET_ERROR ) {
		wsa_result = WSAGetLastError();
		LOG_print( "ioctlsocket(): error: %d.\n", wsa_result );
		printf( "ioctlsocket(): error: %d.\n", wsa_result );
		SOCKET_stop();
		exit( EXIT_FAILURE );
	}

	/* Rozpoczêcie nas³uchiwania */
	if( listen( socket_server, MAX_CLIENTS ) == SOCKET_ERROR ) {
		wsa_result = WSAGetLastError();
		LOG_print( "listen() error: %d.\n", wsa_result );
		printf( "listen() error: %d.\n", wsa_result );
		SOCKET_stop();
		exit( EXIT_FAILURE );
	}

	LOG_print( "ok.\nSocket server is running:\n" );
	LOG_print( "- Port: %d.\n", active_port );
	LOG_print( "Communication Interface ready...\n" );
	printf( "ok.\n" );
	/* Teraz czekamy na po³¹czenia i dane */
}

/*
SOCKET_run( void )
Funkcja zarz¹dzaj¹ca po³¹czeniami przychodz¹cymi do gniazda. */
void SOCKET_run( void ) {
	register int i = 0;
	struct timeval tv;
	pthread_t sthread;

	FD_SET( socket_server, &master );

	tv.tv_sec = 1;
	tv.tv_usec = 500000;

	fdmax = socket_server;

	/* Uruchomienie w¹tku wysy³aj¹cego dane do pod³¹czonych klientów */
	pthread_create( &sthread, NULL, TELEMETRY_send_live_data, "voyager7computer" );

	for( ;"elvis presley lives"; ) {
		read_fds = master;
		if( select( fdmax+1, &read_fds, NULL, NULL, &tv ) == -1 ) {
			SOCKET_stop();
			exit( EXIT_FAILURE );
		}

		i = fdmax+1;
		while( --i ) {
			if( FD_ISSET( i, &read_fds ) ) { /* Coœ siê dzieje na sockecie... */
				if( i == socket_server ) {
					/* Pod³¹czy³ siê nowy klient */
					SOCKET_modify_clients_count( 1 );

					communication_session_.data_length = sizeof( struct sockaddr );
					newfd = accept( socket_server, ( struct sockaddr* )&communication_session_.address, &communication_session_.data_length );
					communication_session_.socket_descriptor = newfd;

					if( newfd == -1 ) {
						LOG_print( "Socket closed.\n" );
					} else {
						SOCKET_register_client( newfd );
						FD_SET( newfd, &master );
						if( newfd > fdmax ) {
							fdmax = newfd;
						}
					}
				} else {
					/* Pod³¹czony klient przes³a³ dane */
					SOCKET_process( i );
				}
			} /*nowe po³¹czenie */
		} /*pêtla deskryptorów while( --i )*/
	} /*for( ;; ) */
}

void REQUEST_parse_command( CONNECTED_CLIENT *client, const char *data ) {
	vDEVICE rocket_device;
	vCOMMAND rocket_command;
	int rocket_value;
	INTERPRETER_RESULT *result;

	cJSON *json = cJSON_Parse( data );
	cJSON *command_json;
	cJSON *command_type_json;
	cJSON *response_mode_json;

	char *command = NULL;// = ( char * )calloc( SMALL_BUFF_SIZE, sizeof( char ) );
	char *command_type = NULL;//( char * )calloc( SMALL_BUFF_SIZE, sizeof( char ) );
	char *response_mode = NULL;//( char * )calloc( SMALL_BUFF_SIZE, sizeof( char ) );
	char *main_computer_response_str = NULL;
	short main_computer_response_success = 0;

	LOG_print( "[%s] [%d] Received command: \"%s\".\n", get_actual_time_gmt(), client->socket_descriptor, data );

	if( json == NULL ) {
		SOCKET_send( &communication_session_, client, INVALID_JSON, -1 );
		return;
	}

	command_json = cJSON_GetObjectItem( json, "command" );
	command_type_json = cJSON_GetObjectItem( json, "command_type" );
	response_mode_json = cJSON_GetObjectItem( json, "response_mode" );

	if( command_json ) {
		command = command_json->valuestring;
	}

	if( command_type_json ) {
		command_type = command_type_json->valuestring;
	}

	if( response_mode_json ) {
		response_mode = response_mode_json->valuestring;
	}

	if( client->authorized == 0 ) {
		if( strlen( app_auth ) == 0 ) {
			client->authorized = 1;
			SOCKET_send( &communication_session_, client, LOGIN_SUCCESS, -1 );
		} else {
			if( command_type && command ) {
				if( strncmp( "authorization", command_type, STD_BUFF_SIZE ) == 0 ) {
					if( strncmp( app_auth, command, STD_BUFF_SIZE ) == 0 ) {
						client->authorized = 1;
						SOCKET_send( &communication_session_, client, LOGIN_SUCCESS, -1 );
					} else {
						SOCKET_send( &communication_session_, client, LOGIN_STR, -1 );
					}
				} else {
					SOCKET_send( &communication_session_, client, LOGIN_STR, -1 );
				}
			} else {
				SOCKET_send( &communication_session_, client, LOGIN_STR, -1 );
			}
		}
	} else {
		if( command && command_type ) {
			if(strncmp( "computer", command_type, STD_BUFF_SIZE ) == 0 ) {
				if( sscanf( command, "%d %d %d", ( int * )&rocket_device, ( int * )&rocket_command, &rocket_value ) == 3 ) {
					main_computer_response_str = ( char * )calloc( STD_BUFF_SIZE, sizeof( char ) );
					result = EXEC_COMMAND( rocket_device, rocket_command, rocket_value );

					main_computer_response_success = result->success;
					snprintf( main_computer_response_str, STD_BUFF_SIZE, MAIN_COMPUTER_RESPONSE_TEMPLATE, ( main_computer_response_success == 1 ? "true" : "false" ), result->message );
					SOCKET_send( &communication_session_, client, main_computer_response_str, -1 );

					free( main_computer_response_str );
					main_computer_response_str = NULL;

				} else {
					SOCKET_send( &communication_session_, client, "ILLEGAL COMMAND", -1 );
				}
			} else if( strncmp( "data", command_type, STD_BUFF_SIZE ) == 0 ) {
				if( strncmp( "status", command, STD_BUFF_SIZE ) == 0 ) {
					TELEMETRY_send_ondemand_data( client );
				}
			}
		}

		if( response_mode ) {
			if(strncmp( "live", response_mode, STD_BUFF_SIZE ) == 0 ) {
				client->binded = 1;
			} else if ( strncmp( "on-demand", response_mode, STD_BUFF_SIZE ) == 0 ) {
				client->binded = 0;
			}
		}
	}

	if( json ) {
		//cJSON_Delete( json );
	}

	if( command ) {
		free( command );
		command = NULL;
	}

	if( command_type ) {
		free( command_type );
		command_type = NULL;
	}

	if( response_mode ) {
		free( response_mode );
		response_mode = NULL;
	}
}

/*
SOCKET_process( int socket_fd )
@socket_fd - identyfikator gniazda
Funkcja odczytuje dane z gniazda. */
static void SOCKET_process( int socket_fd ) {
	CONNECTED_CLIENT *client = SOCKET_find_client( socket_fd );
	unsigned char parsed_content[ MAX_BUFFER ];
	char websocket_handshake_response[ MAX_BUFFER ];
	extern int errno;
	int websocket_recv_data_len;
	int websocket_send_data_len;
	unsigned char websocket_encoded_data[ MAX_BUFFER ];

	memset( websocket_encoded_data, '\0', MAX_BUFFER );
	errno = 0;
	communication_session_.socket_descriptor = socket_fd;
	memset( communication_session_.content, '\0', MAX_BUFFER );
	memset( parsed_content, '\0', MAX_BUFFER );

	communication_session_.data_length = recv( ( int )socket_fd, communication_session_.content, MAX_BUFFER, 0 );

	/* Do czasu przes³ania danych z przegl¹darki nie wiemy, czy handshake siê powiód³ - no chyba, ¿e to roz³¹czenie... */
	if( client->socket_info.connection_status == CPENDING ) {
		if( communication_session_.data_length > 0 ) {
			client->socket_info.connection_status = CCONNECTED;
		} else {
			LOG_print("Handshake failed.\n");
		}
	}

	if( errno > 1) {
		SOCKET_unregister_client( socket_fd );
		SOCKET_close( socket_fd );
	} else {
		if ( communication_session_.data_length <= 0 ) {
			/* ...ale to jednak by³o roz³¹czenie */
			SOCKET_unregister_client( socket_fd );
			SOCKET_close( socket_fd );
		} else {

			/* Weryfikacja typu socketu */
			if( client->socket_info.type == CUNKNOWN ) {
				client->socket_info.type = ( WEBSOCKET_valid_connection( communication_session_.content ) == 1 ? CWEBSOCKET : CSOCKET );
			}

			if( client->socket_info.type == CWEBSOCKET && client->socket_info.connection_status == CDISCONNECTED ) {
				/* Klient po³¹czony przez WebSocket, wymagany handshake */
				memset( websocket_handshake_response, '\0', MAX_BUFFER );
				WEBSOCKET_generate_handshake( communication_session_.content, websocket_handshake_response, MAX_BUFFER );
				SOCKET_send( &communication_session_, client, websocket_handshake_response, strlen( websocket_handshake_response ) );
				client->socket_info.connection_status = CPENDING;
			} else if( client->socket_info.type == CSOCKET && client->socket_info.connection_status == CDISCONNECTED ) {
				/* Klien po³¹czony przez zwyk³y socket */
				client->socket_info.connection_status = CCONNECTED;
			} else if( client->socket_info.connection_status == CCONNECTED ) {
				/* Klient w pe³ni gotowy do wymiany danych */
				if( client->socket_info.type == CWEBSOCKET ) {
					/* Pobranie danych z WebSocketu */
					websocket_recv_data_len = WEBSOCKET_get_content( communication_session_.content, communication_session_.data_length, parsed_content, MAX_BUFFER );
					if( websocket_recv_data_len == -2 ) {
						SOCKET_unregister_client( socket_fd );
						SOCKET_close( socket_fd );
					}
				} else if( client->socket_info.type == CSOCKET ) {
					/* Pobranie danych z socketu */
					strncpy( parsed_content, communication_session_.content, MAX_BUFFER );
				}
				/* Przetworzenie komendy */
				if( websocket_recv_data_len > 0 ) {
					REQUEST_parse_command( client, parsed_content );
				}
			}
		}
	}
}

void SOCKET_modify_clients_count( int mod ) {
	if( mod > 0 ) {
		http_conn_count++;
	} else {
		if( (http_conn_count - mod) >= 0 ) {
			http_conn_count--;
		}
	}
}

void SOCKET_close( int socket_descriptor ) {
	FD_CLR( socket_descriptor, &master );
	shutdown( socket_descriptor, SHUT_RDWR );
	close( socket_descriptor );
	/* Zmniejszensie licznika podï¿½ï¿½czonych klientï¿½w */
	SOCKET_modify_clients_count( -1 );
}

/*
SOCKET_stop( void )
- zwolnienie WinSock
- zwolnienie socketa */
void SOCKET_stop( void ) {
	shutdown( socket_server, SHUT_RDWR );
	close( communication_session_.socket );
	close( socket_server );

#ifdef _WIN32
	WSACleanup();
#endif
	//LOG_print( "Sockets closed.\n" );
}

/*
SOCKET_release( COMMUNICATION_SESSION *communication_session )
@communication_session - wskaï¿½nik do podï¿½ï¿½czonego klienta
- funkcja resetuje zmienne informujï¿½ce o podï¿½ï¿½czonym sockecie. */
void SOCKET_release( COMMUNICATION_SESSION *communication_session ) {
	communication_session->socket_descriptor = -1;
	communication_session->data_length = -1;
	communication_session->keep_alive = -1;
}

/*
SOCKET_disconnect_client( COMMUNICATION_SESSION *communication_session )
- rozï¿½ï¿½cza klienta podanego jako struktura communication_session */
void SOCKET_disconnect_client( COMMUNICATION_SESSION *communication_session ) {
	if( communication_session->socket_descriptor != SOCKET_ERROR ) {
		SOCKET_close( communication_session->socket_descriptor );
	} else {
		SOCKET_release( communication_session );
	}
}

/*
void SOCKET_send( COMMUNICATION_SESSION *communication_session, const char *data, unsigned int data_size )
- wysyï¿½a pakiet danych ( data ) do danego klienta ( communication_session ) */
void SOCKET_send( COMMUNICATION_SESSION *communication_session, CONNECTED_CLIENT *client, const char *data, unsigned int data_size ) {
	int _data_size = data_size;
	char *data_to_send = ( char * )calloc( MAX_BUFFER, sizeof( char ) );

	if( _data_size == -1 ) {
		_data_size = strlen( data );
	}
	if( client->socket_info.type == CWEBSOCKET  && ( client->socket_info.connection_status == CPENDING || client->socket_info.connection_status == CCONNECTED ) ) {
		_data_size = WEBSOCKET_set_content( data, _data_size, data_to_send, MAX_BUFFER );
	} else if(client->socket_info.type == CWEBSOCKET  && client->socket_info.connection_status == CDISCONNECTED ) {
		strncpy( data_to_send, data, MAX_BUFFER );
	}else if( client->socket_info.type == CSOCKET ) {
		strncpy( data_to_send, data, MAX_BUFFER );
	}

	if( ( communication_session->data_length = send( client->socket_descriptor, data_to_send, _data_size, 0 ) ) <= 0 ) {
		SOCKET_disconnect_client( communication_session );
	}

	free( data_to_send );
	data_to_send = NULL;
}

/*
server_get_remote_hostname( const char *remote_addr )
@communication_session - wskaŸnik do pod³¹czonego klienta
- zwraca ciï¿½g znakï¿½w bï¿½dï¿½cy nazwï¿½ hosta. */
char* server_get_remote_hostname( COMMUNICATION_SESSION *communication_session ) {
	static char remote_name[ TINY_BUFF_SIZE ];
	memset( remote_name, '\0', TINY_BUFF_SIZE );
	getnameinfo( ( struct sockaddr * )&communication_session->address, sizeof( communication_session->address ), remote_name, sizeof( remote_name ), NULL, 0, NI_NAMEREQD );
	return ( ( char* )&remote_name );
}

/*
SOCKET_get_remote_ip( COMMUNICATION_SESSION *communication_session )
@communication_session - wskaŸnik do pod³¹czonego klienta
- zwraca ciï¿½g znakï¿½w bï¿½dï¿½cy adresem IP. */
char* SOCKET_get_remote_ip( COMMUNICATION_SESSION *communication_session ) {
	static char ip_addr[ TINY_BUFF_SIZE ];
	memset( ip_addr, '\0', TINY_BUFF_SIZE );
	getnameinfo( ( struct sockaddr * )&communication_session->address, sizeof( communication_session->address ), ip_addr, sizeof( ip_addr ), NULL, 0, NI_NUMERICHOST );
	return ( ( char* )&ip_addr );
}

/*
SOCKET_main( void )
- obsï¿½uga funkcji socketï¿½w */
void SOCKET_main( void ) {
	( void )SOCKET_initialization();
	( void )SOCKET_prepare();
	( void )SOCKET_run();
	( void )SOCKET_stop();
}

/*
SOCKET_register_client( int socket_descriptor )
@socket_descriptor - identyfikator gniazda
- funkcja znajduje pust¹ strukturê w tablicy SEND_INFO i przydziela jej identyfikator gniazda w celu umo¿liwienia póŸniejszej wysy³ki danych */
void SOCKET_register_client( int socket_descriptor ) {
	int i;

	for( i = 0; i < MAX_CLIENTS; i++ ){
		if( connected_clients[ i ].socket_descriptor == 0 ) {
			connected_clients[ i ].socket_descriptor = socket_descriptor;
			connected_clients[ i ].binded = 0;
			connected_clients[ i ].mode = CONNECTION;
			connected_clients[ i ].authorized = 0;
			connected_clients[ i ].socket_info.type = CUNKNOWN;
			connected_clients[ i ].socket_info.connection_status = CDISCONNECTED;
			//LOG_print("[%s] client connected with descriptor %d.\n", get_actual_time_gmt(), socket_descriptor );
			return;
		}
	}
}

/*
SESSION_add_new_send_struct( int socket_descriptor )
@socket_descriptor - identyfikator gniazda
- funkcja usuwa strukturê SEND_INFO, poniewa¿ wysy³anie contentu zakoñczy³o siê lub klient siê roz³¹czy³ */
void SOCKET_unregister_client( int socket_descriptor ) {
	int i;

	for( i = 0; i < MAX_CLIENTS; i++ ){
		if( connected_clients[ i ].socket_descriptor == socket_descriptor ) {
			connected_clients[ i ].socket_descriptor = 0;
			connected_clients[ i ].binded = 0;
			connected_clients[ i ].mode = CONNECTION;
			connected_clients[ i ].authorized = 0;
			connected_clients[ i ].socket_info.type = CUNKNOWN;
			connected_clients[ i ].socket_info.connection_status = CDISCONNECTED;
			//LOG_print("[%s] client disconnected with descriptor %d.\n", get_actual_time_gmt(), socket_descriptor );
			break;
		}
	}

	for( i = 0; i < MAX_CLIENTS; i++ ) {
		if( i <= MAX_CLIENTS && connected_clients[ i ].socket_descriptor == 0 && connected_clients[ i + 1].socket_descriptor > 0) {
			connected_clients[ i ] = connected_clients[ i + 1 ];
			connected_clients[ i + 1 ].socket_descriptor = 0;
			connected_clients[ i + 1 ].authorized = 0;
			connected_clients[ i + 1 ].binded = 0;
			connected_clients[ i + 1 ].mode = CONNECTION;
		}
	}
}

CONNECTED_CLIENT* SOCKET_find_client( int socket_descriptor ) {
	int i;

	for( i = 0; i < MAX_CLIENTS; i++ ){
		if( connected_clients[ i ].socket_descriptor == socket_descriptor ) {
			return &connected_clients[ i ];
		}
	}
}
