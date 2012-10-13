/*******************************************************************

Projekt Voyager 7 Board Computer

Plik: main_computer.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef MAIN_COMPUTER_H
#define MAIN_COMPUTER_H

#include "celestial_objects.h"
#include "spacecraft_components.h"

/* G³ówny silnik */
ROCKET_ENGINE main_engine;
ROCKET_ENGINE aps_system;

void MAIN_COMPUTER_init( void );
void compute_all( void );
void TELEMETRY_update( void );
void* TELEMETRY_send_live_data( void* data );
void TELEMETRY_send_ondemand_data( CONNECTED_CLIENT *client );
void TELEMETRY_prepare_data( char *dst, unsigned int dst_len );

INTERPRETER_RESULT* EXEC_COMMAND( vDEVICE device, vCOMMAND command, const int value );

#endif
