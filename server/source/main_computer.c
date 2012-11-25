/*******************************************************************

Projekt Voyager 7 Board Computer

Plik: main_computer.c

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/main_computer.h"
#include "include/server_log.h"
#include "include/spacecraft_components.h"
#include "include/celestial_objects.h"
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
/**TELEMETRIA**/
int						current_fuel_mass = 0;
long double				total_mass = 0.0;
long double				thrust_newtons = 0.0;
double					current_acceleration = 0.0;
double					current_distance = 0.0;
double					current_velocity = 0.0;
double					current_vertical_velocity = 0.0;
double					current_horizontal_velocity = 0.0;
double					current_fuel_burn = 0.0;
int						current_thrust = 0;
double					current_altitude = 0.0;
double					total_distance = 0.0;
double					last_velocity = 0.0;
double					mission_time = -7.0;
double					ascending_time = -1;
short					max_q_achieved = 0;
int						current_dynamic_pressure = 0;
short					stable_orbit_achieved = 0;

/**
Modyfikatory fizyki
**/
double					pitch_modifier = 0.0;
short					liftoff_yaw_achieved = 0;
double					normal_atmospheric_pressure = 1013.25;
int						time_interval = 100;
double					time_mod = 0.0;

/**
Zmienne dla auto pilota
**/
short					auto_pilot_enabled = 1;
short					launch_escape_tower_ready = 1;

/**
Pozostałe
**/
short					computing_all = 0;
pthread_t				sthread;

void *run_simulation( void ) {

	srand ( time(NULL) );

	while(1) {
		compute_launch_physics();
		Sleep(100);
	}
}

void TELEMETRY_prepare_data( char *dst, unsigned int dst_len ) {
	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();
	char *output;

	cJSON_AddTrueToObject( root, "success" );
	cJSON_AddStringToObject( root, "data_type", "telemetry" );
	cJSON_AddItemToObject( root, "data", data );

	cJSON_AddStringToObject( data, "computer_message", telemetry_data.computer_message );
	cJSON_AddNumberToObject( data, "mission_time", telemetry_data.mission_time );
	cJSON_AddStringToObject( data, "current_time_gmt", telemetry_data.current_time_gmt );

	cJSON_AddNumberToObject( data, "current_fuel_mass", telemetry_data.current_fuel_mass );
	cJSON_AddNumberToObject( data, "total_mass", telemetry_data.total_mass );
	cJSON_AddNumberToObject( data, "thrust_newtons", telemetry_data.thrust_newtons );
	cJSON_AddNumberToObject( data, "current_acceleration", telemetry_data.current_acceleration );
	cJSON_AddNumberToObject( data, "current_gforce", telemetry_data.current_gforce );
	cJSON_AddNumberToObject( data, "current_distance", telemetry_data.current_distance );
	cJSON_AddNumberToObject( data, "current_velocity", telemetry_data.current_velocity );
	cJSON_AddNumberToObject( data, "current_vertical_velocity", telemetry_data.current_vertical_velocity );
	cJSON_AddNumberToObject( data, "current_horizontal_velocity", telemetry_data.current_horizontal_velocity );
	cJSON_AddNumberToObject( data, "current_fuel_burn", telemetry_data.current_fuel_burn );
	cJSON_AddNumberToObject( data, "current_thrust", telemetry_data.current_thrust );
	cJSON_AddNumberToObject( data, "current_altitude", telemetry_data.current_altitude );
	cJSON_AddNumberToObject( data, "total_distance", telemetry_data.total_distance );
	cJSON_AddNumberToObject( data, "last_velocity", telemetry_data.last_velocity );

	cJSON_AddNumberToObject( data, "ascending_time", telemetry_data.ascending_time );

	cJSON_AddNumberToObject( data, "max_q_achieved", telemetry_data.max_q_achieved );
	cJSON_AddNumberToObject( data, "current_dynamic_pressure", telemetry_data.current_dynamic_pressure );

	cJSON_AddNumberToObject( data, "stable_orbit_achieved", telemetry_data.stable_orbit_achieved );
	cJSON_AddNumberToObject( data, "launch_escape_tower_ready", telemetry_data.launch_escape_tower_ready );

	cJSON_AddNumberToObject( data, "pitch", telemetry_data.pitch );
	cJSON_AddNumberToObject( data, "dest_pitch", telemetry_data.dest_pitch );
	cJSON_AddNumberToObject( data, "roll", telemetry_data.roll );
	cJSON_AddNumberToObject( data, "dest_roll", telemetry_data.dest_roll );
	cJSON_AddNumberToObject( data, "yaw", telemetry_data.yaw );

	cJSON_AddStringToObject( data, "destination", telemetry_data.destination );
	cJSON_AddNumberToObject( data, "destination_altitude", telemetry_data.destination_altitude );

	cJSON_AddNumberToObject( data, "internal_guidance_engaged", telemetry_data.internal_guidance_engaged );
	cJSON_AddNumberToObject( data, "main_engine_engaged", telemetry_data.main_engine_engaged );

	cJSON_AddNumberToObject( data, "pitch_program_engaged", telemetry_data.pitch_program_engaged );
	cJSON_AddNumberToObject( data, "roll_program_engaged", telemetry_data.roll_program_engaged );
	cJSON_AddNumberToObject( data, "yaw_program_engaged", telemetry_data.yaw_program_engaged );

	cJSON_AddNumberToObject( data, "s_ic_fuel", telemetry_data.s_ic_fuel );
	cJSON_AddNumberToObject( data, "s_ic_attached", telemetry_data.s_ic_attached );
	cJSON_AddNumberToObject( data, "s_ic_thrust", telemetry_data.s_ic_thrust );
	cJSON_AddNumberToObject( data, "s_ic_burn_time", telemetry_data.s_ic_burn_time );
	cJSON_AddNumberToObject( data, "s_ic_center_engine_available", telemetry_data.s_ic_center_engine_available );

	cJSON_AddNumberToObject( data, "s_ii_fuel", telemetry_data.s_ii_fuel );
	cJSON_AddNumberToObject( data, "s_ii_attached", telemetry_data.s_ii_attached );
	cJSON_AddNumberToObject( data, "s_ii_thrust", telemetry_data.s_ii_thrust );
	cJSON_AddNumberToObject( data, "s_ii_burn_time", telemetry_data.s_ii_burn_time );
	cJSON_AddNumberToObject( data, "s_ii_center_engine_available", telemetry_data.s_ii_center_engine_available );

	cJSON_AddNumberToObject( data, "s_ivb_fuel", telemetry_data.s_ivb_fuel );
	cJSON_AddNumberToObject( data, "s_ivb_attached", telemetry_data.s_ivb_attached );
	cJSON_AddNumberToObject( data, "s_ivb_thrust", telemetry_data.s_ivb_thrust );
	cJSON_AddNumberToObject( data, "s_ivb_burn_time", telemetry_data.s_ivb_burn_time );
	cJSON_AddNumberToObject( data, "s_ivb_center_engine_available", telemetry_data.s_ivb_center_engine_available );

	output = cJSON_Print( root );

	strncpy( dst, output, dst_len );

	cJSON_Delete( root );
	root = NULL;

	free( output );
	output = NULL;
}

void TELEMETRY_send_ondemand_data( CONNECTED_CLIENT *client) {
	char *output;

	output = ( char * )calloc( BIG_BUFF_SIZE, sizeof( char ) );

	TELEMETRY_prepare_data( output, BIG_BUFF_SIZE );
	SOCKET_send( &communication_session_, client, output, -1 );

	free( output );
	output = NULL;
}
/*
void* TELEMETRY_send_live_data(void* data)
Funkcja przesyła do wszystkich podłączonych klientów informacje o telemetrii. */
void* TELEMETRY_send_live_data( void* data ) {
	int i;

	while(1) {
		Sleep(200);
		for( i = 0; i < MAX_CLIENTS; i++ ) {
			TELEMETRY_update();
			if(connected_clients[ i ].socket_descriptor > 0 ) {
				if( connected_clients[ i ].authorized == 1 && connected_clients[ i ].binded == 1 ) {
					TELEMETRY_send_ondemand_data( &connected_clients[ i ] );
				}
			}
		}
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
	//EXEC_COMMAND( INTERNAL_GUIDANCE, START, 0 );

	time_mod = ( 1000 / time_interval );
	normal_atmospheric_pressure += rand() % 10;
	pitch_modifier = ( (double)rand()/(double)RAND_MAX ) / 1380;


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
							ROCKET_STAGE_set_fuel( &system_s1, system_s1.max_fuel );
							success = 1;
							strncpy( message, "S-IC STAGE PROPELLANT IS LOADED", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IC PROPELLANT LOADING OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					} break;

					case ATTACH : {
						if( current_altitude > 0 ) {
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
						if( current_altitude == 0 ) {
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
						if( system_s1.center_engine_available == 1 && current_altitude > 0 ) {
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
							ROCKET_STAGE_set_fuel( &system_s2, system_s2.max_fuel );
							success = 1;
							strncpy( message, "S-II STAGE PROPELLANT IS LOADED", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-II PROPELLANT LOADING OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					} break;

					case ATTACH : {
						if( current_altitude > 0 ) {
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
						if( current_altitude == 0 ) {
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
						if( system_s2.center_engine_available == 1 && current_altitude > 0 ) {
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
							ROCKET_STAGE_set_fuel( &system_s3, system_s3.max_fuel );
							success = 1;
							strncpy( message, "S-IVB STAGE PROPELLANT IS LOADED", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: UNABLE TO PERFORM S-IVB PROPELLANT LOADING OPERATION. CHECK CONFIGURATION", BIG_BUFF_SIZE );
						}
					} break;

					case ATTACH : {
						if( current_altitude > 0 ) {
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
						if( current_altitude == 0 ) {
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
						if( current_system->id == system_s3.id && current_altitude > 0 ) {
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
							strncpy( message, "MAIN ENGINE ENGAGED", BIG_BUFF_SIZE );
							if( computing_all == 0 ) {
								/* Uruchomienie w¹tku wysy³aj¹cego dane do pod³¹czonych klientów */
								pthread_create(&sthread, NULL, run_simulation, NULL );
								strncpy( message, "IGNITION SEQUENCE START", BIG_BUFF_SIZE );
								computing_all = 1;
								EXEC_COMMAND( THRUST, INCREASE, 15 );
							}
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
							strncpy( message, "MAIN ENGINE DISENGAGED", BIG_BUFF_SIZE );
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
							strncpy( message, "THRUST SET TO 100%", BIG_BUFF_SIZE );
						} else {
							success = 0;
							strncpy( message, "ERROR: MAIN ENGINE SYSTEM IS IDLE", BIG_BUFF_SIZE );
						}
					} break;

					case NULL_THRUST : {
						if( ROCKET_ENGINE_get_engaged( &main_engine ) == 1 ) {
							ROCKET_ENGINE_set_thrust( &main_engine, 0 );
							success = 1;
							strncpy( message, "THRUST TERMINATED", BIG_BUFF_SIZE );
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
								sprintf( message, "THRUST SET TO %d%%", ROCKET_ENGINE_get_thrust( &main_engine ) );
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
								sprintf( message, "THRUST SET TO %d%%", ROCKET_ENGINE_get_thrust( &main_engine ) );
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
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 ) {
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

		case ROLL_PROGRAM : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || current_altitude <= 0 ) {
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

		case YAW_PROGRAM : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || current_altitude <= 0 ) {
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

		case LET : {
			if( ROCKET_ENGINE_get_engaged( &internal_guidance ) == 0 || ROCKET_ENGINE_get_engaged( &main_engine ) == 0 || ( current_altitude <= 0 || launch_escape_tower_ready == 0 ) ) {
				success = 0;
				strncpy( message, "ERROR: UNABLE TO ENGAGE LAUNCH ESCAPE TOWER SYSTEM. CHECK CONFIGURATION", BIG_BUFF_SIZE );
			} else {
				switch( command ) {
					default : /* Nic */ break;
					case JETTISON : {
						launch_escape_tower_ready = 0;
						system_s3.instrument_mass -= 4200;
						success = 1;
						strncpy( message, "LAUNCH ESCAPE TOWER JETTISONED", BIG_BUFF_SIZE );
					} break;
				}
			}
		} break;
	}

	if( ROCKET_ENGINE_get_engaged( &main_engine ) == 1 ) {
		ROCKET_ENGINE_set_thrust( &internal_guidance, ROCKET_ENGINE_get_thrust( &main_engine ) );
	}

	interpreter_result.success = success;
	strncpy( interpreter_result.message, message, BIG_BUFF_SIZE );

	LOG_print( "[%s] %s.\n", get_actual_time_gmt(), message );
	printf( "[%s] %s.\n", get_actual_time_gmt(), message );

	strncpy( telemetry_data.computer_message, message, STD_BUFF_SIZE );

	return ( INTERPRETER_RESULT * )&interpreter_result;
}

double get_dynamic_pressure_force( double altitude ) {
	double current_temperature = 288.15 - (0.0065 * altitude );
	long double gas_constant = 8.3144621;
	double current_atmospheric_pressure = normal_atmospheric_pressure * exp( ( -1 * ( 0.0289644 * CELESTIAL_OBJECT_get_gravity_value( AO_current, altitude ) * altitude ) ) / (gas_constant * current_temperature ) );
	double current_air_destiny = current_atmospheric_pressure / ( gas_constant * current_temperature );
	double dynamic_pressure = ( 0.5 * current_air_destiny) * pow( last_velocity, 2 );
	double result = dynamic_pressure;

	/*double vel = sqrt( ( ( 2 * dynamic_pressure ) / current_air_destiny));*/

	if( max_q_achieved == 1 ) {
		if( round( result ) == 0 ) {
			return round( result );
		}
	} else {
		if( result >= current_dynamic_pressure ) {
			current_dynamic_pressure = result;
			return round( result );
		} else {
			max_q_achieved = 1;
			current_dynamic_pressure = round( current_dynamic_pressure );
			return current_dynamic_pressure;
		}
	}

	return 0;
}

double get_pitch_step( void ) {
	int seconds = round(pitch_program.running_time);
	double result = 0.0;

	if(seconds >= 1 && seconds < 30) {
		result = 0.8965517;
	}
	if(seconds >= 30 && seconds < 70) {
		result = 0.5212500;
	}
	if(seconds >= 70 && seconds < 130 ) {
		result = 0.3908333;
	}
	if(seconds >= 130 && seconds < 210) {
		result = (0.1562500*(-1));
	}
	if(seconds >= 210 && seconds < 360) {
		result = 0.0975067;
	}
	if(seconds >= 360 && seconds < 510) {
		result = 0.0487867;
	}
	if(seconds >= 510 && seconds < 590) {
		result = 0.0194500;
	}
	if(seconds >= 590 && seconds < 670) {
		result = 0.1262500;
	}

	return ( result + pitch_modifier );
}

double get_roll_step( void ) {
	return 0.94;
}

double get_yaw_step( void ) {
	if( yaw_program.current_value >= yaw_program.dest_value ) {
		liftoff_yaw_achieved = 1;
	}

	if( current_altitude < 130 && liftoff_yaw_achieved == 0 ) {
		return 0.15625;
	} else if( current_altitude > 130 ) {
		return -0.10625;
	}

	return 0;
}

void auto_pilot( double real_second ) {
	int second = round( real_second );

	if( stable_orbit_achieved == 1 ) {
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

	if( system_s1.fuel <= 10000 && system_s1.attached == 1 ) {
		EXEC_COMMAND( THRUST, NULL_THRUST, 0 );
		EXEC_COMMAND( MAIN_ENGINE, STOP, 0 );
		EXEC_COMMAND( S1, DETACH, 0 );
	}

	switch( second ) {
		default : /* Nic */ break;
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

		case 31 : {
			if( pitch_program.running == 0 ) {
				EXEC_COMMAND( PITCH_PROGRAM, START, 0 );
			}
		} break;

		case 135 : {
			if( system_s1.center_engine_available == 1 ) {
				EXEC_COMMAND( S1, CENTER_ENGINE_CUTOFF, 0 );
			}
		} break;

		/*case 161 : {
			if( ROCKET_ENGINE_get_thrust( &internal_guidance ) == 80 && current_system->id == 1 ) {
				EXEC_COMMAND( THRUST, NULL_THRUST, 0 );
				EXEC_COMMAND( MAIN_ENGINE, STOP, 0 );
				EXEC_COMMAND( S1, DETACH, 0 );
			}
		} break;*/

		case 166 : {
			if( ROCKET_ENGINE_get_thrust( &internal_guidance ) == 0 ) {
				EXEC_COMMAND( MAIN_ENGINE, START, 0 );
				EXEC_COMMAND( THRUST, FULL_THRUST, 0 );
			}
		} break;

		case 197 : {
			if( launch_escape_tower_ready == 1 ) {
				EXEC_COMMAND( LET, JETTISON, 0 );
			}
		} break;

		case 460 : {
			if( system_s2.center_engine_available == 1 ) {
				EXEC_COMMAND( S2, CENTER_ENGINE_CUTOFF, 0 );
			}
		} break;

		case 500 : {
			if( ROCKET_ENGINE_get_thrust( &internal_guidance ) > 60 ) {
				EXEC_COMMAND( THRUST, DECREASE, 20 );
			}
		} break;

		case 548 : {
			if( ROCKET_ENGINE_get_thrust( &internal_guidance ) == 60 && current_system->id == 2 ) {
				EXEC_COMMAND( THRUST, NULL_THRUST, 0 );
				EXEC_COMMAND( MAIN_ENGINE, STOP, 0 );
				EXEC_COMMAND( S2, DETACH, 0 );
			}
		} break;

		case 552 : {
			if( ROCKET_ENGINE_get_thrust( &internal_guidance ) == 0 ) {
				EXEC_COMMAND( MAIN_ENGINE, START, 0 );
				EXEC_COMMAND( THRUST, FULL_THRUST, 0 );
			}
		} break;
	}
}

void instrument_unit_calculations( void ) {

	if( mission_time > -7 && mission_time < 0 && current_thrust < 100 ) {
		EXEC_COMMAND( THRUST, INCREASE, 10 / time_mod);
	} else if( mission_time > 0 && mission_time < 1 && current_thrust < 100 ) {
		EXEC_COMMAND( THRUST, FULL_THRUST, 0 );
	}

	if( current_velocity >= CELESTIAL_OBJECT_get_orbital_speed( AO_current, current_altitude ) ) {
		if( stable_orbit_achieved == 0 ) {
			stable_orbit_achieved = 1;
			/*printf("EARTH ORBIT INSERTION\n");*/
			if( pitch_program.current_value > 90 ) {
				pitch_program.current_value = 90;
			}
			if( auto_pilot_enabled == 1 ) {
				auto_pilot( mission_time );
			}
		}
	}

	//printf("[%4.0f MET][dV:%2.1f m/s][alt:%6.0f m][vel:%4.0f m/s]             \r", round(telemetry_data.mission_time), (telemetry_data.current_acceleration), (telemetry_data.current_altitude), round(telemetry_data.last_velocity) );
}

void compute_launch_physics( void ) {

	double time_tick = ( time_interval * 0.001 );
	double dynamic_pressure = 0;
	double dynamic_pressure_newtons = 0;
	long double fw = 0;
	double rad2deg = pitch_program.current_value * _PI / 180;
	double pitch_mod = ( 100 - pitch_program.current_value ) / 100;

	current_fuel_mass = 0;
	total_mass = 0;
	thrust_newtons = 0;
	current_acceleration = 0;
	current_distance = 0;
	current_velocity = 0;
	current_vertical_velocity = 0;
	current_horizontal_velocity = 0;
	current_fuel_burn = 0;
	current_thrust = ROCKET_ENGINE_get_thrust( &internal_guidance );

	if( ROCKET_STAGE_get_attached( &system_s1 ) == 0) {
		if( ROCKET_STAGE_get_attached( &system_s2 ) == 1 ) {
			current_system = &system_s2;
		}
	}

	if( ROCKET_STAGE_get_attached( &system_s2 ) == 0) {
		if( ROCKET_STAGE_get_attached( &system_s3 ) == 1 ) {
			current_system = &system_s3;
		}
	}

	if( ( ROCKET_STAGE_get_attached( &system_s1 ) == 0 ) && ( ROCKET_STAGE_get_attached( &system_s2 ) == 0 ) && ( ROCKET_STAGE_get_attached( &system_s3 ) == 0 ) ) {
		current_system = &system_null;
	}

	if( current_system->burn_start == 0 ) {
		if( current_thrust > 0 ) {
			current_system->burn_start = get_current_epoch();
		}
	} else {
		if( ROCKET_ENGINE_get_engaged( &main_engine ) == 1 ) {
			current_system->burn_time += time_tick;
		}
	}

	current_fuel_mass = ROCKET_STAGE_get_fuel( current_system );

	if( ROCKET_STAGE_get_attached( &system_s1 ) == 1 ) {
		total_mass += ROCKET_STAGE_get_total_mass( &system_s1 );
	}

	if( ROCKET_STAGE_get_attached( &system_s2 ) == 1 ) {
		total_mass += ROCKET_STAGE_get_total_mass( &system_s2 );
	}

	if( ROCKET_STAGE_get_attached( &system_s3 ) == 1 ) {
		total_mass += ROCKET_STAGE_get_total_mass( &system_s3 );
	}

	mission_time += time_tick;

	if( auto_pilot_enabled == 1 ) {
		auto_pilot( mission_time );
	}


	if( current_altitude == 0 ) {
		ascending_time = 0;
	}

	if( current_fuel_mass > 0 ) {
		current_fuel_burn = ( ( current_system->max_fuel_burn * current_thrust ) / 100 ) / time_mod;
		ROCKET_STAGE_set_fuel( current_system, ( current_fuel_mass - current_fuel_burn) );
	} else {
		current_fuel_mass = 0;
	}

	if( current_fuel_burn > 0 ) {
		if( current_system->variable_thrust == 1 && ( current_system->initial_thrust < current_system->max_thrust_n ) ) {
			if( current_thrust == 100 ) {
				current_system->initial_thrust += ( current_system->thrust_step ) / ( time_mod );
				thrust_newtons = ( long double )( ( current_system->initial_thrust * current_thrust ) / 100 );
			} else {
				thrust_newtons = ( long double )( current_thrust * current_system->initial_thrust ) / 100;
			}
		} else {
			thrust_newtons = ( long double )( current_thrust * current_system->max_thrust_n ) / 100;
		}
	} else {
		thrust_newtons = 0;
	}

	if( ROCKET_STAGE_get_attached( current_system ) == 1 ) {
		current_system->current_thrust = thrust_newtons;
	}

	if( ROCKET_ENGINE_get_thrust( &internal_guidance ) >= 60 ) {
		dynamic_pressure = get_dynamic_pressure_force( current_altitude );
	}

	dynamic_pressure_newtons = dynamic_pressure * 4.44;

	fw = 0;
	if( max_q_achieved == 0 ) {
		fw = thrust_newtons - ( total_mass * CELESTIAL_OBJECT_get_gravity_value( AO_current, current_altitude ) );
	} else {
		fw = thrust_newtons - ( total_mass * ( CELESTIAL_OBJECT_get_gravity_value( AO_current, current_altitude) / (time_mod) ) ) + dynamic_pressure_newtons * ( 1000 );
	}

	current_acceleration = ( fw / total_mass );

	if( current_acceleration < 0 && current_altitude <= 0 ) {
		current_acceleration = 0;
	}

	if( last_velocity >= CELESTIAL_OBJECT_get_orbital_speed( AO_current, current_altitude ) ) {
		if( current_acceleration < 0 ) {
			current_acceleration = 0;
		}
	}

	current_velocity = last_velocity + ( current_acceleration / ( time_mod ) );

	if( pitch_program.running == 1 ) {
		pitch_program.running_time += time_tick;
		pitch_program.current_value += get_pitch_step() / time_mod;
	}

	if( pitch_program.current_value > 0 ) {
		rad2deg = pitch_program.current_value * _PI / 180;
		current_vertical_velocity = round( ( (last_velocity + current_acceleration) - CELESTIAL_OBJECT_get_gravity_value( AO_current, current_altitude ) ) * cos( rad2deg ) );
		if( current_vertical_velocity < 0 ) {
			current_vertical_velocity = 0;
		}

		if( current_vertical_velocity == 0 ) {
			current_horizontal_velocity = current_velocity;
		} else {
			current_horizontal_velocity = round( last_velocity * sin( rad2deg ) );
		}
	} else {
		current_vertical_velocity = current_velocity;
	}

	current_distance = current_velocity;

	if( current_velocity > 0 ) {
		ascending_time += time_tick;
	} else {
		ascending_time = -1;
	}

	if( roll_program.running == 1 ) {
		roll_program.current_value += get_roll_step() / time_mod;
	}

	if( yaw_program.running == 1 ) {
		yaw_program.current_value += get_yaw_step() / time_mod;
	}

	if( current_distance > 0 ) {
		if( pitch_program.current_value <= 92 ) {
			pitch_mod = ( 100 - pitch_program.current_value ) / 100;
			if( current_system->id == 1 ) {
				current_altitude += ( ( ( current_distance ) * pitch_mod ) / ( time_mod ) );
			 } else {
				pitch_mod = ( 90.1 - pitch_program.current_value ) / 100;
				current_altitude += ( ( current_vertical_velocity ) * pitch_mod ) / ( time_mod );
			 }
		}
	}

	if( current_distance < 0 && current_acceleration < 0 && ROCKET_ENGINE_get_thrust( &internal_guidance ) < 100 && max_q_achieved == 0 ) {
		current_altitude += (current_distance / time_mod);
	}

	last_velocity = current_velocity;

	if( current_altitude > 0 ) {
		total_distance += ( current_distance / time_mod );
	} else {
		last_velocity = 0;
	}

	if( current_altitude < 0 ) {
		current_altitude = 0;
	}

	instrument_unit_calculations();
}

void TELEMETRY_update( void ) {
	telemetry_data.ascending_time = ascending_time;
	telemetry_data.current_acceleration = current_acceleration;
	telemetry_data.current_gforce = round( current_acceleration / 10 );
	telemetry_data.current_altitude = current_altitude;
	telemetry_data.current_distance = current_distance;
	telemetry_data.current_dynamic_pressure = current_dynamic_pressure;
	telemetry_data.current_fuel_burn = current_fuel_burn;
	telemetry_data.current_fuel_mass = current_fuel_mass;
	telemetry_data.current_horizontal_velocity = current_horizontal_velocity;
	telemetry_data.current_thrust = current_thrust;
	telemetry_data.current_velocity = current_velocity;
	telemetry_data.current_vertical_velocity = current_vertical_velocity;
	telemetry_data.last_velocity = last_velocity;
	telemetry_data.max_q_achieved = max_q_achieved;
	telemetry_data.mission_time = mission_time;
	strncpy( telemetry_data.current_time_gmt, get_actual_time_gmt(), TIME_BUFF_SIZE );
	telemetry_data.thrust_newtons = thrust_newtons;
	telemetry_data.total_distance = total_distance;
	telemetry_data.total_mass = total_mass;
	telemetry_data.pitch = pitch_program.current_value;
	telemetry_data.dest_pitch = pitch_program.dest_value;
	telemetry_data.roll = roll_program.current_value;
	telemetry_data.dest_roll = roll_program.dest_value;
	telemetry_data.yaw = yaw_program.current_value;
	strncpy( telemetry_data.destination, AO_current->ground_destination, SMALL_BUFF_SIZE );
	telemetry_data.destination_altitude = AO_current->ground_destination_altitude;

	telemetry_data.stable_orbit_achieved = stable_orbit_achieved;
	telemetry_data.launch_escape_tower_ready = launch_escape_tower_ready;

	telemetry_data.internal_guidance_engaged = ROCKET_ENGINE_get_engaged( &internal_guidance );
	telemetry_data.main_engine_engaged = ROCKET_ENGINE_get_engaged( &main_engine );

	telemetry_data.pitch_program_engaged = pitch_program.running;
	telemetry_data.roll_program_engaged = roll_program.running;
	telemetry_data.yaw_program_engaged = yaw_program.running;

	telemetry_data.s_ic_fuel = system_s1.fuel;
	telemetry_data.s_ic_attached = ROCKET_STAGE_get_attached( &system_s1 );
	telemetry_data.s_ic_thrust = system_s1.current_thrust;
	telemetry_data.s_ic_burn_time = system_s1.burn_time;
	telemetry_data.s_ic_center_engine_available = system_s1.center_engine_available;

	telemetry_data.s_ii_fuel = system_s2.fuel;
	telemetry_data.s_ii_attached = ROCKET_STAGE_get_attached( &system_s2 );
	telemetry_data.s_ii_thrust = system_s2.current_thrust;
	telemetry_data.s_ii_burn_time = system_s2.burn_time;
	telemetry_data.s_ii_center_engine_available = system_s2.center_engine_available;

	telemetry_data.s_ivb_fuel = system_s3.fuel;
	telemetry_data.s_ivb_attached = ROCKET_STAGE_get_attached( &system_s3 );
	telemetry_data.s_ivb_thrust = system_s3.current_thrust;
	telemetry_data.s_ivb_burn_time = system_s3.burn_time;
	telemetry_data.s_ivb_center_engine_available = system_s3.center_engine_available;
}
