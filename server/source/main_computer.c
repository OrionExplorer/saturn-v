/*******************************************************************

Projekt Saturn V Main Computer

Plik: main_computer.c

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/main_computer.h"
#include "include/log.h"
#include "include/spacecraft_components.h"
#include "include/celestial_objects.h"
#include "include/telemetry.h"
#include "include/auto_pilot.h"
#include "include/physics.h"
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
long double				fw;
long double				real_fw;
short					computing_all = 0;
double					launch_pad_latitude = 28.500;

/**
Modyfikatory fizyki
**/
int						time_interval = 100;
double					rad2deg;
double					time_tick;
double					time_mod = 0.0;

pthread_t				sthread;

void 					MAIN_COMPUTER_display_last_message( void );

void *MAIN_COMPUTER_simulation_progress( void ) {
	srand ( time( NULL ) );

	while( 1 ) {
		
		MAIN_COMPUTER_shared_calculations();

		switch( MAIN_FLIGHT_STATUS ) {
			case LAUNCH_FROM_EARTH : MAIN_COMPUTER_launch_calculations(); break;
			case STABLE_ORBIT : MAIN_COMPUTER_orbit_calculations(); break;
			case TRANSFER_ORBIT : { /*TODO*/ } break;
			default : break;
		}

		MAIN_COMPUTER_instrument_unit_calculations();

		Sleep( simulation_speed );
	}
}

void MAIN_COMPUTER_init( void ) {
	vDEVICE device;
	vCOMMAND command;
	int value;
	INTERPRETER_RESULT *result;

	srand ( time(NULL) );

	STAGES_init();
	COMPUTER_PROGRAMS_init();
	CELESTIAL_OBJECTS_load();

	telemetry_data.auto_pilot_enabled = 1;

	time_mod = ( 1000 / time_interval );
	normal_atmospheric_pressure += rand() % 10;
	time_tick = ( time_interval * 0.001 );
	/*telemetry_data.mission_time = -10.0;*/
	telemetry_data.launch_escape_tower_ready = 1;
	telemetry_data.active_stage = 1;
	telemetry_data.orbit_revolution_duration = -1;
	AUTOPILOT_init();

	MAIN_FLIGHT_STATUS = LAUNCH_FROM_EARTH;
	/* Uruchomienie sieci */
	SOCKET_main();

	while( scanf("%d %d %d", ( int * )&device, ( int * )&command, &value ) == 3) {
		result = MAIN_COMPUTER_exec( device, command, value );
		printf("%s\n", result->message );
	}
}

/**
Interpreter komend komputera
**/
INTERPRETER_RESULT* MAIN_COMPUTER_exec( vDEVICE device, vCOMMAND command, const int value ) {
	static INTERPRETER_RESULT interpreter_result;
	short success = 0;
	char message[ BIG_BUFF_SIZE ];

	interpreter_result.success = 0;
	strncpy( message, "ERROR: UNKNOWN COMMAND", BIG_BUFF_SIZE );
	memset( interpreter_result.message, '\0', BIG_BUFF_SIZE );

	switch( device ) {
		default : /* Nic */ break;
		case ITERATIVE_GUIDANCE_MODE : {
			switch( command ) {
				default : /* Nic */ break;

				case START : {
					if( telemetry_data.iterative_guidance_mode_active == 1 ) {
						success = 0;
						strncpy( message, "ERROR: ITERATIVE GUIDANCE MODE IS ENGAGED", BIG_BUFF_SIZE );
					} else {
						if( pitch_program.running == 0 && telemetry_data.current_altitude > 0 ) {
							telemetry_data.iterative_guidance_mode_active = 1;
							success = 1;
							strncpy( message, "ITERATIVE GUIDANCE MODE INITIATED", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO ENGAGE ITERATIVE GUIDANCE MODE", BIG_BUFF_SIZE );
						}
					}
				} break;

				case STOP : {
					if( telemetry_data.iterative_guidance_mode_active == 0 ) {
						success = 0;
						strncpy( message, "ERROR: ITERATIVE GUIDANCE MODE IS DISENGAGED", BIG_BUFF_SIZE );
					} else {
						if( telemetry_data.iterative_guidance_mode_active == 1) {
							telemetry_data.iterative_guidance_mode_active = 0;
							success = 1;
							strncpy( message, "ITERATIVE GUIDANCE MODE DISENGAGED", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO DISENGAGE ITERATIVE GUIDANCE MODE", BIG_BUFF_SIZE );
						}
					}
				} break;
			}
		} break;
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
							strncpy( message, "ERROR: UNABLE TO ENGAGE INTERNAL GUIDANCE", BIG_BUFF_SIZE );
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
							strncpy( message, "ERROR: UNABLE TO SWITCH TO EXTERNAL GUIDANCE", BIG_BUFF_SIZE );
						}
					}
				} break;
			}
		} break;

		case S1 : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 1 && ROCKET_ENGINE_get_engaged( &main_engine ) == 1&& command != CENTER_ENGINE_CUTOFF ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO PERFORM ANY S-IC OPERATION", BIG_BUFF_SIZE );
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
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IC PROPELLANT LOADING OPERATION", BIG_BUFF_SIZE );
						}
					} break;

					case ATTACH : {
						if( telemetry_data.current_altitude > 0 ) {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IC ATTACH OPERATION", BIG_BUFF_SIZE );
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
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IC DETACH OPERATION", BIG_BUFF_SIZE );
						} else {
							if( ROCKET_STAGE_get_attached( &system_s1 ) == 1 ) {
								ROCKET_STAGE_do_detach( &system_s1 );
								success = 1;
								strncpy( message, "S-IC STAGED", BIG_BUFF_SIZE );
								telemetry_data.active_stage = 2;
							} else {
								success = 0;
								strncpy( message, "ERROR: S-IC STAGE IS STAGED", BIG_BUFF_SIZE );
							}
						}
					} break;

					case CENTER_ENGINE_CUTOFF : {
						if( system_s1.center_engine_available == 1 && telemetry_data.current_altitude > 0 ) {
							ROCKET_ENGINE_set_thrust( &main_engine, ROCKET_ENGINE_get_thrust( &main_engine ) - 25 );
							system_s1.center_engine_available = 0;
							success = 1;
							strncpy( message, "S-IC CENTER ENGINE CUTOFF", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IC CENTER ENGINE CUTOFF", BIG_BUFF_SIZE );
						}
					} break;
				}
			}
		} break;

		case S2 : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) && ROCKET_ENGINE_get_engaged( &main_engine ) == 1 && ( command != CENTER_ENGINE_CUTOFF && command != INTERSTAGE_JETTISON ) ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO PERFORM ANY S-II OPERATION", BIG_BUFF_SIZE );
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
							strncpy( message, "ERROR: UNABLE TO PERFORM S-II PROPELLANT LOADING OPERATION", BIG_BUFF_SIZE );
						}
					} break;

					case ATTACH : {
						if( telemetry_data.current_altitude > 0 ) {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-II ATTACH OPERATION", BIG_BUFF_SIZE );
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
							strncpy( message, "ERROR: UNABLE TO PERFORM S-II DETACH OPERATION", BIG_BUFF_SIZE );
						} else {
							if( ROCKET_STAGE_get_attached( &system_s2 ) == 1 ) {
								ROCKET_STAGE_do_detach( &system_s2 );
								success = 1;
								strncpy( message, "S-II STAGED", BIG_BUFF_SIZE );
								telemetry_data.active_stage = 3;
							} else {
								success = 0;
								strncpy( message, "ERROR: S-II STAGE IS STAGED", BIG_BUFF_SIZE );
							}
						}
					} break;

					case CENTER_ENGINE_CUTOFF : {
						if( system_s2.center_engine_available == 1 && telemetry_data.current_altitude > 0 ) {
							ROCKET_ENGINE_set_thrust( &main_engine, ROCKET_ENGINE_get_thrust( &main_engine ) - 25 );
							system_s2.center_engine_available = 0;
							success = 1;
							strncpy( message, "S-II CENTER ENGINE CUTOFF", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-II CENTER ENGINE CUTOFF", BIG_BUFF_SIZE );
						}
					} break;

					case INTERSTAGE_JETTISON : {
						if( ROCKET_STAGE_get_attached( &system_s2 ) == 1 && system_s2.interstage_mass > 0 ) {
							system_s2.interstage_mass = 0;
							success = 1;
							strncpy( message, "S-IC/S-II INTERSTAGE JETTISONED", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IC/S-II INTERSTAGE JETTISON", BIG_BUFF_SIZE );
						}
					} break;
				}
			}
		} break;

		case S3 : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 1 && ROCKET_ENGINE_get_engaged( &main_engine ) == 1 && ( command != CENTER_ENGINE_CUTOFF && command != INTERSTAGE_JETTISON ) ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO PERFORM ANY S-IVB OPERATION", BIG_BUFF_SIZE );
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
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IVB PROPELLANT LOADING OPERATION", BIG_BUFF_SIZE );
						}
					} break;

					case ATTACH : {
						if( telemetry_data.current_altitude > 0 ) {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IVB ATTACH OPERATION", BIG_BUFF_SIZE );
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
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IVB DETACH OPERATION", BIG_BUFF_SIZE );
						} else {
							if( ROCKET_STAGE_get_attached( &system_s3 ) == 1 ) {
								ROCKET_STAGE_do_detach( &system_s3 );
								success = 1;
								strncpy( message, "S-IVB STAGED", BIG_BUFF_SIZE );
								telemetry_data.active_stage = -1;
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
							strncpy( message, "ERROR: UNABLE TO RESTART S-IVB BURN", BIG_BUFF_SIZE );
						}
					} break;

					case INTERSTAGE_JETTISON : {
						if( ROCKET_STAGE_get_attached( &system_s3 ) == 1 && system_s3.interstage_mass > 0 ) {
							system_s3.interstage_mass = 0;
							success = 1;
							strncpy( message, "S-II/S-IVB INTERSTAGE JETTISONED", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-II/S-IVB INTERSTAGE JETTISON", BIG_BUFF_SIZE );
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
							strncpy( message, "ERROR: UNABLE TO START MAIN ENGINE", BIG_BUFF_SIZE );
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
							strncpy( message, "ERROR: UNABLE TO STOP MAIN ENGINE", BIG_BUFF_SIZE );
						}
					}
				} break;
			}
		} break;

		case THRUST : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 && ROCKET_ENGINE_get_engaged( &main_engine ) == 0) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO CHANGE THRUST", BIG_BUFF_SIZE );
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
							sprintf( message, "%s ENGINE SHUTDOWN", current_system->id == 1 ? "S-IC" : current_system->id == 2 ? "S-II" : current_system->id == 3 ? "S-IVB" : "MAIN", BIG_BUFF_SIZE );
							//memset( message, '\0', BIG_BUFF_SIZE );
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
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || telemetry_data.current_altitude == 0 || telemetry_data.iterative_guidance_mode_active == 1 ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO START PITCH PROGRAM", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case START : {
						if( pitch_program.running_time > 0 ) {
							success = 0;
							strncpy( message, "ERROR: PITCH PROGRAM RESTART IS NOT ALLOWED", BIG_BUFF_SIZE );	
						} else {
							COMPUTER_PROGRAM_start( &pitch_program );
							success = 1;
							strncpy( message, "PITCH PROGRAM STARTED", BIG_BUFF_SIZE );	
						}
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
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO PITCH", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case INCREASE : {
						pitch_program.current_value += 0.25;
						success = 1;
						memset( message, '\0', BIG_BUFF_SIZE );
					} break;

					case DECREASE : {
						pitch_program.current_value -= 0.25;
						success = 1;
						memset( message, '\0', BIG_BUFF_SIZE );
					} break;
				}
			}
		} break;

		case ROLL_PROGRAM : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || telemetry_data.current_altitude <= 0 ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO START ROLL PROGRAM", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case START : {
						if( roll_program.running_time > 0 ) {
							success = 0;
							strncpy( message, "ERROR: ROLL PROGRAM RESTART NOT ALLOWED", BIG_BUFF_SIZE );
						} else {
							COMPUTER_PROGRAM_start( &roll_program );
							success = 1;
							strncpy( message, "ROLL PROGRAM STARTED", BIG_BUFF_SIZE );	
						}
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
				strncpy( message, "ERROR: UNABLE TO START ROLL PROGRAM", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case INCREASE : {
						roll_program.current_value += 0.1;
						success = 1;
						memset( message, '\0', BIG_BUFF_SIZE );
					} break;

					case DECREASE : {
						roll_program.current_value -= 0.1;
						success = 1;
						memset( message, '\0', BIG_BUFF_SIZE );
					} break;
				}
			}
		} break;

		case YAW_PROGRAM : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || telemetry_data.current_altitude <= 0 ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO START YAW PROGRAM", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case START : {
						if( yaw_program.running_time > 0 ) {
							success = 0;
							strncpy( message, "ERROR: YAW PROGRAM RESTART IS NOT ALLOWED", BIG_BUFF_SIZE );	
						} else {
							COMPUTER_PROGRAM_start( &yaw_program );
							success = 1;
							strncpy( message, "YAW PROGRAM STARTED", BIG_BUFF_SIZE );	
						}
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
				strncpy( message, "ERROR: UNABLE TO START YAW PROGRAM", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case INCREASE : {
						yaw_program.current_value += 0.3;
						success = 1;
						memset( message, '\0', BIG_BUFF_SIZE );
					} break;

					case DECREASE : {
						yaw_program.current_value -= 0.3;
						success = 1;
						memset( message, '\0', BIG_BUFF_SIZE );
					} break;
				}
			}
		} break;

		case LET : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || ( telemetry_data.current_altitude <= 0 || telemetry_data.launch_escape_tower_ready == 0 ) ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO ENGAGE LAUNCH ESCAPE TOWER SYSTEM", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case JETTISON : {
						telemetry_data.launch_escape_tower_ready = 0;
						system_s3.instrument_mass -= 4041.50802;
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
						strncpy( message, "ERROR: UNABLE TO RELEASE HOLDDOWN ARMS", BIG_BUFF_SIZE );
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
							pthread_create(&sthread, NULL, MAIN_COMPUTER_simulation_progress, NULL );
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
							strncpy( message, "ERROR: UNABLE TO HOLD COUNTDOWN", BIG_BUFF_SIZE );
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
		LOG_print( "[%s] %s.\n", TIME_get_gmt(), message );
		if( telemetry_data.mission_time  ) {
			printf( "[%s, T%s%.0f]\t%s\n", TIME_get_gmt(), telemetry_data.mission_time <= 0 ? "" : "+", round( telemetry_data.mission_time ), message );
		}
		
		strncpy( telemetry_data.computer_message, message, STD_BUFF_SIZE );
	}

	return ( INTERPRETER_RESULT * )&interpreter_result;
}



void MAIN_COMPUTER_display_last_message( void ) {
	static char last_message[ STD_BUFF_SIZE ];

	if( strcmp( last_message, telemetry_data.computer_message ) != 0 ) {
		printf( "[%s, T%s%.0f]\t%s.\n", TIME_get_gmt(), telemetry_data.mission_time <= 0 ? "" : "+", round( telemetry_data.mission_time ), telemetry_data.computer_message );
		strncpy( last_message, telemetry_data.computer_message, STD_BUFF_SIZE );
	}	
}

void MAIN_COMPUTER_instrument_unit_calculations( void ) {
	/* Informacje o postępie lotu*/
	if( telemetry_data.current_altitude < 150 && telemetry_data.current_altitude > 130 ) {
		strncpy( telemetry_data.computer_message, "TOWER CLEARED", STD_BUFF_SIZE );
		MAIN_COMPUTER_display_last_message();
	}
	if( telemetry_data.current_velocity < 1 && telemetry_data.current_velocity > 0 && telemetry_data.current_altitude < 10 ) {
		strncpy( telemetry_data.computer_message, "LIFT OFF", STD_BUFF_SIZE );
		MAIN_COMPUTER_display_last_message();
	}
	if( current_system->burn_time < 1 && current_system->burn_time > 0 ) {
		snprintf( telemetry_data.computer_message, STD_BUFF_SIZE, "%s IGNITION", current_system->name );
		MAIN_COMPUTER_display_last_message();
	}
	if( telemetry_data.mach_1_achieved == 0 && telemetry_data.current_velocity >= 340 ) {
		telemetry_data.mach_1_achieved = 1;
		strncpy( telemetry_data.computer_message, "MACH 1 ACHIEVED", STD_BUFF_SIZE );
		MAIN_COMPUTER_display_last_message();
	}

	/*if( telemetry_data.stable_orbit_achieved == 0 && ( telemetry_data.current_velocity + 150 ) >= CELESTIAL_OBJECT_get_orbital_speed( AO_current, telemetry_data.current_altitude ) ) {
		strncpy( telemetry_data.computer_message, "PREPARE TO ORBIT INSERTION", STD_BUFF_SIZE );
	}*/

	/* CRASH */
	if( telemetry_data.current_velocity != 0 && telemetry_data.current_altitude < 0 ) {
		strncpy( telemetry_data.computer_message, "YOU DIED", STD_BUFF_SIZE );
		telemetry_data.auto_pilot_enabled = 0;
		/* TODO */
	}

	if( telemetry_data.holddown_arms_released == 0 ) {
		if( telemetry_data.current_acceleration >= 3 ) {
			MAIN_COMPUTER_exec( THRUST, NULL_THRUST, 0 );
			MAIN_COMPUTER_exec( MAIN_ENGINE, STOP, 0 );
			strncpy( telemetry_data.computer_message, "AUTOMATIC ENGINE DISENGAGE", STD_BUFF_SIZE );
			MAIN_COMPUTER_display_last_message();
		}
	}

	if( round( telemetry_data.current_velocity ) == telemetry_data.orbital_velocity ) {
		if( telemetry_data.stable_orbit_achieved == 0 ) {
			telemetry_data.stable_orbit_achieved = 1;
			strncpy( telemetry_data.computer_message, "ORBIT INSERTION", STD_BUFF_SIZE );
			MAIN_COMPUTER_display_last_message();
			telemetry_data.orbit_revolution_duration = 1;
			MAIN_FLIGHT_STATUS = STABLE_ORBIT;
		}
	}

	if( system_s1.attached == 0 && pitch_program.running == 1 && telemetry_data.mission_time - system_s1.staging_time >= 1 ) {
		MAIN_COMPUTER_exec( PITCH_PROGRAM, STOP, 0 );
	}

	if( system_s1.attached == 0 && telemetry_data.iterative_guidance_mode_active == 0 && telemetry_data.mission_time - system_s1.staging_time >= 44 && telemetry_data.mission_time - system_s1.staging_time < 45 && telemetry_data.auto_pilot_enabled == 1 && telemetry_data.stable_orbit_achieved == 0 ) {
		MAIN_COMPUTER_exec( ITERATIVE_GUIDANCE_MODE, START, 0 );
	}
}

void MAIN_COMPUTER_shared_calculations( void ) {
	double pitch_mod = ( telemetry_data.stable_orbit_achieved ? 0 : ( ( 100 - pitch_program.current_value ) / 100 ) );
	double tmp;

	fw = 0;
	real_fw = 0;
	telemetry_data.current_fuel_mass = 0;
	telemetry_data.total_mass = 0;
	telemetry_data.thrust_newtons = 0;
	telemetry_data.current_acceleration = 0;
	telemetry_data.current_distance = 0;
	telemetry_data.current_velocity = 0;
	telemetry_data.current_fuel_burn = 0;
	telemetry_data.current_thrust = ROCKET_ENGINE_get_thrust( &internal_guidance );

	current_system = ROCKET_STAGE_get_active();

	if( telemetry_data.countdown_in_progress == 1 ) {
		telemetry_data.mission_time += time_tick;
	}

	if( current_system->burn_start == 0 ) {
		if( telemetry_data.current_thrust > 0 ) {
			current_system->burn_start = TIME_get_epoch();
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

	if( telemetry_data.current_acceleration < 0 ) {
		if( telemetry_data.current_thrust == 0 && telemetry_data.stable_orbit_achieved == 0 && ( telemetry_data.mission_time - current_system->staging_time ) < 4 ) {
			telemetry_data.current_acceleration = ( real_fw / telemetry_data.total_mass );	
		}

		if( telemetry_data.current_altitude <= 0 ) {
			telemetry_data.current_altitude = 0;
			telemetry_data.current_acceleration = 0;	
		}
		if( telemetry_data.stable_orbit_achieved == 1 && telemetry_data.current_altitude >= 100000 ) {
			telemetry_data.current_acceleration = 0;
		}
	}

	telemetry_data.current_gforce = telemetry_data.current_acceleration / 10;

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

	if( pitch_program.current_value > 0 ) {
		rad2deg = pitch_program.current_value * _PI / 180;
		telemetry_data.current_vertical_velocity = round( ( ( telemetry_data.last_velocity + telemetry_data.current_acceleration) - CELESTIAL_OBJECT_get_gravity_value( AO_current, telemetry_data.current_altitude ) ) * cos( rad2deg ) );
		if( telemetry_data.stable_orbit_achieved == 0 ) {
			telemetry_data.current_vertical_velocity -= CELESTIAL_OBJECT_get_gravity_value( AO_current, telemetry_data.current_altitude ) / time_mod;
		} else {
			if( telemetry_data.current_vertical_velocity < 0 ) {
				telemetry_data.current_vertical_velocity = 0;
			}
		}

		if( telemetry_data.current_vertical_velocity == 0 ) {
			telemetry_data.current_horizontal_velocity = telemetry_data.current_velocity;
		} else {
			telemetry_data.current_horizontal_velocity = round( telemetry_data.last_velocity * sin( rad2deg ) );
		}
	} else {
		telemetry_data.current_vertical_velocity = telemetry_data.current_velocity;
	}


	telemetry_data.current_distance = telemetry_data.current_velocity;

	/* This is pitch and altitude relation simulation */
	if( telemetry_data.current_distance != 0 ) {
		if( pitch_program.current_value < 90 ) {
			if( telemetry_data.current_vertical_velocity >= 0 ) {
				telemetry_data.current_altitude += telemetry_data.current_vertical_velocity / time_mod;
			}
		} else if( pitch_program.current_value > 90 ) {
			if( telemetry_data.current_vertical_velocity != 0 ) {
				telemetry_data.current_altitude += ( telemetry_data.current_vertical_velocity / time_mod );
			}
		}
	}

	telemetry_data.last_velocity = telemetry_data.current_velocity;

	if( telemetry_data.current_altitude > 0 ) {
		telemetry_data.total_distance += ( abs( telemetry_data.current_distance ) / time_mod );
		telemetry_data.downrange += telemetry_data.current_horizontal_velocity / time_mod;
	} else {
		telemetry_data.last_velocity = 0;
	}

	telemetry_data.orbital_velocity = round( CELESTIAL_OBJECT_get_orbital_speed( AO_current, telemetry_data.current_altitude ) );

	if( telemetry_data.auto_pilot_enabled == 1 ) {
		AUTOPILOT_progress( telemetry_data.mission_time );
	}

	if( telemetry_data.iterative_guidance_mode_active == 1 ) {
		pitch_program.current_value += PHYSICS_IGM_get_pitch_step() / time_mod;	
	}

	if( telemetry_data.orbit_periapsis < 0 ) {
		telemetry_data.orbit_inclination = _PHYSICS_get_orbit_inclination( launch_pad_latitude, roll_program.current_value );		
	}
	telemetry_data.orbit_semi_major_axis = _PHYSICS_get_orbit_semi_major_axis( telemetry_data.current_altitude, telemetry_data.current_velocity );
	telemetry_data.orbit_eccentrity = _PHYSICS_get_orbit_eccentrity( telemetry_data.current_altitude, telemetry_data.current_velocity );
	telemetry_data.orbit_semi_minor_axis = _PHYSICS_get_orbit_semi_minor_axis( telemetry_data.orbit_semi_major_axis, telemetry_data.orbit_eccentrity );
	telemetry_data.orbit_apoapsis = _PHYSICS_get_orbit_apogee( telemetry_data.orbit_semi_major_axis, telemetry_data.orbit_eccentrity );
	telemetry_data.orbit_periapsis = _PHYSICS_get_orbit_perigee( telemetry_data.orbit_semi_major_axis, telemetry_data.orbit_eccentrity );
	telemetry_data.orbit_apoapsis_velocity = _PHYSICS_get_orbit_apoapsis_velocity( telemetry_data.orbit_apoapsis, telemetry_data.orbit_periapsis );
	telemetry_data.orbit_periapsis_velocity = _PHYSICS_get_orbit_periapsis_velocity( telemetry_data.orbit_apoapsis, telemetry_data.orbit_periapsis );
	telemetry_data.orbit_circumference = _PHYSICS_get_orbit_circumference( telemetry_data.orbit_semi_major_axis, telemetry_data.orbit_semi_minor_axis );
	telemetry_data.orbit_revolution_period = _PHYSICS_get_orbit_revolution_period( telemetry_data.orbit_semi_major_axis );
	telemetry_data.orbit_mean_motion = _PHYSICS_get_orbit_mean_motion( telemetry_data.orbit_semi_major_axis );

	if( telemetry_data.orbit_periapsis > telemetry_data.orbit_apoapsis ) {
		tmp = telemetry_data.orbit_apoapsis;
		telemetry_data.orbit_apoapsis = telemetry_data.orbit_periapsis;
		telemetry_data.orbit_periapsis = tmp;
	}
}

void MAIN_COMPUTER_launch_calculations( void ) {
	double dynamic_pressure = 0;

	if( telemetry_data.countdown_in_progress == 1 ) {
		if( current_system->id == 1 && current_system->burn_time < 15 && ROCKET_ENGINE_get_engaged( &main_engine ) == 1 ) {
			if( telemetry_data.current_thrust < 100 ) {
				if( MAIN_COMPUTER_exec( THRUST, INCREASE, 10 / time_mod )->success == 0) {
					MAIN_COMPUTER_exec( THRUST, FULL_THRUST, 0 );
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
		_PHYSICS_get_dynamic_pressure_force( telemetry_data.current_altitude );
	}

	if( pitch_program.running == 1 ) {
		pitch_program.running_time += time_tick;
		if( telemetry_data.auto_pilot_enabled == 1 ) {
			pitch_program.current_value += _AUTOPILOT_get_pitch_step() / time_mod;
		}
	}

	if( telemetry_data.stable_orbit_achieved == 0 && pitch_program.current_value > 0 && pitch_program.current_value < 90 ) {
		pitch_program.current_value -= 0.0662500 / time_mod;
	}

	if( roll_program.running == 1 ) {
		roll_program.current_value += _AUTOPILOT_get_roll_step() / time_mod;
	}

	if( yaw_program.running == 1 ) {
		yaw_program.current_value += _AUTOPILOT_get_yaw_step() / time_mod;
	}
}

void MAIN_COMPUTER_orbit_calculations( void ) {
	telemetry_data.orbit_revolution_duration += time_tick;
}
