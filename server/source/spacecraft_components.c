/*******************************************************************

Projekt Saturn V Main Computer

Plik: spacecraft_components.c

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/server_shared.h"
#include "include/spacecraft_components.h"


/* Program komputerowy */
void COMPUTER_PROGRAM_start( COMPUTER_PROGRAM *cp ) {
	cp->start_time = get_current_epoch();
	cp->running = 1;
}

void COMPUTER_PROGRAM_stop( COMPUTER_PROGRAM *cp ) {
	cp->running = 0;
}

/* Cz³on rakiety */
int ROCKET_STAGE_get_burn_time( ROCKET_STAGE *rs ) {
	return ( rs->burn_start > 0 ? rs->burn_time : 0 );
}

int ROCKET_STAGE_get_fuel( ROCKET_STAGE *rs ) {
	return rs->fuel;
}

int ROCKET_STAGE_get_total_mass( ROCKET_STAGE *rs ) {
	return ( rs->fuel + rs->dry_mass + rs->instrument_mass + rs->interstage_mass );
}

short ROCKET_STAGE_get_attached( ROCKET_STAGE *rs ) {
	return rs->attached;
}

void ROCKET_STAGE_do_attach( ROCKET_STAGE *rs ) {
	rs->attached = 1;
}

void ROCKET_STAGE_do_detach( ROCKET_STAGE *rs ) {
	rs->attached = 0;
	rs->current_thrust = 0;
	rs->staging_time = telemetry_data.mission_time;
}

void ROCKET_STAGE_set_fuel( ROCKET_STAGE *rs, long double value, short do_mod ) {
	rs->fuel = value;

	if( rs->fuel < 0 ) {
		rs->fuel = 0;
	}
}

ROCKET_STAGE *ROCKET_STAGE_get_active( void ) {
	if( ROCKET_STAGE_get_attached( &system_s1 ) == 0) {
		if( ROCKET_STAGE_get_attached( &system_s2 ) == 1 ) {
			return &system_s2;
		}
	}

	if( ROCKET_STAGE_get_attached( &system_s2 ) == 0) {
		if( ROCKET_STAGE_get_attached( &system_s3 ) == 1 ) {
			return &system_s3;
		}
	}

	if( ( ROCKET_STAGE_get_attached( &system_s1 ) == 0 ) && ( ROCKET_STAGE_get_attached( &system_s2 ) == 0 ) && ( ROCKET_STAGE_get_attached( &system_s3 ) == 0 ) ) {
		return &system_null;
	}

	return &system_s1;
}

ROCKET_STAGE *ROCKET_STAGE_get_by_id( short id ) {
	switch( id ) {
		case 1 : { return &system_s1; } break;
		case 2 : { return &system_s2; } break;
		case 3 : { return &system_s3; } break;
		default : { return &system_null; } break;
	}

	return &system_null;
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
	system_s1.staging_time = -1;
	system_s1.dry_mass = 130422;//132890;
	system_s1.max_fuel = 2145798;//2106905 + 20311;
	system_s1.instrument_mass = 2469;//7674;//5206;
	system_s1.interstage_mass = 0;//5205;
	system_s1.max_fuel_burn = 13232;
	system_s1.max_thrust_n = 39782855;
	system_s1.current_thrust = 0;
	system_s1.variable_thrust = 1;
	system_s1.initial_thrust = 34517594;
	system_s1.thrust_step = 33387;
	system_s1.burn_start = 0;
	system_s1.burn_time = 0;
	system_s1.center_engine_available = 1;

	strncpy( system_s1.name, "S-IC", MICRO_BUFF_SIZE );

	current_system = &system_s1;

	system_s1.attached = 1;
	system_s1.fuel = system_s1.max_fuel;

	printf( "done.\n" );

}

void STAGE_2_init( void ) {

	printf( "Initializing STAGE S-II..." );

	system_s2.id = 2;
	system_s2.fuel = 0;
	system_s2.attached = 0;
	system_s2.staging_time = -1;
	system_s2.dry_mass = 36157;//36729;
	system_s2.max_fuel = 443235;
	system_s2.instrument_mass = 5205;//571;//4234;//3663;
	system_s2.interstage_mass = 3663;
	system_s2.max_fuel_burn = 1224;
	system_s2.max_thrust_n = 5095279;
	system_s2.variable_thrust = 1;
	system_s2.current_thrust = 0;
	system_s2.initial_thrust = 5095184;
	system_s2.thrust_step = 12;
	system_s2.burn_start = 0;
	system_s2.burn_time = 0;
	system_s2.center_engine_available = 1;

	strncpy( system_s2.name, "S-II", MICRO_BUFF_SIZE );

	system_s2.attached = 1;
	system_s2.fuel = system_s2.max_fuel;

	printf( "done.\n" );

}

void STAGE_3_init( void ) {

	printf( "Initializing STAGE S-IVB..." );

	system_s3.id = 3;
	system_s3.fuel = 0;
	system_s3.attached = 0;
	system_s3.staging_time = -1;
	system_s3.dry_mass = 11272;//11943;
	system_s3.max_fuel = 107095;
	/*
	Spacecraft/Lunar Module Adapter: 3,951 lbm
	Lunar Module: 33,278 lbm
	Command and Service Module: 63,507 lbm
	Total Launch Escape System: 8,910 lbm
	Total Spacecraft (CSM): 109,646 lbm
	*/
	system_s3.instrument_mass = 49734;//45693+1939;
	system_s3.interstage_mass = 571;
	system_s3.max_fuel_burn = 213;
	system_s3.max_thrust_n = 901223;
	system_s3.current_thrust = 0;
	system_s3.variable_thrust = 0;
	system_s3.initial_thrust = 0;
	system_s3.thrust_step = 0;
	system_s3.burn_start = 0;
	system_s3.burn_time = 0;
	system_s3.center_engine_available = 0;

	strncpy( system_s3.name, "S-IVB", MICRO_BUFF_SIZE );

	system_s3.attached = 1;
	system_s3.fuel = system_s3.max_fuel;

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
