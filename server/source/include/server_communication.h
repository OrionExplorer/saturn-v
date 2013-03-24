/*******************************************************************

Projekt Saturn V Main Computer

Plik: server_communication.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef MAIN_COMPUTER_H
#define MAIN_COMPUTER_H

#include "server_shared.h"

void SYS_MESSAGE_send_to_all( char *msg );
void CHAT_send_to_all( char *msg, CONNECTED_CLIENT *client );

#endif
