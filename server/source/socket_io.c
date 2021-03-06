/*******************************************************************

Projekt voyager7computer

Plik: socket_io.c

Przeznaczenie:
Inicjalizacja socket�w
Konfiguracja socket�w
Odbieranie danych z sieci i przekazanie do interpretacji

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/socket_io.h"
#include "include/shared.h"
#include "include/log.h"
#include "include/file_util.h"
#include "include/string_util.h"
#include "include/cWebSockets.h"
#include "include/main_computer.h"
#include "include/telemetry.h"
#include "include/communication.h"
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
	/*Utworzenie socketa nas�uchuj�cego */
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
	server_address.sin_port = htons( ( unsigned short )active_port );
}

/*
SOCKET_prepare( void )
Nas�uchiwanie w celu odbioru danych od klienta */
static void SOCKET_prepare( void ) {
	unsigned long b = 0;
	int i = 1;
	int wsa_result = 0;
	struct timeval tv = {0, 0};

	tv.tv_sec = 0;
	tv.tv_usec = 20000;

	FD_ZERO( &master );
	FD_ZERO( &read_fds );

#ifndef _WIN32
	setuid( 0 );
	setgid( 0 );
#endif

	if( setsockopt( socket_server, SOL_SOCKET, SO_REUSEADDR, ( char * )&i, sizeof( i ) ) == SOCKET_ERROR ) {
		wsa_result = WSAGetLastError();
		LOG_print( "setsockopt( SO_REUSEADDR ) error: %d.\n", wsa_result );
		printf( "setsockopt( SO_REUSEADDR ) error: %d.\n", wsa_result );
	}

	if( setsockopt( socket_server, SOL_SOCKET, SO_RCVTIMEO, ( char* )&tv, sizeof( struct timeval ) ) == SOCKET_ERROR ) {
		wsa_result = WSAGetLastError();
		LOG_print( "setsockopt( SO_RCVTIMEO ) error: %d.\n", wsa_result );
		printf( "setsockopt( SO_RCVTIMEO ) error: %d.\n", wsa_result );
	}

	if( setsockopt( socket_server, SOL_SOCKET, SO_SNDTIMEO, ( char* )&tv, sizeof( struct timeval ) ) == SOCKET_ERROR ) {
		wsa_result = WSAGetLastError();
		LOG_print( "setsockopt( SO_SNDTIMEO ) error: %d.\n", wsa_result );
		printf( "setsockopt( SO_SNDTIMEO ) error: %d.\n", wsa_result );
	}

	if( setsockopt( socket_server, IPPROTO_TCP, TCP_NODELAY, ( char * )&i, sizeof( i ) ) == SOCKET_ERROR ) {
		wsa_result = WSAGetLastError();
		LOG_print( "setsockopt( TCP_NODELAY ) error: %d.\n", wsa_result );
		printf( "setsockopt( TCP_NODELAY ) error: %d.\n", wsa_result );
	}

	/* Ustawienie na non-blocking socket */
	if( fcntl( socket_server, F_SETFL, &b ) == SOCKET_ERROR ) {
		wsa_result = WSAGetLastError();
		LOG_print( "ioctlsocket() error: %d.\n", wsa_result );
		printf( "ioctlsocket() error: %d.\n", wsa_result );
		SOCKET_stop();
		exit( EXIT_FAILURE );
	}

	if ( bind( socket_server, ( struct sockaddr* )&server_address, sizeof( server_address ) ) == SOCKET_ERROR ) {
		wsa_result = WSAGetLastError();
		LOG_print( "bind() error: %d.\n", wsa_result );
		printf( "bind() error: %d.\n", wsa_result );
		SOCKET_stop();
		exit( EXIT_FAILURE );
	}

	/* Rozpocz�cie nas�uchiwania */
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
	/* Teraz czekamy na po��czenia i dane */
}

/*
SOCKET_run( void )
Funkcja zarz�dzaj�ca po��czeniami przychodz�cymi do gniazda. */
void SOCKET_run( void ) {
	register int i = 0;
	struct timeval tv;
	pthread_t sthread;

	FD_SET( socket_server, &master );

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	fdmax = socket_server;

	/* Uruchomienie w�tku wysy�aj�cego dane do pod��czonych klient�w */
	pthread_create( &sthread, NULL, TELEMETRY_send_live_data, "saturnVcomputer" );

	for( ;"elvis presley lives"; ) {
		read_fds = master;
		if( select( fdmax+1, &read_fds, NULL, NULL, &tv ) == -1 ) {
			SOCKET_stop();
			exit( EXIT_FAILURE );
		}

		i = fdmax+1;
		while( --i ) {
			if( FD_ISSET( i, &read_fds ) ) { /* Co� si� dzieje na sockecie... */
				if( i == socket_server ) {
					/* Pod��czy� si� nowy klient */
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
					/* Pod��czony klient przes�a� dane */
					SOCKET_process( i );
				}
			} /*nowe po��czenie */
		} /*p�tla deskryptor�w while( --i )*/
		Sleep( 1 );
	} /*for( ;; ) */
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
	char *main_computer_response_str;

	memset( websocket_encoded_data, '\0', MAX_BUFFER );
	errno = 0;
	communication_session_.socket_descriptor = socket_fd;
	memset( communication_session_.content, '\0', MAX_BUFFER );
	memset( parsed_content, '\0', MAX_BUFFER );

	communication_session_.data_length = recv( ( int )socket_fd, communication_session_.content, MAX_BUFFER, 0 );

	/* Do czasu przes�ania danych z przegl�darki nie wiemy, czy handshake si� powi�d� - no chyba, �e to roz��czenie... */
	if( client->socket_info.connection_status == CPENDING ) {
		if( communication_session_.data_length > 0 ) {
			client->socket_info.connection_status = CCONNECTED;
		} else {
			LOG_print( "[%s] WebSocket handshake failed.\n", TIME_get_gmt() );
		}
	}

	if( errno > 1) {
		SOCKET_unregister_client( socket_fd );
		SOCKET_close( socket_fd );
	} else {
		if ( communication_session_.data_length <= 0 ) {
			/* ...ale to jednak by�o roz��czenie */
			if( strlen( client->name ) > 0 ) {
				main_computer_response_str = ( char * )calloc( STD_BUFF_SIZE, sizeof( char ) );
				snprintf( main_computer_response_str, STD_BUFF_SIZE, DEL_USER_STR, client->name );
				COMMUNICATION_send_to_all( main_computer_response_str );

				free( main_computer_response_str );
				main_computer_response_str = NULL;
			}

			SOCKET_unregister_client( socket_fd );
			SOCKET_close( socket_fd );
		} else {

			/* Weryfikacja typu socketu */
			if( client->socket_info.type == CUNKNOWN ) {
				client->socket_info.type = ( WEBSOCKET_valid_connection( communication_session_.content ) == 1 ? CWEBSOCKET : CSOCKET );
			}

			if( client->socket_info.type == CWEBSOCKET && client->socket_info.connection_status == CDISCONNECTED ) {
				/* Klient po��czony przez WebSocket, wymagany handshake */
				memset( websocket_handshake_response, '\0', MAX_BUFFER );
				WEBSOCKET_generate_handshake( communication_session_.content, websocket_handshake_response, MAX_BUFFER );
				SOCKET_send( &communication_session_, client, websocket_handshake_response, strlen( websocket_handshake_response ) );
				client->socket_info.connection_status = CPENDING;
			} else if( client->socket_info.type == CSOCKET && client->socket_info.connection_status == CDISCONNECTED ) {
				/* Klien po��czony przez zwyk�y socket */
				client->socket_info.connection_status = CCONNECTED;
			} else if( client->socket_info.connection_status == CCONNECTED ) {
				/* Klient w pe�ni gotowy do wymiany danych */
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
					COMMUNICATION_parse_command( client, parsed_content );
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
	/* Zmniejszensie licznika pod��czonych klient�w */
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
@communication_session - wska�nik do pod��czonego klienta
- funkcja resetuje zmienne informuj�ce o pod��czonym sockecie. */
void SOCKET_release( COMMUNICATION_SESSION *communication_session ) {
	communication_session->socket_descriptor = -1;
	communication_session->data_length = -1;
	communication_session->keep_alive = -1;
}

/*
SOCKET_disconnect_client( COMMUNICATION_SESSION *communication_session )
- roz��cza klienta podanego jako struktura communication_session */
void SOCKET_disconnect_client( COMMUNICATION_SESSION *communication_session ) {
	if( communication_session->socket_descriptor != SOCKET_ERROR ) {
		SOCKET_close( communication_session->socket_descriptor );
	} else {
		SOCKET_release( communication_session );
	}
}

/*
void SOCKET_send( COMMUNICATION_SESSION *communication_session, const char *data, unsigned int data_size )
- wysy�a pakiet danych ( data ) do danego klienta ( communication_session ) */
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
SOCKET_get_remote_ip( COMMUNICATION_SESSION *communication_session )
@communication_session - wska�nik do pod��czonego klienta
- zwraca ci�g znak�w b�d�cy adresem IP. */
char* SOCKET_get_remote_ip( COMMUNICATION_SESSION *communication_session ) {
	static char ip_addr[ TINY_BUFF_SIZE ];
	memset( ip_addr, '\0', TINY_BUFF_SIZE );
	getnameinfo( ( struct sockaddr * )&communication_session->address, sizeof( communication_session->address ), ip_addr, sizeof( ip_addr ), NULL, 0, NI_NUMERICHOST );
	return ( ( char* )&ip_addr );
}

/*
SOCKET_main( void )
- obs�uga funkcji socket�w */
void SOCKET_main( void ) {
	( void )SOCKET_initialization();
	( void )SOCKET_prepare();
	( void )SOCKET_run();
	( void )SOCKET_stop();
}

/*
SOCKET_register_client( int socket_descriptor )
@socket_descriptor - identyfikator gniazda
- funkcja znajduje pust� struktur� w tablicy SEND_INFO i przydziela jej identyfikator gniazda w celu umo�liwienia p�niejszej wysy�ki danych */
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
			memset( connected_clients[ i ].name, '\0', STD_BUFF_SIZE );
			LOG_print("[%s] client connected with descriptor %d.\n", TIME_get_gmt(), socket_descriptor );
			return;
		}
	}
}

/*
SESSION_add_new_send_struct( int socket_descriptor )
@socket_descriptor - identyfikator gniazda
- funkcja usuwa struktur� SEND_INFO, poniewa� wysy�anie contentu zako�czy�o si� lub klient si� roz��czy� */
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
			if( strlen( connected_clients[ i ].name ) > 0 ) {
				LOG_print("[%s] client disconnected: %s (descriptor %d.)\n", TIME_get_gmt(), connected_clients[ i ].name, socket_descriptor );
			} else {
				LOG_print("[%s] client disconnected: %d.\n", TIME_get_gmt(), socket_descriptor );
			}

			memset( connected_clients[ i ].name, '\0', STD_BUFF_SIZE );
			break;
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
