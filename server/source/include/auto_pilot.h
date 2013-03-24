/*******************************************************************

Projekt Saturn V Main Computer

Plik: auto_pilot.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef AUTOPILOT_H
#define AUTOPILOT_H

double _AUTOPILOT_get_pitch_step( void );
double _AUTOPILOT_get_roll_step( void );
double _AUTOPILOT_get_yaw_step( void );
void AUTOPILOT_progress( double real_second );
void AUTOPILOT_init( void );

#endif


