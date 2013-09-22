/*******************************************************************

Projekt Saturn V Main Computer

Plik: telemetry.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef SERVER_TELEMETRY_H
#define SERVER_TELEMETRY_H

#include "shared.h"

void TELEMETRY_prepare_data( char *dst, unsigned int dst_len );
void* TELEMETRY_send_live_data( void* data );
void TELEMETRY_send_ondemand_data( CONNECTED_CLIENT *client);
void TELEMETRY_update( void );

#endif

