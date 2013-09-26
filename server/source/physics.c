/*******************************************************************

Projekt Saturn V Main Computer

Plik: physics.c

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/physics.h"
#include "include/shared.h"
#include "include/celestial_objects.h"

double		normal_atmospheric_pressure = 1013.25;
int			current_dynamic_pressure = 0;

double _PHYSICS_get_orbit_mean_motion( double semi_major_axis ) {
	return sqrt( (GRAVITATIONAL_CONSTANT * AO_current->mass ) / pow( semi_major_axis, 3 ) );
}

double _PHYSICS_get_orbit_periapsis_velocity( double apoapsis, double periapsis ) {
	double Rp = AO_current->radius + periapsis;
	double Ra = AO_current->radius + apoapsis;
	double GM = GRAVITATIONAL_CONSTANT * AO_current->mass;

	return sqrt( 2 * GM * Ra / ( Rp * ( Ra + Rp ) ) );
}

double _PHYSICS_get_orbit_apoapsis_velocity( double apoapsis, double periapsis ) {
	double Rp = AO_current->radius + periapsis;
	double Ra = AO_current->radius + apoapsis;
	double GM = GRAVITATIONAL_CONSTANT * AO_current->mass;

	return apoapsis > 0 ? sqrt( 2 * GM * Rp / ( Ra * ( Ra + Rp ) ) ) : 0;
}

double _PHYSICS_get_orbit_revolution_period( double semi_major_axis ) {
	return sqrt( 4 * ( _PI * _PI ) * pow( semi_major_axis, 3 ) / ( GRAVITATIONAL_CONSTANT * AO_current->mass ) );
}

double _PHYSICS_get_orbit_circumference( double semi_major_axis, double semi_minor_axis ) {
	return ( ( sqrt( 0.5 * (( semi_major_axis * semi_major_axis) + ( semi_minor_axis * semi_minor_axis ) ) ) ) * ( _PI * 2 ) ) / 2;
}

double _PHYSICS_get_orbit_eccentrity( double altitude, double velocity ) {
	return ( AO_current->radius + altitude ) * pow( velocity, 2 ) / ( 3.986005 * pow( 10, 14 ) ) -1;
}

double _PHYSICS_get_orbit_semi_minor_axis( double semi_major_axis, double eccentrity ) {
	return sqrt( semi_major_axis * sqrt( 1 - pow( eccentrity, 2 ) ) * ( semi_major_axis * ( 1 - pow( eccentrity, 2 ) ) ) );
}

double _PHYSICS_get_orbit_semi_major_axis( double altitude, double velocity ) {
	return ( 1 / ( 2 / ( AO_current->radius + altitude ) - pow( velocity, 2 ) / ( 3.986005 * pow( 10,14 ) ) ) );
}

double _PHYSICS_get_orbit_perigee( double semi_major_axis, double eccentrity ) {
	return semi_major_axis * ( 1 - eccentrity ) - AO_current->radius;
}

double _PHYSICS_get_orbit_apogee( double semi_major_axis, double eccentrity ) {
	return semi_major_axis * ( 1 + eccentrity ) - AO_current->radius;
}

double _PHYSICS_get_orbit_inclination( double latitude, double current_roll ) {
	return ( acos( cos( latitude * DEG2RAD ) * sin( ( 90 - current_roll ) * DEG2RAD ) ) ) * 57.29577;
}

double _PHYSICS_get_dynamic_pressure_force( double altitude ) {
	double current_temperature = 288.15 - (0.0065 * altitude );
	long double gas_constant = 8.3144621;
	double current_atmospheric_pressure = normal_atmospheric_pressure * exp( ( -1 * ( 0.0289644 * CELESTIAL_OBJECT_get_gravity_value( AO_current, altitude ) * altitude ) ) / (gas_constant * current_temperature ) );
	double current_air_destiny = current_atmospheric_pressure / ( gas_constant * current_temperature );
	double dynamic_pressure = ( 0.5 * current_air_destiny) * pow( telemetry_data.last_velocity, 2 );
	double result = dynamic_pressure;

	if( telemetry_data.max_q_achieved == 1 ) {
		if( round( result ) == 0 ) {
			return round( result );
		}
	} else {
		if( result >= telemetry_data.current_dynamic_pressure ) {
			telemetry_data.current_dynamic_pressure = result;
			return round( result );
		} else {
			telemetry_data.max_q_achieved = 1;
			strncpy( telemetry_data.computer_message, "MAXIMUM DYNAMIC PRESSURE", STD_BUFF_SIZE );
			MAIN_COMPUTER_display_last_message();
			telemetry_data.current_dynamic_pressure = round( telemetry_data.current_dynamic_pressure );
			return current_dynamic_pressure;
		}
	}
	return ( result >= 0 ? result : -1 );
}

double PHYSICS_IGM_get_pitch_step( void ) {
	long int seconds = round( telemetry_data.mission_time );
	double result = 0.0;

	if( seconds >= 210 && seconds < 260 ) {
		result = 0.349;
	}
	if( seconds >= 260 && telemetry_data.stable_orbit_achieved == 0 ) {
		result = 0.055;
	}

	if( telemetry_data.current_vertical_velocity <= 0 ) {
		if( telemetry_data.stable_orbit_achieved == 0 ) {
			result = -0.0662500;
		}
	}

	return result + ( telemetry_data.stable_orbit_achieved == 0 ? 0.0662500 : 0 );
}