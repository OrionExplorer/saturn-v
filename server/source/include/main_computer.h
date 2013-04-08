/*******************************************************************

Projekt Saturn V Main Computer

Plik: main_computer.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef MAIN_COMPUTER_H
#define MAIN_COMPUTER_H

#include "celestial_objects.h"
#include "spacecraft_components.h"

void MAIN_COMPUTER_init( void );
double _PHYSICS_get_orbit_eccentrity( double altitude, double velocity );
double _PHYSICS_get_orbit_semi_major_axis( double altitude, double velocity );
double _PHYSICS_get_orbit_perigee( double semi_major_axis, double eccentrity );
double _PHYSICS_get_orbit_apogee( double semi_major_axis, double eccentrity );
INTERPRETER_RESULT* EXEC_COMMAND( vDEVICE device, vCOMMAND command, const int value );

#endif
