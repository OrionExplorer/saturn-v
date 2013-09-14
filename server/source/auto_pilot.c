/*******************************************************************

Projekt Saturn V Main Computer

Plik: auto_pilot.c

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/server_shared.h"
#include "include/spacecraft_components.h"
#include <stdlib.h>

double					pitch_modifier = 0.0;
short					liftoff_yaw_achieved = 0;

void AUTOPILOT_init( void ) {
	pitch_modifier = ( (double)rand()/(double)RAND_MAX ) / 1380;
}

double _AUTOPILOT_get_pitch_step( void ) {
	int seconds = round(pitch_program.running_time);
	double result = 0.0;

	if( seconds >= 0 && seconds < 30 ) {
		result = 0.767;
	}
	if( seconds >= 30 && seconds < 40 ) {
		result = 0.885;
	}
	if( seconds >= 40 && seconds < 70 ) {
		result = 0.85;
	}
	if( seconds >= 70 && seconds < 150  ) {
		result = 0.1618333;
	}
	if( seconds >= 150 && seconds < 230 ) {
		result = 0.1659900;
	}
	if( seconds >= 230 && seconds < 300 ) {
		result = 0.004;
	}
	if( seconds >= 300 && seconds < 400 ) {
		result = 0.035;
	}
	if( seconds > 400) {
		result = 0.041;
	}

	return ( result + pitch_modifier );
}

double _AUTOPILOT_get_roll_step( void ) {
	return 0.94;
}

double _AUTOPILOT_get_yaw_step( void ) {
	if( yaw_program.current_value >= yaw_program.dest_value ) {
		liftoff_yaw_achieved = 1;
	}

	if( telemetry_data.current_altitude < 130 && liftoff_yaw_achieved == 0 ) {
		return 0.15625;
	} else if( telemetry_data.current_altitude > 130 ) {
		return -0.10625;
	}

	return 0;
}

void AUTOPILOT_progress( double real_second ) {
	int second = round( real_second );

	if( telemetry_data.stable_orbit_achieved == 1 ) {
		if( ROCKET_ENGINE_get_thrust( &internal_guidance ) == 100 ) {
			EXEC_COMMAND( THRUST, NULL_THRUST, 0 );
			EXEC_COMMAND( MAIN_ENGINE, STOP, 0 );
		}
		return;
	}

	if( yaw_program.current_value <= 0 && liftoff_yaw_achieved == 1 && yaw_program.running == 1 ) {
		EXEC_COMMAND( YAW_PROGRAM, STOP, 0 );
	}

	if( ( 90 - roll_program.current_value ) <= roll_program.dest_value && roll_program.running == 1 ) {
		EXEC_COMMAND( ROLL_PROGRAM, STOP, 0 );
	}

	if( pitch_program.current_value >= pitch_program.dest_value && pitch_program.running == 1 ) {
		EXEC_COMMAND( PITCH_PROGRAM, STOP, 0 );
	}

	if( system_s1.center_engine_available == 1 && telemetry_data.current_velocity >= 1970 ) {
		EXEC_COMMAND( S1, CENTER_ENGINE_CUTOFF, 0 );
	}

	if( telemetry_data.current_velocity >= 2750 && system_s1.attached == 1 ) {
		EXEC_COMMAND( THRUST, NULL_THRUST, 0 );
		EXEC_COMMAND( MAIN_ENGINE, STOP, 0 );
		EXEC_COMMAND( S1, DETACH, 0 );
	}

	if( system_s2.center_engine_available == 1 && telemetry_data.current_velocity >= 5678 ) {
		EXEC_COMMAND( S2, CENTER_ENGINE_CUTOFF, 0 );
	}

	if( telemetry_data.current_velocity >= 7000 && system_s2.attached == 1 ) {
		EXEC_COMMAND( THRUST, NULL_THRUST, 0 );
		EXEC_COMMAND( MAIN_ENGINE, STOP, 0 );
		EXEC_COMMAND( S2, DETACH, 0 );
	}

	if( current_system->id == system_s2.id  && system_s2.attached == 1 && system_s1.attached == 0 && system_s1.burn_start > 0 && ( telemetry_data.mission_time - system_s1.staging_time ) >= 4 ) {
		if( ROCKET_ENGINE_get_thrust( &internal_guidance ) == 0 ) {
			EXEC_COMMAND( MAIN_ENGINE, START, 0 );
			EXEC_COMMAND( THRUST, FULL_THRUST, 0 );
		}
	}

	if( current_system->id == system_s3.id && system_s3.attached == 1 && system_s2.attached == 0 && system_s2.burn_start > 0 && ( telemetry_data.mission_time - system_s2.staging_time ) >= 4 ) {
		if( ROCKET_ENGINE_get_thrust( &internal_guidance ) == 0 ) {
			EXEC_COMMAND( MAIN_ENGINE, START, 0 );
			EXEC_COMMAND( THRUST, FULL_THRUST, 0 );
		}
	}

	switch( second ) {
		default : /* Nic */ break;
		case -10 : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 ) {
				EXEC_COMMAND( INTERNAL_GUIDANCE, START, 0 );
			}
		} break;
		case -8 : {
			if( ROCKET_ENGINE_get_engaged( &main_engine ) == 0 ) {
				EXEC_COMMAND( MAIN_ENGINE, START, 0 );
			}
		} break;
		case 0 : {
			if( telemetry_data.holddown_arms_released == 0 ) {
				EXEC_COMMAND( HOLDDOWN_ARMS, STOP, 0 );
			}
		} break;
		case 2 : {
			if( yaw_program.running == 0 ) {
				EXEC_COMMAND( YAW_PROGRAM, START, 0 );
			}
		} break;

		case 13 : {
			if( roll_program.running == 0 ) {
				EXEC_COMMAND( ROLL_PROGRAM, START, 0 );
			}
		} break;

		case 15 : {
			if( pitch_program.running == 0 ) {
				EXEC_COMMAND( PITCH_PROGRAM, START, 0 );
			}
		} break;

		case 192 : {
			if( system_s2.interstage_mass > 0 ) {
				EXEC_COMMAND( S2, INTERSTAGE_JETTISON, 0 );
			}
		} break;

		case 197 : {
			if( telemetry_data.launch_escape_tower_ready == 1 ) {
				EXEC_COMMAND( LET, JETTISON, 0 );
			}
		} break;

		case 500 : {
			if( ROCKET_ENGINE_get_thrust( &internal_guidance ) > 60 ) {
				EXEC_COMMAND( THRUST, DECREASE, 20 );
			}
		} break;
	}
}
