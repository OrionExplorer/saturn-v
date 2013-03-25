/*******************************************************************

Projekt Saturn V Main Computer

Plik: main_computer.c

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/main_computer.h"
#include "include/server_log.h"
#include "include/spacecraft_components.h"
#include "include/celestial_objects.h"
#include "include/server_telemetry.h"
#include "include/auto_pilot.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#ifdef _WIN32
	#include <windows.h>
#endif

/**
Człony rakiety
**/
extern ROCKET_STAGE		*current_system;

/***************************************
Zmienne globalne
***************************************/
int						current_dynamic_pressure = 0;
long double				fw;
long double				real_fw;
short					computing_all = 0;

/**
Modyfikatory fizyki
**/
double					normal_atmospheric_pressure = 1013.25;
int						time_interval = 100;
double					rad2deg;
double					time_tick;
double					time_mod = 0.0;

pthread_t				sthread;

void *SIMULATION_progress( void ) {
	srand ( time(NULL) );

	while(1) {
		PHYSICS_shared_calculations();

		switch( MAIN_FLIGHT_STATUS ) {
			case LAUNCH_FROM_EARTH : PHYSICS_launch_calculations(); break;
			case STABLE_ORBIT : { /*TODO*/ } break;
			case TRANSFER_ORBIT : { /*TODO*/ } break;
			default : break;
		}

		PHYSICS_instrument_unit_calculations();

		Sleep( 100 );
	}
}

void MAIN_COMPUTER_init( void ) {
	vDEVICE device;
	vCOMMAND command;
	int value;
	INTERPRETER_RESULT *result;

	srand ( time(NULL) );

	printf( "Initializing MAIN COMPUTER...\n");
	STAGES_init();
	COMPUTER_PROGRAMS_init();
	CELESTIAL_OBJECTS_load();

	printf( "MAIN COMPUTER initialized.\n");

	EXEC_COMMAND( S1, ATTACH, 0 );
	EXEC_COMMAND( S1, TANK, 0 );
	EXEC_COMMAND( S2, ATTACH, 0 );
	EXEC_COMMAND( S2, TANK, 0 );
	EXEC_COMMAND( S3, ATTACH, 0 );
	EXEC_COMMAND( S3, TANK, 0 );
	EXEC_COMMAND( AUTO_PILOT, START, 0 );

	time_mod = ( 1000 / time_interval );
	normal_atmospheric_pressure += rand() % 10;
	time_tick = ( time_interval * 0.001 );
	telemetry_data.mission_time = -20.0;
	AUTOPILOT_init();

	MAIN_FLIGHT_STATUS = LAUNCH_FROM_EARTH;
	/* Uruchomienie sieci */
	SOCKET_main();

	while( scanf("%d %d %d", ( int * )&device, ( int * )&command, &value ) == 3) {
		result = EXEC_COMMAND( device, command, value );
		printf("%s\n", result->message );
	}
}

/**
Interpreter komend komputera
**/
INTERPRETER_RESULT* EXEC_COMMAND( vDEVICE device, vCOMMAND command, const int value ) {
	static INTERPRETER_RESULT interpreter_result;
	short success = 0;
	char message[ BIG_BUFF_SIZE ];

	interpreter_result.success = 0;
	strncpy( message, "ERROR: UNKNOWN COMMAND", BIG_BUFF_SIZE );
	memset( interpreter_result.message, '\0', BIG_BUFF_SIZE );

	switch( device ) {
		default : /* Nic */ break;
		case INTERNAL_GUIDANCE : {
			switch( command ) {
				default : /* Nic */ break;

				case START : {
					if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 1 ) {
						success = 0;
						strncpy( message, "ERROR: INTERNAL GUIDANCE IS ENGAGED", BIG_BUFF_SIZE );
					} else {
						if( ROCKET_ENGINE_get_engaged( &main_engine ) == 0 && ROCKET_STAGE_get_attached( &system_s1 ) == 1 && ROCKET_STAGE_get_fuel( &system_s1) > 0 ) {
							ROCKET_ENGINE_do_engage( &internal_guidance );
							success = 1;
							strncpy( message, "GUIDANCE IS INTERNAL", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO ENGAGE INTERNAL GUIDANCE. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					}
				} break;

				case STOP : {
					if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 ) {
						success = 0;
						strncpy( message, "ERROR: INTERNAL GUIDANCE IS DISENGAGED", BIG_BUFF_SIZE );
					} else {
						if( ROCKET_ENGINE_get_engaged( &main_engine ) == 0 && ROCKET_ENGINE_get_thrust( &internal_guidance) == 0 && current_system->burn_time == 0 ) {
							ROCKET_ENGINE_do_disengage( &internal_guidance );
							success = 1;
							strncpy( message, "GUIDANCE IS EXTERNAL", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO SWITCH TO EXTERNAL GUIDANCE. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					}
				} break;
			}
		} break;

		case S1 : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 1 && ROCKET_ENGINE_get_engaged( &main_engine ) == 1&& command != CENTER_ENGINE_CUTOFF ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO PERFORM ANY S-IC OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case TANK : {
						if( (ROCKET_STAGE_get_fuel( &system_s1 ) < system_s1.max_fuel) && system_s1.attached == 1 ) {
							ROCKET_STAGE_set_fuel( &system_s1, system_s1.max_fuel, 0 );
							success = 1;
							strncpy( message, "S-IC STAGE PROPELLANT IS LOADED", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IC PROPELLANT LOADING OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					} break;

					case ATTACH : {
						if( telemetry_data.current_altitude > 0 ) {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IC ATTACH OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						} else {
							if( ROCKET_STAGE_get_attached( &system_s1 ) == 0 ) {
								ROCKET_STAGE_do_attach( &system_s1 );
								success = 1;
								strncpy( message, "S-IC STAGE ATTACHED", BIG_BUFF_SIZE );
							} else {
								success = 0;
								strncpy( message, "ERROR: S-IC STAGE IS ATTACHED", BIG_BUFF_SIZE );
							}
						}
					} break;

					case DETACH : {
						if( telemetry_data.current_altitude == 0 ) {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IC DETACH OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						} else {
							if( ROCKET_STAGE_get_attached( &system_s1 ) == 1 ) {
								ROCKET_STAGE_do_detach( &system_s1 );
								success = 1;
								strncpy( message, "S-IC STAGED", BIG_BUFF_SIZE );
							} else {
								success = 0;
								strncpy( message, "ERROR: S-IC STAGE IS STAGED", BIG_BUFF_SIZE );
							}
						}
					} break;

					case CENTER_ENGINE_CUTOFF : {
						if( system_s1.center_engine_available == 1 && telemetry_data.current_altitude > 0 ) {
							ROCKET_ENGINE_set_thrust( &main_engine, ROCKET_ENGINE_get_thrust( &main_engine ) - 20 );
							system_s1.center_engine_available = 0;
							success = 1;
							strncpy( message, "S-IC CENTER ENGINE CUTOFF", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IC CENTER ENGINE CUTOFF. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					} break;
				}
			}
		} break;

		case S2 : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) && ROCKET_ENGINE_get_engaged( &main_engine ) == 1 && command != CENTER_ENGINE_CUTOFF ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO PERFORM ANY S-II OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case TANK : {
						if( ROCKET_STAGE_get_fuel( &system_s2 ) < system_s2.max_fuel && system_s2.attached == 1 ) {
							ROCKET_STAGE_set_fuel( &system_s2, system_s2.max_fuel, 0 );
							success = 1;
							strncpy( message, "S-II STAGE PROPELLANT IS LOADED", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-II PROPELLANT LOADING OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					} break;

					case ATTACH : {
						if( telemetry_data.current_altitude > 0 ) {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-II ATTACH OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						} else {
							if( ROCKET_STAGE_get_attached( &system_s2 ) == 0 ) {
								ROCKET_STAGE_do_attach( &system_s2 );
								success = 1;
								strncpy( message, "S-II STAGE ATTACHED", BIG_BUFF_SIZE );
							} else {
								success = 0;
								strncpy( message, "ERROR: S-II STAGE IS ATTACHED", BIG_BUFF_SIZE );
							}
						}
					} break;

					case DETACH : {
						if( telemetry_data.current_altitude == 0 ) {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-II DETACH OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						} else {
							if( ROCKET_STAGE_get_attached( &system_s2 ) == 1 ) {
								ROCKET_STAGE_do_detach( &system_s2 );
								success = 1;
								strncpy( message, "S-II STAGED", BIG_BUFF_SIZE );
							} else {
								success = 0;
								strncpy( message, "ERROR: S-II STAGE IS STAGED", BIG_BUFF_SIZE );
							}
						}
					} break;

					case CENTER_ENGINE_CUTOFF : {
						if( system_s2.center_engine_available == 1 && telemetry_data.current_altitude > 0 ) {
							ROCKET_ENGINE_set_thrust( &main_engine, ROCKET_ENGINE_get_thrust( &main_engine ) - 20 );
							system_s2.center_engine_available = 0;
							success = 1;
							strncpy( message, "S-II CENTER ENGINE CUTOFF", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-II CENTER ENGINE CUTOFF. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					} break;
				}
			}
		} break;

		case S3 : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 1 && ROCKET_ENGINE_get_engaged( &main_engine ) == 1 && command != CENTER_ENGINE_CUTOFF ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO PERFORM ANY S-IVB OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case TANK : {
						if( ROCKET_STAGE_get_fuel( &system_s3 ) < system_s3.max_fuel && system_s3.attached == 1 ) {
							ROCKET_STAGE_set_fuel( &system_s3, system_s3.max_fuel, 0 );
							success = 1;
							strncpy( message, "S-IVB STAGE PROPELLANT IS LOADED", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IVB PROPELLANT LOADING OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					} break;

					case ATTACH : {
						if( telemetry_data.current_altitude > 0 ) {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IVB ATTACH OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						} else {
							if( ROCKET_STAGE_get_attached( &system_s3 ) == 0 ) {
								ROCKET_STAGE_do_attach( &system_s3 );
								success = 1;
								strncpy( message, "S-IVB STAGE ATTACHED", BIG_BUFF_SIZE );
							} else {
								success = 0;
								strncpy( message, "ERROR: S-IVB STAGE IS ATTACHED", BIG_BUFF_SIZE );
							}
						}
					} break;

					case DETACH : {
						if( telemetry_data.current_altitude == 0 ) {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IVB DETACH OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						} else {
							if( ROCKET_STAGE_get_attached( &system_s3 ) == 1 ) {
								ROCKET_STAGE_do_detach( &system_s3 );
								success = 1;
								strncpy( message, "S-IVB STAGED", BIG_BUFF_SIZE );
							} else {
								success = 0;
								strncpy( message, "ERROR: S-IVB STAGE IS STAGED", BIG_BUFF_SIZE );
							}
						}
					} break;

					case RESTART : {
						if( current_system->id == system_s3.id && telemetry_data.current_altitude > 0 ) {
							system_s3.burn_start = 0;
							system_s3.burn_time = 0;
							success = 1;
							strncpy( message, "S-IVB BURN RESTART", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO RESTART S-IVB BURN. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					} break;
				}
			}
		} break;

		case MAIN_ENGINE : {
			switch( command ) {
				default : /* Nic */ break;
				case START : {
					if( ROCKET_ENGINE_get_engaged( &main_engine ) == 1 ) {
						success = 0;
						strncpy( message, "ERROR: MAIN ENGINE IS ENGAGED", BIG_BUFF_SIZE );
					} else {
						if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 1 && ROCKET_ENGINE_get_thrust( &internal_guidance ) == 0 ) {
							ROCKET_ENGINE_do_engage( &main_engine );
							success = 1;
							memset( message, '\0', BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO START MAIN ENGINE. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					}
				} break;

				case STOP : {
					if( ROCKET_ENGINE_get_engaged( &main_engine ) == 0 ) {
						success = 0;
						strncpy( message, "ERROR: MAIN ENGINE IS DISENGAGED", BIG_BUFF_SIZE );
					} else {
						if( ROCKET_ENGINE_get_thrust( &main_engine ) == 0 ) {
							ROCKET_ENGINE_do_disengage( &main_engine );
							success = 1;
							memset( message, '\0', BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO STOP MAIN ENGINE. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					}
				} break;
			}
		} break;

		case THRUST : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 && ROCKET_ENGINE_get_engaged( &main_engine ) == 0) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO CHANGE THRUST. CHECK CONFIGURATION", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case FULL_THRUST : {
						if( ROCKET_ENGINE_get_engaged( &main_engine ) == 1 ) {
							ROCKET_ENGINE_set_thrust( &main_engine, MAX_THRUST );
							success = 1;
							memset( message, '\0', BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: MAIN ENGINE SYSTEM IS IDLE", BIG_BUFF_SIZE );
						}
					} break;

					case NULL_THRUST : {
						if( ROCKET_ENGINE_get_engaged( &main_engine ) == 1 ) {
							ROCKET_ENGINE_set_thrust( &main_engine, 0 );
							success = 1;
							memset( message, '\0', BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: MAIN ENGINE SYSTEM IS IDLE", BIG_BUFF_SIZE );
						}
					} break;

					case INCREASE : {
						if( ROCKET_ENGINE_get_engaged( &main_engine ) == 1 ) {
							if( ROCKET_ENGINE_get_thrust( &main_engine) + value <= MAX_THRUST ) {
								ROCKET_ENGINE_set_thrust( &main_engine, ROCKET_ENGINE_get_thrust( &main_engine ) + value );
								success = 1;
								memset( message, '\0', BIG_BUFF_SIZE );
							} else {
								success = 0;
								sprintf( message, "ERROR: UNABLE TO INCREASE THRUST TO %d%%", (ROCKET_ENGINE_get_thrust( &main_engine ) + value) );
							}
						} else {
							success = 0;
							strncpy( message, "ERROR: MAIN ENGINE SYSTEM IS IDLE", BIG_BUFF_SIZE );
						}
					} break;

					case DECREASE : {
						if( ROCKET_ENGINE_get_engaged( &main_engine ) == 1 ) {
							if( ROCKET_ENGINE_get_thrust( &main_engine ) - value >= 0 ) {
								ROCKET_ENGINE_set_thrust( &main_engine, ROCKET_ENGINE_get_thrust( &main_engine) - value );
								success = 1;
								memset( message, '\0', BIG_BUFF_SIZE );
							} else {
								success = 0;
								sprintf( message, "ERROR: UNABLE TO DECREASE THRUST TO %d%%", (ROCKET_ENGINE_get_thrust( &main_engine ) - value) );
							}
						} else {
							success = 0;
							strncpy( message, "ERROR: MAIN ENGINE SYSTEM IS IDLE", BIG_BUFF_SIZE );
						}
					}
				}
			}
		} break;

		case PITCH_PROGRAM : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || telemetry_data.current_acceleration <= 0 ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO START PITCH PROGRAM. CHECK CONFIGURATION", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case START : {
						COMPUTER_PROGRAM_start( &pitch_program );
						success = 1;
						strncpy( message, "PITCH PROGRAM STARTED", BIG_BUFF_SIZE );
					} break;

					case STOP : {
						COMPUTER_PROGRAM_stop( &pitch_program );
						success = 1;
						strncpy( message, "PITCH PROGRAM STOPPED", BIG_BUFF_SIZE );
					} break;
				}
			}
		} break;

		case PITCH_MOD : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || telemetry_data.current_acceleration <= 0 ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO START PITCH PROGRAM. CHECK CONFIGURATION", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case INCREASE : {
						pitch_program.current_value++;
						success = 1;
						memset( message, '\0', BIG_BUFF_SIZE );
					} break;

					case DECREASE : {
						pitch_program.current_value--;
						success = 1;
						memset( message, '\0', BIG_BUFF_SIZE );
					} break;
				}
			}
		} break;

		case ROLL_PROGRAM : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || telemetry_data.current_altitude <= 0 ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO START ROLL PROGRAM. CHECK CONFIGURATION", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case START : {
						COMPUTER_PROGRAM_start( &roll_program );
						success = 1;
						strncpy( message, "ROLL PROGRAM STARTED", BIG_BUFF_SIZE );
					} break;

					case STOP : {
						COMPUTER_PROGRAM_stop( &roll_program );
						success = 1;
						strncpy( message, "ROLL PROGRAM STOPPED", BIG_BUFF_SIZE );
					} break;
				}
			}
		} break;

		case ROLL_MOD : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || telemetry_data.current_altitude <= 0 ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO START ROLL PROGRAM. CHECK CONFIGURATION", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case INCREASE : {
						roll_program.current_value++;
						success = 1;
						memset( message, '\0', BIG_BUFF_SIZE );
					} break;

					case DECREASE : {
						roll_program.current_value--;
						success = 1;
						memset( message, '\0', BIG_BUFF_SIZE );
					} break;
				}
			}
		} break;

		case YAW_PROGRAM : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || telemetry_data.current_altitude <= 0 ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO START YAW PROGRAM. CHECK CONFIGURATION", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case START : {
						COMPUTER_PROGRAM_start( &yaw_program );
						success = 1;
						strncpy( message, "YAW PROGRAM STARTED", BIG_BUFF_SIZE );
					} break;

					case STOP : {
						COMPUTER_PROGRAM_stop( &yaw_program );
						success = 1;
						strncpy( message, "YAW PROGRAM STOPPED", BIG_BUFF_SIZE );
					} break;
				}
			}
		} break;

		case YAW_MOD : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || telemetry_data.current_altitude <= 0 ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO START YAW PROGRAM. CHECK CONFIGURATION", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case INCREASE : {
						yaw_program.current_value += 0.1;
						success = 1;
						memset( message, '\0', BIG_BUFF_SIZE );
					} break;

					case DECREASE : {
						yaw_program.current_value -= 0.1;
						success = 1;
						memset( message, '\0', BIG_BUFF_SIZE );
					} break;
				}
			}
		} break;

		case LET : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || ( telemetry_data.current_altitude <= 0 || telemetry_data.launch_escape_tower_ready == 0 ) ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO ENGAGE LAUNCH ESCAPE TOWER SYSTEM. CHECK CONFIGURATION", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case JETTISON : {
						telemetry_data.launch_escape_tower_ready = 0;
						system_s3.instrument_mass -= 4200;
						success = 1;
						strncpy( message, "LAUNCH ESCAPE TOWER JETTISONED", BIG_BUFF_SIZE );
					} break;
				}
			}
		} break;

		case AUTO_PILOT : {
			switch( command ) {
				default : /* Nic */ break;
				case START : {
					telemetry_data.auto_pilot_enabled = 1;
					success = 1;
					strncpy( message, "AUTOPILOT IS ON", BIG_BUFF_SIZE );
				} break;
				case STOP : {
					telemetry_data.auto_pilot_enabled = 0;
					success = 1;
					strncpy( message, "AUTOPILOT IS OFF", BIG_BUFF_SIZE );
				} break;
			}
		} break;

		case HOLDDOWN_ARMS : {
			switch( command ) {
				default : /* Nic */ break;
				case STOP : {
					if( telemetry_data.holddown_arms_released == 0 ) {
						telemetry_data.holddown_arms_released = 1;
						success = 1;
						strncpy( message, "HOLDDOWN ARMS RELEASED", BIG_BUFF_SIZE );
					} else {
						success = 0;
						strncpy( message, "ERROR: UNABLE TO RELEASE HOLDDOWN ARMS. CHECK CONFIGURATION", BIG_BUFF_SIZE );
					}
				} break;
			}
		} break;

		case COUNTDOWN : {
			switch( command ) {
				default : /* Nic */ break;
				case START : {
					if( telemetry_data.countdown_in_progress == 1 ) {
						success = 0;
						strncpy( message, "ERROR: COUNTDOWN IS IN PROGRESS ALREADY", BIG_BUFF_SIZE );
					} else {
						telemetry_data.countdown_in_progress = 1;
						success = 1;
						strncpy( message, "COUNTDOWN IS CONTINUED", BIG_BUFF_SIZE );
						if( computing_all == 0 ) {
							pthread_create(&sthread, NULL, SIMULATION_progress, NULL );
							computing_all = 1;
						}
					}
				} break;
				case STOP : {
					if( telemetry_data.countdown_in_progress == 0 ) {
						success = 0;
						strncpy( message, "ERROR: COUNTDOWN IS ON HOLD ALREADY", BIG_BUFF_SIZE );
					} else {
						if( telemetry_data.mission_time > -8 ) {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO HOLD COUNTDOWN. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						} else {
							telemetry_data.countdown_in_progress = 0;
							success = 1;
							strncpy( message, "COUNTDOWN IS ON HOLD", BIG_BUFF_SIZE );
						}
					}
				} break;
			}
		} break;
	}

	if( ROCKET_ENGINE_get_engaged( &main_engine ) == 1 ) {
		ROCKET_ENGINE_set_thrust( &internal_guidance, ROCKET_ENGINE_get_thrust( &main_engine ) );
	}

	interpreter_result.success = success;
	strncpy( interpreter_result.message, message, BIG_BUFF_SIZE );

	if( strlen( message ) > 0 ) {
		LOG_print( "[%s] %s.\n", get_actual_time(), message );
		printf( "[%s] %s.\n", get_actual_time(), message );
		strncpy( telemetry_data.computer_message, message, STD_BUFF_SIZE );
	}

	return ( INTERPRETER_RESULT * )&interpreter_result;
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
			telemetry_data.current_dynamic_pressure = round( telemetry_data.current_dynamic_pressure );
			return current_dynamic_pressure;
		}
	}

	return ( result >= 0 ? result : -1 );
}

void PHYSICS_instrument_unit_calculations( void ) {
	/* Informacje o postępie lotu*/
	if( telemetry_data.current_altitude > 130 && telemetry_data.current_altitude < 150 ) {
		strncpy( telemetry_data.computer_message, "TOWER CLEARED", STD_BUFF_SIZE );
	}
	if( telemetry_data.current_velocity > 0 && telemetry_data.current_velocity < 1 && telemetry_data.current_altitude < 10 ) {
		strncpy( telemetry_data.computer_message, "LIFT OFF", STD_BUFF_SIZE );
	}
	if( current_system->burn_time > 0 && current_system->burn_time < 1 ) {
		snprintf( telemetry_data.computer_message, STD_BUFF_SIZE, "%s IGNITION", current_system->name );
	}

	if( telemetry_data.stable_orbit_achieved == 0 && ( telemetry_data.current_velocity + 150 ) >= CELESTIAL_OBJECT_get_orbital_speed( AO_current, telemetry_data.current_altitude ) ) {
		strncpy( telemetry_data.computer_message, "PREPARE TO ORBIT INSERTION", STD_BUFF_SIZE );
	}

	/* CRASH */
	if( telemetry_data.current_velocity != 0 && telemetry_data.current_altitude <= 0 ) {
		strncpy( telemetry_data.computer_message, "YOU DIED", STD_BUFF_SIZE );
		telemetry_data.auto_pilot_enabled = 0;
		/* TODO */
	}

	if( telemetry_data.holddown_arms_released == 0 ) {
		if( telemetry_data.current_acceleration >= 3 ) {
			EXEC_COMMAND( THRUST, NULL_THRUST, 0 );
			EXEC_COMMAND( MAIN_ENGINE, STOP, 0 );
			strncpy( telemetry_data.computer_message, "AUTOMATIC ENGINE DISENGAGE", STD_BUFF_SIZE );
		}
	}

	if( telemetry_data.current_velocity >= CELESTIAL_OBJECT_get_orbital_speed( AO_current, telemetry_data.current_altitude ) ) {
		if( telemetry_data.stable_orbit_achieved == 0 ) {
			telemetry_data.stable_orbit_achieved = 1;
			strncpy( telemetry_data.computer_message, "ORBIT INSERTION", STD_BUFF_SIZE );
			MAIN_FLIGHT_STATUS = STABLE_ORBIT;
		}
	}
}

void PHYSICS_shared_calculations( void ) {
	double pitch_mod = ( telemetry_data.stable_orbit_achieved ? 0 : ( ( 100 - pitch_program.current_value ) / 100 ) );

	fw = 0;
	real_fw = 0;
	rad2deg = pitch_program.current_value * _PI / 180;
	telemetry_data.current_fuel_mass = 0;
	telemetry_data.total_mass = 0;
	telemetry_data.thrust_newtons = 0;
	telemetry_data.current_acceleration = 0;
	telemetry_data.current_distance = 0;
	telemetry_data.current_velocity = 0;
	telemetry_data.current_vertical_velocity = 0;
	telemetry_data.current_horizontal_velocity = 0;
	telemetry_data.current_fuel_burn = 0;
	telemetry_data.current_thrust = ROCKET_ENGINE_get_thrust( &internal_guidance );

	current_system = ROCKET_STAGE_get_active();

	if( telemetry_data.countdown_in_progress == 1 ) {
		telemetry_data.mission_time += time_tick;
	}

	if( current_system->burn_start == 0 ) {
		if( telemetry_data.current_thrust > 0 ) {
			current_system->burn_start = get_current_epoch();
		}
	} else {
		if( ROCKET_ENGINE_get_engaged( &main_engine ) == 1 ) {
			current_system->burn_time += time_tick;
		}
	}

	telemetry_data.current_fuel_mass = ROCKET_STAGE_get_fuel( current_system );

	if( ROCKET_STAGE_get_attached( &system_s1 ) == 1 ) {
		telemetry_data.total_mass += ROCKET_STAGE_get_total_mass( &system_s1 );
	}

	if( ROCKET_STAGE_get_attached( &system_s2 ) == 1 ) {
		telemetry_data.total_mass += ROCKET_STAGE_get_total_mass( &system_s2 );
	}

	if( ROCKET_STAGE_get_attached( &system_s3 ) == 1 ) {
		telemetry_data.total_mass += ROCKET_STAGE_get_total_mass( &system_s3 );
	}

	if( telemetry_data.current_fuel_mass > 0 ) {
		if( ROCKET_ENGINE_get_thrust( &internal_guidance ) > 0 ) {
			telemetry_data.current_fuel_burn = ( ( current_system->max_fuel_burn * telemetry_data.current_thrust ) / 100 ) / time_mod;
			ROCKET_STAGE_set_fuel( current_system, ( telemetry_data.current_fuel_mass - telemetry_data.current_fuel_burn), 1 );
		}
	} else {
		telemetry_data.current_fuel_mass = 0;
	}

	if( telemetry_data.current_fuel_burn > 0 ) {
		if( current_system->variable_thrust == 1 && ( current_system->initial_thrust < current_system->max_thrust_n ) ) {
			if( telemetry_data.current_thrust == 100 ) {
				current_system->initial_thrust += ( current_system->thrust_step ) / ( time_mod );
				telemetry_data.thrust_newtons = ( long double )( ( current_system->initial_thrust * telemetry_data.current_thrust ) / 100 );
			} else {
				telemetry_data.thrust_newtons = ( long double )( telemetry_data.current_thrust * current_system->initial_thrust ) / 100;
			}
		} else {
			telemetry_data.thrust_newtons = ( long double )( telemetry_data.current_thrust * current_system->max_thrust_n ) / 100;
		}
	} else {
		telemetry_data.thrust_newtons = 0;
	}

	if( ROCKET_STAGE_get_attached( current_system ) == 1 ) {
		current_system->current_thrust = telemetry_data.thrust_newtons;
	}

	fw = 0;
	real_fw = telemetry_data.thrust_newtons - ( telemetry_data.total_mass * CELESTIAL_OBJECT_get_gravity_value( AO_current, telemetry_data.current_altitude ) );

	if( telemetry_data.max_q_achieved == 0 ) {
		fw = real_fw;
	} else {
		fw = telemetry_data.thrust_newtons - ( telemetry_data.total_mass * ( CELESTIAL_OBJECT_get_gravity_value( AO_current, telemetry_data.current_altitude) / (time_mod) ) );
	}

	telemetry_data.current_acceleration = ( fw / telemetry_data.total_mass );

	if( telemetry_data.current_acceleration < 0 && telemetry_data.current_thrust == 0 && telemetry_data.stable_orbit_achieved == 0 && ( telemetry_data.mission_time - current_system->staging_time ) < 4) {
		telemetry_data.current_acceleration = ( real_fw / telemetry_data.total_mass );
	}

	if( telemetry_data.current_acceleration < 0 && telemetry_data.current_altitude <= 0 ) {
		telemetry_data.current_acceleration = 0;
	}

	if( telemetry_data.stable_orbit_achieved == 1 && telemetry_data.current_altitude >= 100000 ) {
		if( telemetry_data.current_acceleration < 0 ) {
			telemetry_data.current_acceleration = 0;
		}
	}

	if( telemetry_data.holddown_arms_released == 1 ) {
		if( pitch_program.current_value < 90 || pitch_program.current_value > 270 ) {
			telemetry_data.current_velocity = telemetry_data.last_velocity + ( telemetry_data.current_acceleration / ( time_mod ) );
		} else if( pitch_program.current_value > 90 && pitch_program.current_value < 270 ) {
			if( telemetry_data.current_acceleration > 0 ) {
				telemetry_data.current_velocity = telemetry_data.last_velocity + ( telemetry_data.current_acceleration / ( time_mod ) );
			} else {
				telemetry_data.current_velocity = telemetry_data.last_velocity - ( telemetry_data.current_acceleration / ( time_mod ) );
			}
		}
	}

	if( pitch_program.running == 1 ) {
		pitch_program.running_time += time_tick;
		pitch_program.current_value += _AUTOPILOT_get_pitch_step() / time_mod;
	}

	if( pitch_program.current_value > 0 ) {
		rad2deg = pitch_program.current_value * _PI / 180;
		telemetry_data.current_vertical_velocity = round( ( ( telemetry_data.last_velocity + telemetry_data.current_acceleration) - CELESTIAL_OBJECT_get_gravity_value( AO_current, telemetry_data.current_altitude ) ) * cos( rad2deg ) );

		if( telemetry_data.current_vertical_velocity == 0 ) {
			telemetry_data.current_horizontal_velocity = telemetry_data.current_velocity;
		} else {
			telemetry_data.current_horizontal_velocity = round( telemetry_data.last_velocity * sin( rad2deg ) );
		}
	} else {
		telemetry_data.current_vertical_velocity = telemetry_data.current_velocity;
	}

	if( roll_program.running == 1 ) {
		roll_program.current_value += _AUTOPILOT_get_roll_step() / time_mod;
	}

	if( yaw_program.running == 1 ) {
		yaw_program.current_value += _AUTOPILOT_get_yaw_step() / time_mod;
	}

	telemetry_data.current_distance = telemetry_data.current_velocity;

	if( telemetry_data.current_distance != 0 && telemetry_data.current_acceleration != 0 ) {
		if( pitch_program.current_value < 90 ) {
			if( current_system->id == 1 ) {
				pitch_mod = ( 100 - pitch_program.current_value ) / 100;
				telemetry_data.current_altitude += ( ( telemetry_data.current_distance ) * pitch_mod ) / ( time_mod );
			} else {
				pitch_mod = ( 90.1 - pitch_program.current_value ) / 100;
				telemetry_data.current_altitude += ( ( telemetry_data.current_vertical_velocity ) * pitch_mod ) / ( time_mod );
			}
		} else if( pitch_program.current_value > 90 ) {
			if( current_system->id == 1 ) {
				pitch_mod = ( 100 - pitch_program.current_value ) / 100;
				telemetry_data.current_altitude -= ( ( ( telemetry_data.current_distance ) ) * pitch_mod ) / ( time_mod );
			} else {
				pitch_mod = ( 90.1 - pitch_program.current_value ) / 100;
				telemetry_data.current_altitude -= abs( ( ( telemetry_data.current_vertical_velocity ) * pitch_mod ) / ( time_mod ) );
			}
		}
	}

	telemetry_data.last_velocity = telemetry_data.current_velocity;

	if( telemetry_data.current_altitude > 0 ) {
		telemetry_data.total_distance += ( abs( telemetry_data.current_distance ) / time_mod );
	} else {
		telemetry_data.last_velocity = 0;
	}

	if( telemetry_data.current_altitude < 0 ) {
		telemetry_data.current_altitude = -1;
	}

	if( telemetry_data.auto_pilot_enabled == 1 ) {
		AUTOPILOT_progress( telemetry_data.mission_time );
	}
}

void PHYSICS_launch_calculations( void ) {
	double dynamic_pressure = 0;
	double dynamic_pressure_newtons = 0;

	if( telemetry_data.countdown_in_progress == 1 ) {
		if( current_system->id == 1 && current_system->burn_time < 15 && ROCKET_ENGINE_get_engaged( &main_engine ) == 1 ) {
			if( telemetry_data.current_thrust < 100 ) {
				if( EXEC_COMMAND( THRUST, INCREASE, 10 / time_mod )->success == 0) {
					EXEC_COMMAND( THRUST, FULL_THRUST, 0 );
				}
			}
		}
	}

	if(current_system->id == 2) {
		if( ROCKET_STAGE_get_fuel( &system_s1 ) > 0 ) {
			ROCKET_STAGE_set_fuel( &system_s1, ( system_s1.fuel - ( system_s1.max_fuel_burn / time_mod / 10 ) ), 0 );
		}
	}

	if(current_system->id == 3) {
		if( ROCKET_STAGE_get_fuel( &system_s2 ) > 0 ) {
			ROCKET_STAGE_set_fuel( &system_s2, ( system_s2.fuel - ( system_s2.max_fuel_burn / time_mod / 10 ) ), 0 );
		}
	}

	if( ROCKET_ENGINE_get_thrust( &internal_guidance ) >= 95 ) {
		dynamic_pressure = _PHYSICS_get_dynamic_pressure_force( telemetry_data.current_altitude );
	}

	dynamic_pressure_newtons = dynamic_pressure * 4.44;
}
