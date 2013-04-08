/*******************************************************************

Projekt Saturn V Main Computer

Plik: celestial_objects.c

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/celestial_objects.h"

void CELESTIAL_OBJECTS_load( void ) {

	/**
	* Earth
	**/
	printf("Loading Earth data...");
	strncpy( AO_Earth.name, "EARTH", SMALL_BUFF_SIZE );
	AO_Earth.gravitation = 9.82;
	AO_Earth.mass = (5.98 * pow( 10, 24 ));
	AO_Earth.radius = 6378140;
	AO_Earth.orbit_radius = 924375700;
	strncpy( AO_Earth.ground_destination, "EPO", SMALL_BUFF_SIZE );
	AO_Earth.ground_destination_altitude = 190756;
	AO_Earth.current_position = 0;
	printf("done.\n");

	/**
	* Moon
	**/
	printf("Loading Moon data...");
	strncpy( AO_Moon.name, "MOON", SMALL_BUFF_SIZE );
	AO_Moon.gravitation = 1.63;
	AO_Moon.mass = 7.34 * pow( 10, 22 );
	AO_Moon.radius = 1737000;
	AO_Moon.orbit_radius = 2413402000;
	strncpy( AO_Moon.ground_destination, "LO", SMALL_BUFF_SIZE );
	AO_Moon.ground_destination_altitude = 111120;
	AO_Moon.current_position = 0;
	printf("done.\n");

	AO_current = &AO_Earth;
}

char* CELESTIAL_OBJECT_get_current_destination( CELESTIAL_OBJECT *co ) {
	return ( co->current_position == 0 ? co->ground_destination : co->space_destination );
}

int CELESTIAL_OBJECT_get_current_destination_altitude( CELESTIAL_OBJECT *co ) {
	return ( co->current_position == 0 ? co->ground_destination_altitude : -1 );
}

double CELESTIAL_OBJECT_get_orbital_speed( CELESTIAL_OBJECT *co, int height ) {
	return sqrt( (GRAVITATIONAL_CONSTANT * co->mass) / ( co->radius + height ) );
}

int CELESTIAL_OBJECT_get_altitude_from_velocity( CELESTIAL_OBJECT *co, int velocity ) {
	return (GRAVITATIONAL_CONSTANT * ( co->mass/pow( velocity, 2 ) ) ) - co->radius;
}

double CELESTIAL_OBJECT_get_gravity_value( CELESTIAL_OBJECT *co, int height ) {
	if( height >= 0 ) {
		return ( co->gravitation * pow( (co->radius / ( co->radius + height ) ), 2 ) );
	} else {
		return co->gravitation;
	}
}

