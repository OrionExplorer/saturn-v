/*******************************************************************

Projekt Saturn V Main Computer

Plik: auto_pilot.c

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/shared.h"
#include "include/spacecraft_components.h"
#include <stdlib.h>

double					pitch_modifier = 0.0;
short					liftoff_yaw_achieved = 0;

void AUTOPILOT_init( void ) {
	pitch_modifier = ( ( double )rand() / ( double )RAND_MAX ) / 1380;
}

double _AUTOPILOT_get_pitch_step( void ) {
	int seconds = round( telemetry_data.mission_time );
	double result = 0.0662500;

	if( seconds >= 80 ) {
		result -= result;
	}

	if(seconds >= 31 && seconds < 60) {
		result += 1.1525;
	}
	if(seconds >= 60 && seconds < 80) {
		result += 1.285;
	}
	if(seconds >= 80 && seconds < 110) {
		result += 0.465;
	}

	return ( result + pitch_modifier );
}

double _AUTOPILOT_get_roll_step( void ) {
	return 1.1;
}

double _AUTOPILOT_get_yaw_step( void ) {
	if( yaw_program.current_value >= yaw_program.dest_value ) {
		liftoff_yaw_achieved = 1;
	}

	if( telemetry_data.current_altitude < 130 && liftoff_yaw_achieved == 0 ) {
		return 0.16625;
	} else if( telemetry_data.current_altitude > 130 ) {
		return -0.10625;
	}

	return 0;
}

void AUTOPILOT_progress( double real_second ) {
	int second = round( real_second );

	if( yaw_program.current_value <= 0 && liftoff_yaw_achieved == 1 && yaw_program.running == 1 ) {
		MAIN_COMPUTER_exec( YAW_PROGRAM, STOP, 0 );
	}

	if( ( 90 - roll_program.current_value ) <= roll_program.dest_value && roll_program.running == 1 ) {
		MAIN_COMPUTER_exec( ROLL_PROGRAM, STOP, 0 );
	}

	if( pitch_program.current_value >= pitch_program.dest_value && pitch_program.running == 1 ) {
		MAIN_COMPUTER_exec( PITCH_PROGRAM, STOP, 0 );
	}

	if( system_s1.center_engine_available == 1 && telemetry_data.current_velocity >= 1970 ) {
		MAIN_COMPUTER_exec( S1, CENTER_ENGINE_CUTOFF, 0 );
	}

	if( telemetry_data.current_velocity >= 2750 && system_s1.attached == 1 ) {
		MAIN_COMPUTER_exec( THRUST, NULL_THRUST, 0 );
		MAIN_COMPUTER_exec( MAIN_ENGINE, STOP, 0 );
		MAIN_COMPUTER_exec( S1, DETACH, 0 );
	}

	if( system_s2.center_engine_available == 1 && telemetry_data.current_velocity >= 5690 ) {
		MAIN_COMPUTER_exec( S2, CENTER_ENGINE_CUTOFF, 0 );
	}

	if( system_s2.attached == 1 && ROCKET_STAGE_get_fuel( &system_s2 ) <= 5880 ) {
		MAIN_COMPUTER_exec( THRUST, NULL_THRUST, 0 );
		MAIN_COMPUTER_exec( MAIN_ENGINE, STOP, 0 );
		MAIN_COMPUTER_exec( S2, DETACH, 0 );
	}

	if( current_system->id == system_s2.id  && system_s2.attached == 1 && system_s1.attached == 0 && system_s1.burn_start > 0 && ( telemetry_data.mission_time - system_s1.staging_time ) >= 4  && ( telemetry_data.mission_time - system_s1.staging_time ) <= 6 ) {
		if( ROCKET_ENGINE_get_thrust( &internal_guidance ) == 0 ) {
			MAIN_COMPUTER_exec( MAIN_ENGINE, START, 0 );
			MAIN_COMPUTER_exec( THRUST, FULL_THRUST, 0 );
		}
	}

	if( current_system->id == system_s3.id && system_s3.attached == 1 && system_s2.attached == 0 && system_s2.burn_start > 0 && ( telemetry_data.mission_time - system_s2.staging_time ) >= 4 && ( telemetry_data.mission_time - system_s2.staging_time ) <= 6 ) {
		if( ROCKET_ENGINE_get_thrust( &internal_guidance ) == 0 ) {
			MAIN_COMPUTER_exec( MAIN_ENGINE, START, 0 );
			MAIN_COMPUTER_exec( THRUST, FULL_THRUST, 0 );
		}
	}

	if( current_system->id == system_s3.id && ( current_system->burn_time > 141.5 && current_system->burn_time < 142.0 ) && ROCKET_ENGINE_get_thrust( &internal_guidance ) > 0 ) {
		if( ROCKET_ENGINE_get_thrust( &internal_guidance ) == 100 ) {
			MAIN_COMPUTER_exec( THRUST, NULL_THRUST, 0 );
			MAIN_COMPUTER_exec( MAIN_ENGINE, STOP, 0 );
		}
	}

	switch( second ) {
		default : /* Nic */ break;
		case -10 : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 ) {
				MAIN_COMPUTER_exec( INTERNAL_GUIDANCE, START, 0 );
			}
		} break;
		case -8 : {
			if( ROCKET_ENGINE_get_engaged( &main_engine ) == 0 ) {
				MAIN_COMPUTER_exec( MAIN_ENGINE, START, 0 );
			}
		} break;
		case 0 : {
			if( telemetry_data.holddown_arms_released == 0 ) {
				MAIN_COMPUTER_exec( HOLDDOWN_ARMS, STOP, 0 );
			}
		} break;
		case 2 : {
			if( yaw_program.running == 0 ) {
				MAIN_COMPUTER_exec( YAW_PROGRAM, START, 0 );
			}
		} break;

		case 13 : {
			if( roll_program.running == 0 ) {
				MAIN_COMPUTER_exec( ROLL_PROGRAM, START, 0 );
			}
		} break;

		case 31 : {
			if( pitch_program.running == 0 ) {
				MAIN_COMPUTER_exec( PITCH_PROGRAM, START, 0 );
			}
		} break;

		case 192 : {
			if( system_s2.interstage_mass > 0 ) {
				MAIN_COMPUTER_exec( S2, INTERSTAGE_JETTISON, 0 );
			}
		} break;

		case 197 : {
			if( telemetry_data.launch_escape_tower_ready == 1 ) {
				MAIN_COMPUTER_exec( LET, JETTISON, 0 );
			}
		} break;

		case 500 : {
			if( ROCKET_ENGINE_get_thrust( &internal_guidance ) > 70 ) {
				MAIN_COMPUTER_exec( THRUST, DECREASE, 5 );
			}
		} break;

		/*case 695 : {
			if( ROCKET_ENGINE_get_thrust( &internal_guidance ) == 100 ) {
				MAIN_COMPUTER_exec( THRUST, NULL_THRUST, 0 );
				MAIN_COMPUTER_exec( MAIN_ENGINE, STOP, 0 );
			}
		} break;*/
	}
}
