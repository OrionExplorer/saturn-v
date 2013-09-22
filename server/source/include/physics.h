/*******************************************************************

Projekt Saturn V Main Computer

Plik: physics.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef PHYSICS_H
#define PHYSICS_H

double _PHYSICS_get_orbit_mean_motion( double semi_major_axis );
double _PHYSICS_get_orbit_periapsis_velocity( double apoapsis, double periapsis );
double _PHYSICS_get_orbit_apoapsis_velocity( double apoapsis, double periapsis );
double _PHYSICS_get_orbit_revolution_period( double semi_major_axis );
double _PHYSICS_get_orbit_circumference( double semi_major_axis, double semi_minor_axis );
double _PHYSICS_get_orbit_eccentrity( double altitude, double velocity );
double _PHYSICS_get_orbit_semi_minor_axis( double semi_major_axis, double eccentrity );
double _PHYSICS_get_orbit_semi_major_axis( double altitude, double velocity );
double _PHYSICS_get_orbit_perigee( double semi_major_axis, double eccentrity );
double _PHYSICS_get_orbit_apogee( double semi_major_axis, double eccentrity );
double _PHYSICS_get_orbit_inclination( double latitude, double current_roll );
double _PHYSICS_get_dynamic_pressure_force( double altitude );
double PHYSICS_IGM_get_pitch_step( void );

#endif