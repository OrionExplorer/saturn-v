/*******************************************************************

Projekt Saturn V Main Computer

Plik: communication.c

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/communication.h"

void COMMUNICATION_parse_command( CONNECTED_CLIENT *client, const char *data ) {
	vDEVICE rocket_device;
	vCOMMAND rocket_command;
	int rocket_value;
	INTERPRETER_RESULT *result;

	cJSON *json = cJSON_Parse( data );
	cJSON *command_json;
	cJSON *command_type_json;
	cJSON *response_mode_json;

	cJSON *username_json;
	cJSON *password_json;

	char *command = NULL;
	char *command_type = NULL;
	char *response_mode = NULL;
	char *username = NULL;
	char *password = NULL;
	char *login_response = NULL;
	char *main_computer_response_str = NULL;
	short main_computer_response_success = 0;

	LOG_print( "[%s] [%d] Received command: \"%s\".\n", TIME_get_gmt(), client->socket_descriptor, data );

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
			if( command_type ) {
				if( strncmp( "authorization", command_type, STD_BUFF_SIZE ) == 0 ) {
					username_json = cJSON_GetObjectItem( json, "username" );
					password_json = cJSON_GetObjectItem( json, "password" );

					if( username_json != NULL ) {
						username = username_json->valuestring;
					}
					if( password_json != NULL ) {
						password = password_json->valuestring;
					}

					if( username != NULL && password != NULL ) {
						if( strncmp( app_auth, password, STD_BUFF_SIZE ) == 0 ) {
							client->authorized = 1;
							strncpy( client->name, username, STD_BUFF_SIZE );
							SOCKET_send( &communication_session_, client, LOGIN_SUCCESS, -1 );
							LOG_print( "[%s] client connected with descriptor %d is %s.\n", TIME_get_gmt(), client->socket_descriptor, username );

							login_response = ( char * )calloc( STD_BUFF_SIZE, sizeof( char ) );
							snprintf( login_response, STD_BUFF_SIZE, NEW_USER_STR, client->name );
							COMMUNICATION_send_to_all( login_response );

							if( login_response != NULL ) {
								free( login_response );
								login_response = NULL;
							}

						} else {
							SOCKET_send( &communication_session_, client, LOGIN_STR, -1 );
						}

						if( username != NULL ) {
							free( username );
							username = NULL;
						}

						if( password != NULL ) {
							free( password );
							password = NULL;
						}
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
		if( command != NULL && command_type != NULL ) {
			if(strncmp( "computer", command_type, STD_BUFF_SIZE ) == 0 ) {
				if( sscanf( command, "%d %d %d", ( int * )&rocket_device, ( int * )&rocket_command, &rocket_value ) == 3 ) {
					main_computer_response_str = ( char * )calloc( STD_BUFF_SIZE, sizeof( char ) );
					result = MAIN_COMPUTER_exec( rocket_device, rocket_command, rocket_value );

					main_computer_response_success = result->success;
					snprintf( main_computer_response_str, STD_BUFF_SIZE, MAIN_COMPUTER_RESPONSE_TEMPLATE, ( main_computer_response_success == 1 ? "true" : "false" ), result->message );
					SOCKET_send( &communication_session_, client, main_computer_response_str, -1 );

					if( main_computer_response_str != NULL ) {
						free( main_computer_response_str );
						main_computer_response_str = NULL;
					}

				} else {
					SOCKET_send( &communication_session_, client, "ILLEGAL COMMAND", -1 );
				}
			} else if( strncmp( "data", command_type, STD_BUFF_SIZE ) == 0 ) {
				if( strncmp( "status", command, STD_BUFF_SIZE ) == 0 ) {
					TELEMETRY_send_ondemand_data( client );
				}
			} else if( strncmp( "chat_message", command_type, STD_BUFF_SIZE ) == 0 ) {
				COMMUNICATION_chat_to_all( command, client );
			}

			if( command != NULL ) {
				free( command );
				command = NULL;
			}
		}

		if( response_mode != NULL ) {
			if(strncmp( "live", response_mode, STD_BUFF_SIZE ) == 0 ) {
				client->binded = 1;
			} else if ( strncmp( "on-demand", response_mode, STD_BUFF_SIZE ) == 0 ) {
				client->binded = 0;
			}
		}
	}

	if( command_type != NULL ) {
		free( command_type );
		command_type = NULL;
	}

	if( response_mode != NULL ) {
		free( response_mode );
		response_mode = NULL;
	}
}

void COMMUNICATION_send_to_all( char *msg ) {
	int i;

	for( i = 0; i < MAX_CLIENTS; i++ ) {
		if(connected_clients[ i ].socket_descriptor > 0 ) {
			if( connected_clients[ i ].authorized == 1 && connected_clients[ i ].binded == 1 ) {
				SOCKET_send( &communication_session_, &connected_clients[ i ], msg, -1 );
			}
		}
	}
}

void COMMUNICATION_chat_to_all( char *msg, CONNECTED_CLIENT *client ) {
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

	COMMUNICATION_send_to_all( output );

	cJSON_Delete( root );
	root = NULL;

	free( output );
	output = NULL;
}
