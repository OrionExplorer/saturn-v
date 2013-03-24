/*******************************************************************

Projekt Saturn V Main Computer

Plik: server_communication.c

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/server_communication.h"

void SYS_MESSAGE_send_to_all( char *msg ) {
	int i;

	for( i = 0; i < MAX_CLIENTS; i++ ) {
		if(connected_clients[ i ].socket_descriptor > 0 ) {
			if( connected_clients[ i ].authorized == 1 && connected_clients[ i ].binded == 1 ) {
				SOCKET_send( &communication_session_, &connected_clients[ i ], msg, -1 );
			}
		}
	}
}

void CHAT_send_to_all( char *msg, CONNECTED_CLIENT *client ) {
	int i;
	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();
	char *output;

	cJSON_AddTrueToObject( root, "success" );
	cJSON_AddStringToObject( root, "data_type", "chat_message" );
	cJSON_AddItemToObject( root, "data", data );

	cJSON_AddStringToObject( data, "text", msg );
	if( strlen( client->name ) > 0 ) {
		cJSON_AddStringToObject( data, "user", client->name );
	} else {
		cJSON_AddNumberToObject( data, "user", client->socket_descriptor );
	}

	output = cJSON_Print( root );

	SYS_MESSAGE_send_to_all( output );

	cJSON_Delete( root );
	root = NULL;

	free( output );
	output = NULL;
}
