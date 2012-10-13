/*******************************************************************

Projekt Voyager 7 Board Computer

Plik: celestial_objects.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef CELESTIAL_OBJECTS_H
#define CELESTIAL_OBJECTS_H

#include <math.h>
#include "server_shared.h"

#define GRAVITATIONAL_CONSTANT			6.67*pow(10, -11)
#define AU								149598000000LL
#define _PI								4.0 * atan( 1.0 )

typedef struct CELESTIAL_OBJECT			CELESTIAL_OBJECT;
struct CELESTIAL_OBJECT {
	char name[ SMALL_BUFF_SIZE ];
	double gravitation;
	long double mass;
	long double radius;
	long double orbit_radius;
	char ground_destination[ SMALL_BUFF_SIZE ];
	int ground_destination_altitude;
	char space_destination[ SMALL_BUFF_SIZE ];
	int current_position;
};

char* CELESTIAL_OBJECT_get_current_destination( CELESTIAL_OBJECT *co );
int CELESTIAL_OBJECT_get_current_destination_altitude( CELESTIAL_OBJECT *co );
double CELESTIAL_OBJECT_get_orbital_speed( CELESTIAL_OBJECT *co, int height );
int CELESTIAL_OBJECT_get_altitude_from_velocity( CELESTIAL_OBJECT *co, int velocity );
double CELESTIAL_OBJECT_get_gravity_value( CELESTIAL_OBJECT *co, int height );

CELESTIAL_OBJECT AO_Earth;
CELESTIAL_OBJECT AO_Moon;
CELESTIAL_OBJECT *AO_current;

void CELESTIAL_OBJECTS_load( void );

#endif
