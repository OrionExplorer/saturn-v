/*******************************************************************

Projekt Voyager 7 Board Computer

Plik: spacecraft_components.c

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/server_shared.h"
#include "include/spacecraft_components.h"


/* Program komputerowy */
char *COMPUTER_PROGRAM_get_possible_action( COMPUTER_PROGRAM *cp ) {
	return ( cp->running ? STOP_STR : START_STR );
}

void COMPUTER_PROGRAM_start( COMPUTER_PROGRAM *cp ) {
	cp->start_time = get_current_epoch();
	cp->running = 1;
}

void COMPUTER_PROGRAM_stop( COMPUTER_PROGRAM *cp ) {
	cp->running = 0;
	cp->running_time = 0;
}

/* Cz³on rakiety */
int ROCKET_STAGE_get_burn_time( ROCKET_STAGE *rs ) {
	return ( rs->burn_start > 0 ? rs->burn_time : 0 );
}

int ROCKET_STAGE_get_fuel( ROCKET_STAGE *rs ) {
	return rs->fuel;
}

int ROCKET_STAGE_get_total_mass( ROCKET_STAGE *rs ) {
	return ( rs->fuel + rs->dry_mass + rs->instrument_mass );
}

char* ROCKET_STAGE_get_fuel_possible_action( ROCKET_STAGE *rs ) {
	return ( rs->fuel < rs->max_fuel ? TANK_STR : RELEASE_STR );
}

short ROCKET_STAGE_get_attached( ROCKET_STAGE *rs ) {
	return rs->attached;
}

void ROCKET_STAGE_do_attach( ROCKET_STAGE *rs ) {
	rs->attached = 1;
}

void ROCKET_STAGE_do_detach( ROCKET_STAGE *rs ) {
	rs->attached = 0;
}

void ROCKET_STAGE_set_fuel( ROCKET_STAGE *rs, long double value ) {
	rs->fuel = value;
}

/* Silnik */
void ROCKET_ENGINE_do_engage( ROCKET_ENGINE *re ) {
	re->engaged = 1;
}

void ROCKET_ENGINE_do_disengage( ROCKET_ENGINE *re ) {
	re->engaged = 0;
}

int ROCKET_ENGINE_get_thrust( ROCKET_ENGINE *re ) {
	return re->thrust;
}

void ROCKET_ENGINE_set_thrust( ROCKET_ENGINE *re, int value ) {
	re->thrust = value;
}

char* ROCKET_ENGINE_get_possible_action( ROCKET_ENGINE *re ) {
	return ( re->engaged == 1 ? DISENGAGE_STR : ENGAGE_STR );
}

short ROCKET_ENGINE_get_engaged( ROCKET_ENGINE *re ) {
	return re->engaged;
}

/**
Człony rakiety
**/
void STAGE_1_init( void ) {

	printf( "Initializing STAGE S-IC..." );

	system_s1.id = 1;
	system_s1.fuel = 0;
	system_s1.attached = 0;
	system_s1.dry_mass = 132890;
	system_s1.max_fuel = 2106905 + 20311;
	system_s1.instrument_mass = 5206;
	system_s1.max_fuel_burn = 13232;
	system_s1.max_thrust_n = 39782855;
	system_s1.variable_thrust = 1;
	system_s1.initial_thrust = 34517594;
	system_s1.thrust_step = 33387;
	system_s1.burn_start = 0;
	system_s1.burn_time = 0;
	system_s1.center_engine_available = 1;

	current_system = &system_s1;

	printf( "done.\n" );

}

void STAGE_2_init( void ) {

	printf( "Initializing STAGE S-II..." );

	system_s2.id = 2;
	system_s2.fuel = 0;
	system_s2.attached = 0;
	system_s2.dry_mass = 36729;
	system_s2.max_fuel = 443235;
	system_s2.instrument_mass = 3663;
	system_s2.max_fuel_burn = 1224;
	system_s2.max_thrust_n = 5095279;
	system_s2.variable_thrust = 1;
	system_s2.initial_thrust = 5095184;
	system_s2.thrust_step = 12;
	system_s2.burn_start = 0;
	system_s2.burn_time = 0;
	system_s2.center_engine_available = 1;

	printf( "done.\n" );

}

void STAGE_3_init( void ) {

	printf( "Initializing STAGE S-IVB..." );

	system_s3.id = 3;
	system_s3.fuel = 0;
	system_s3.attached = 0;
	system_s3.dry_mass = 12024;
	system_s3.max_fuel = 107095;
	system_s3.instrument_mass = 45693+4200;
	system_s3.max_fuel_burn = 213;
	system_s3.max_thrust_n = 901223;
	system_s3.variable_thrust = 0;
	system_s3.initial_thrust = 0;
	system_s3.thrust_step = 0;
	system_s3.burn_start = 0;
	system_s3.burn_time = 0;
	system_s3.center_engine_available = 0;

	printf( "done.\n" );

}

void STAGE_null_init( void ) {

	system_null.id = -1;
	system_null.max_fuel = 0;
	system_null.dry_mass = 0;
	system_null.instrument_mass = 0;
	system_null.max_fuel_burn = 0;
	system_null.variable_thrust = 0;
	system_null.max_thrust_n = 0;

}

void STAGES_init( void ) {

	STAGE_1_init();
	STAGE_2_init();
	STAGE_3_init();
	STAGE_null_init();

}

/**
Programy komputerowe
**/
void PITCH_init( void ) {

	printf( "Initializing PITCH PROGRAM..." );
	strncpy( pitch_program.name, "PITCH PROGRAM", SMALL_BUFF_SIZE );

	pitch_program.current_value = 0.0;
	pitch_program.dest_value = 90.0;

	printf( "done.\n" );

}

void ROLL_init( void ) {

	printf( "Initializing ROLL PROGRAM...");
	strncpy( roll_program.name, "ROLL PROGRAM", SMALL_BUFF_SIZE );

	roll_program.current_value = 0.0;
	roll_program.dest_value = 72.0;

	printf( "done.\n" );

}

void YAW_init( void ) {

	printf( "Initializing YAW PROGRAM..." );
	strncpy( yaw_program.name, "YAW PROGRAM", SMALL_BUFF_SIZE );

	yaw_program.current_value = 0.0;
	yaw_program.dest_value = 1.2;

	printf( "done.\n" );
}

void COMPUTER_PROGRAMS_init( void ) {

	PITCH_init();
	ROLL_init();
	YAW_init();

}
