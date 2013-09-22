/*******************************************************************

Projekt Saturn V Main Computer

Plik: server_communication.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef MAIN_COMPUTER_H
#define MAIN_COMPUTER_H

#include "server_shared.h"

void COMMUNICATION_parse_command( CONNECTED_CLIENT *client, const char *data );
void COMMUNICATION_send_to_all( char *msg );
void COMMUNICATION_chat_to_all( char *msg, CONNECTED_CLIENT *client );

#endif
