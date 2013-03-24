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
INTERPRETER_RESULT* EXEC_COMMAND( vDEVICE device, vCOMMAND command, const int value );

#endif
