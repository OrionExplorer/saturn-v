/*******************************************************************

Projekt voyager7computer

Plik: socket_io.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef SERVER_SOCKET_IO_H
#define SERVER_SOCKET_IO_H

#include "shared.h"

void			SOCKET_main( void );
void			SOCKET_run( void );
void			SOCKET_stop( void );

void			SOCKET_send( COMMUNICATION_SESSION *communication_session, CONNECTED_CLIENT *client, const char *data, unsigned int data_size );

void			SOCKET_disconnect_client( COMMUNICATION_SESSION *communication_session );
void			SOCKET_release( COMMUNICATION_SESSION *communication_session );
char*			SOCKET_get_remote_ip( COMMUNICATION_SESSION *communication_session );
void			SOCKET_close( int socket_descriptor );
void			SOCKET_modify_clients_count( int mod );
void			SOCKET_register_client( int socket_descriptor );
void			SOCKET_unregister_client( int socket_descriptor );
CONNECTED_CLIENT* SOCKET_find_client( int socket_descriptor );

#endif
