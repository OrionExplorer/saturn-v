/*******************************************************************

Projekt Saturn V Main Computer

Plik: telemetry.c

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#include "include/telemetry.h"
#include "include/spacecraft_components.h"
#include "include/celestial_objects.h"

void TELEMETRY_prepare_data( char *dst, unsigned int dst_len, short no_cache ) {
	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();
	char *output;

	cJSON_AddTrueToObject( root, "success" );
	cJSON_AddStringToObject( root, "data_type", "telemetry" );
	cJSON_AddItemToObject( root, "data", data );

	if( no_cache == 1 || strncmp( cached_telemetry_data.computer_message, telemetry_data.computer_message, STD_BUFF_SIZE ) != 0 ) {
		cJSON_AddStringToObject( data, "computer_message", telemetry_data.computer_message );	
		strncpy( cached_telemetry_data.computer_message, telemetry_data.computer_message, STD_BUFF_SIZE );
	}
	
	if( no_cache == 1 || cached_telemetry_data.mission_time != telemetry_data.mission_time ) {
		cJSON_AddNumberToObject( data, "mission_time", telemetry_data.mission_time );
		cached_telemetry_data.mission_time = telemetry_data.mission_time;	
	}
	
	if( no_cache == 1 || strncmp( cached_telemetry_data.current_time_gmt, telemetry_data.current_time_gmt, TIME_BUFF_SIZE ) != 0 ) {
		cJSON_AddStringToObject( data, "current_time_gmt", telemetry_data.current_time_gmt );	
		strncpy( cached_telemetry_data.current_time_gmt, telemetry_data.current_time_gmt, TIME_BUFF_SIZE );
	}
	
	if( no_cache == 1 || cached_telemetry_data.current_fuel_mass != telemetry_data.current_fuel_mass ) {
		cJSON_AddNumberToObject( data, "current_fuel_mass", telemetry_data.current_fuel_mass );
		cached_telemetry_data.current_fuel_mass = telemetry_data.current_fuel_mass;
	}
	
	if( no_cache == 1 || cached_telemetry_data.total_mass != telemetry_data.total_mass ) {
		cJSON_AddNumberToObject( data, "total_mass", telemetry_data.total_mass );
		cached_telemetry_data.total_mass = telemetry_data.total_mass;
	}
	
	if( no_cache == 1 || cached_telemetry_data.thrust_newtons != telemetry_data.thrust_newtons ) {
		cJSON_AddNumberToObject( data, "thrust_newtons", telemetry_data.thrust_newtons );
		cached_telemetry_data.thrust_newtons = telemetry_data.thrust_newtons;
	}
	
	if( no_cache == 1 || cached_telemetry_data.current_acceleration != telemetry_data.current_acceleration ) {
		cJSON_AddNumberToObject( data, "current_acceleration", telemetry_data.current_acceleration );
		cached_telemetry_data.current_acceleration = telemetry_data.current_acceleration;
	}
	
	if( no_cache == 1 || cached_telemetry_data.current_gforce != telemetry_data.current_gforce ) {
		cJSON_AddNumberToObject( data, "current_gforce", telemetry_data.current_gforce );
		cached_telemetry_data.current_gforce = telemetry_data.current_gforce;
	}
	
	if( no_cache == 1 || cached_telemetry_data.current_distance != telemetry_data.current_distance ) {
		cJSON_AddNumberToObject( data, "current_distance", telemetry_data.current_distance );
		cached_telemetry_data.current_distance = telemetry_data.current_distance;
	}
	
	if( no_cache == 1 || cached_telemetry_data.current_velocity != telemetry_data.current_velocity ) {
		cJSON_AddNumberToObject( data, "current_velocity", telemetry_data.current_velocity );
		cached_telemetry_data.current_velocity = telemetry_data.current_velocity;
	}
	
	if( no_cache == 1 || cached_telemetry_data.current_vertical_velocity != telemetry_data.current_vertical_velocity ) {
		cJSON_AddNumberToObject( data, "current_vertical_velocity", telemetry_data.current_vertical_velocity );
		cached_telemetry_data.current_vertical_velocity = telemetry_data.current_vertical_velocity;
	}
	
	if( no_cache == 1 || cached_telemetry_data.current_horizontal_velocity != telemetry_data.current_horizontal_velocity ) {
		cJSON_AddNumberToObject( data, "current_horizontal_velocity", telemetry_data.current_horizontal_velocity );
		cached_telemetry_data.current_horizontal_velocity = telemetry_data.current_horizontal_velocity;
	}
	
	if( no_cache == 1 || cached_telemetry_data.current_fuel_burn != telemetry_data.current_fuel_burn ) {
		cJSON_AddNumberToObject( data, "current_fuel_burn", telemetry_data.current_fuel_burn );
		cached_telemetry_data.current_fuel_burn = telemetry_data.current_fuel_burn;
	}
	
	if( no_cache == 1 || cached_telemetry_data.current_thrust != telemetry_data.current_thrust ) {
		cJSON_AddNumberToObject( data, "current_thrust", telemetry_data.current_thrust );
		cached_telemetry_data.current_thrust = telemetry_data.current_thrust;
	}
	
	if( no_cache == 1 || cached_telemetry_data.current_altitude != telemetry_data.current_altitude ) {
		cJSON_AddNumberToObject( data, "current_altitude", telemetry_data.current_altitude );
		cached_telemetry_data.current_altitude = telemetry_data.current_altitude;
	}
	
	if( no_cache == 1 || cached_telemetry_data.total_distance != telemetry_data.total_distance ) {
		cJSON_AddNumberToObject( data, "total_distance", telemetry_data.total_distance );
		cached_telemetry_data.total_distance = telemetry_data.total_distance;
	}
	
	if( no_cache == 1 || cached_telemetry_data.downrange != telemetry_data.downrange ) {
		cJSON_AddNumberToObject( data, "downrange", telemetry_data.downrange );
		cached_telemetry_data.downrange = telemetry_data.downrange;
	}
	
	if( no_cache == 1 || cached_telemetry_data.last_velocity != telemetry_data.last_velocity ) {
		cJSON_AddNumberToObject( data, "last_velocity", telemetry_data.last_velocity );
		cached_telemetry_data.last_velocity = telemetry_data.last_velocity;
	}
	

	if( no_cache == 1 || cached_telemetry_data.max_q_achieved != telemetry_data.max_q_achieved ) {
		cJSON_AddNumberToObject( data, "max_q_achieved", telemetry_data.max_q_achieved );
		cached_telemetry_data.max_q_achieved = telemetry_data.max_q_achieved;
	}
	
	if( no_cache == 1 || cached_telemetry_data.mach_1_achieved != telemetry_data.mach_1_achieved ) {
		cJSON_AddNumberToObject( data, "mach_1_achieved", telemetry_data.mach_1_achieved );
		cached_telemetry_data.mach_1_achieved = telemetry_data.mach_1_achieved;
	}
	
	if( no_cache == 1 || cached_telemetry_data.current_dynamic_pressure != telemetry_data.current_dynamic_pressure ) {
		cJSON_AddNumberToObject( data, "current_dynamic_pressure", telemetry_data.current_dynamic_pressure );
		cached_telemetry_data.current_dynamic_pressure = telemetry_data.current_dynamic_pressure;
	}
	

	if( no_cache == 1 || cached_telemetry_data.orbital_velocity != telemetry_data.orbital_velocity ) {
		cJSON_AddNumberToObject( data, "orbital_velocity", telemetry_data.orbital_velocity );
		cached_telemetry_data.orbital_velocity = telemetry_data.orbital_velocity;
	}
	
	if( no_cache == 1 || cached_telemetry_data.stable_orbit_achieved != telemetry_data.stable_orbit_achieved ) {
		cJSON_AddNumberToObject( data, "stable_orbit_achieved", telemetry_data.stable_orbit_achieved );
		cached_telemetry_data.stable_orbit_achieved = telemetry_data.stable_orbit_achieved;
	}
	
	if( no_cache == 1 || cached_telemetry_data.orbit_semi_major_axis != telemetry_data.orbit_semi_major_axis ) {
		cJSON_AddNumberToObject( data, "orbit_semi_major_axis", telemetry_data.orbit_semi_major_axis );
		cached_telemetry_data.orbit_semi_major_axis = telemetry_data.orbit_semi_major_axis;
	}
	
	if( no_cache == 1 || cached_telemetry_data.orbit_semi_minor_axis != telemetry_data.orbit_semi_minor_axis ) {
		cJSON_AddNumberToObject( data, "orbit_semi_minor_axis", telemetry_data.orbit_semi_minor_axis );
		cached_telemetry_data.orbit_semi_minor_axis = telemetry_data.orbit_semi_minor_axis;
	}
	
	if( no_cache == 1 || cached_telemetry_data.orbit_eccentrity != telemetry_data.orbit_eccentrity ) {
		cJSON_AddNumberToObject( data, "orbit_eccentrity", telemetry_data.orbit_eccentrity );
		cached_telemetry_data.orbit_eccentrity = telemetry_data.orbit_eccentrity;
	}
	
	if( no_cache == 1 || cached_telemetry_data.orbit_apoapsis != telemetry_data.orbit_apoapsis ) {
		cJSON_AddNumberToObject( data, "orbit_apoapsis", telemetry_data.orbit_apoapsis );
		cached_telemetry_data.orbit_apoapsis = telemetry_data.orbit_apoapsis;
	}
	
	if( no_cache == 1 || cached_telemetry_data.orbit_apoapsis_velocity != telemetry_data.orbit_apoapsis_velocity ) {
		cJSON_AddNumberToObject( data, "orbit_apoapsis_velocity", telemetry_data.orbit_apoapsis_velocity );
		cached_telemetry_data.orbit_apoapsis_velocity = telemetry_data.orbit_apoapsis_velocity;
	}
	
	if( no_cache == 1 || cached_telemetry_data.orbit_periapsis != telemetry_data.orbit_periapsis ) {
		cJSON_AddNumberToObject( data, "orbit_periapsis", telemetry_data.orbit_periapsis );
		cached_telemetry_data.orbit_periapsis = telemetry_data.orbit_periapsis;
	}
	
	if( no_cache == 1 || cached_telemetry_data.orbit_periapsis_velocity != telemetry_data.orbit_periapsis_velocity ) {
		cJSON_AddNumberToObject( data, "orbit_periapsis_velocity", telemetry_data.orbit_periapsis_velocity );
		cached_telemetry_data.orbit_periapsis_velocity = telemetry_data.orbit_periapsis_velocity;
	}
	
	if( no_cache == 1 || cached_telemetry_data.orbit_revolution_period != telemetry_data.orbit_revolution_period ) {
		cJSON_AddNumberToObject( data, "orbit_revolution_period", telemetry_data.orbit_revolution_period );
		cached_telemetry_data.orbit_revolution_period = telemetry_data.orbit_revolution_period;
	}
	
	if( no_cache == 1 || cached_telemetry_data.orbit_revolution_duration != telemetry_data.orbit_revolution_duration ) {
		cJSON_AddNumberToObject( data, "orbit_revolution_duration", telemetry_data.orbit_revolution_duration );
		cached_telemetry_data.orbit_revolution_duration = telemetry_data.orbit_revolution_duration;
	}
	
	if( no_cache == 1 || cached_telemetry_data.orbit_inclination != telemetry_data.orbit_inclination ) {
		cJSON_AddNumberToObject( data, "orbit_inclination", telemetry_data.orbit_inclination );
		cached_telemetry_data.orbit_inclination = telemetry_data.orbit_inclination;
	}
	
	if( no_cache == 1 || cached_telemetry_data.orbit_circumference != telemetry_data.orbit_circumference ) {
		cJSON_AddNumberToObject( data, "orbit_circumference", telemetry_data.orbit_circumference );
		cached_telemetry_data.orbit_circumference = telemetry_data.orbit_circumference;
	}
	
	if( no_cache == 1 || cached_telemetry_data.orbit_mean_motion != telemetry_data.orbit_mean_motion ) {
		cJSON_AddNumberToObject( data, "orbit_mean_motion", telemetry_data.orbit_mean_motion );
		cached_telemetry_data.orbit_mean_motion = telemetry_data.orbit_mean_motion;
	}
	
	if( no_cache == 1 || cached_telemetry_data.launch_escape_tower_ready != telemetry_data.launch_escape_tower_ready ) {
		cJSON_AddNumberToObject( data, "launch_escape_tower_ready", telemetry_data.launch_escape_tower_ready );
		cached_telemetry_data.launch_escape_tower_ready = telemetry_data.launch_escape_tower_ready;
	}
	

	if( no_cache == 1 || cached_telemetry_data.auto_pilot_enabled != telemetry_data.auto_pilot_enabled ) {
		cJSON_AddNumberToObject( data, "auto_pilot_enabled", telemetry_data.auto_pilot_enabled );
		cached_telemetry_data.auto_pilot_enabled = telemetry_data.auto_pilot_enabled;
	}
	

	if( no_cache == 1 || cached_telemetry_data.pitch != telemetry_data.pitch ) {
		cJSON_AddNumberToObject( data, "pitch", telemetry_data.pitch );
		cached_telemetry_data.pitch = telemetry_data.pitch;
	}
	
	if( no_cache == 1 || cached_telemetry_data.dest_pitch != telemetry_data.dest_pitch ) {
		cJSON_AddNumberToObject( data, "dest_pitch", telemetry_data.dest_pitch );
		cached_telemetry_data.dest_pitch = telemetry_data.dest_pitch;
	}
	
	if( no_cache == 1 || cached_telemetry_data.roll != telemetry_data.roll ) {
		cJSON_AddNumberToObject( data, "roll", telemetry_data.roll );
		cached_telemetry_data.roll = telemetry_data.roll;
	}
	
	if( no_cache == 1 || cached_telemetry_data.dest_roll != telemetry_data.dest_roll ) {
		cJSON_AddNumberToObject( data, "dest_roll", telemetry_data.dest_roll );
		cached_telemetry_data.dest_roll = telemetry_data.dest_roll;
	}
	
	if( no_cache == 1 || cached_telemetry_data.yaw != telemetry_data.yaw ) {
		cJSON_AddNumberToObject( data, "yaw", telemetry_data.yaw );
		cached_telemetry_data.yaw = telemetry_data.yaw;
	}
	
	if( no_cache == 1 || cached_telemetry_data.iterative_guidance_mode_active != telemetry_data.iterative_guidance_mode_active ) {
		cJSON_AddNumberToObject( data, "iterative_guidance_mode_active", telemetry_data.iterative_guidance_mode_active );
		cached_telemetry_data.iterative_guidance_mode_active = telemetry_data.iterative_guidance_mode_active;
	}

	if( no_cache == 1 || cached_telemetry_data.holddown_arms_released != telemetry_data.holddown_arms_released ) {
		cJSON_AddNumberToObject( data, "holddown_arms_released", telemetry_data.holddown_arms_released );
		cached_telemetry_data.holddown_arms_released = telemetry_data.holddown_arms_released;
	}

	if( no_cache == 1 || cached_telemetry_data.countdown_in_progress != telemetry_data.countdown_in_progress ) {
		cJSON_AddNumberToObject( data, "countdown_in_progress", telemetry_data.countdown_in_progress );
		cached_telemetry_data.countdown_in_progress = telemetry_data.countdown_in_progress;
	}

	if( no_cache == 1 || cached_telemetry_data.internal_guidance_engaged != telemetry_data.internal_guidance_engaged ) {
		cJSON_AddNumberToObject( data, "internal_guidance_engaged", telemetry_data.internal_guidance_engaged );
		cached_telemetry_data.internal_guidance_engaged = telemetry_data.internal_guidance_engaged;
	}
	
	if( no_cache == 1 || cached_telemetry_data.main_engine_engaged != telemetry_data.main_engine_engaged ) {
		cJSON_AddNumberToObject( data, "main_engine_engaged", telemetry_data.main_engine_engaged );
		cached_telemetry_data.main_engine_engaged = telemetry_data.main_engine_engaged;
	}

	if( no_cache == 1 || cached_telemetry_data.pitch_program_engaged != telemetry_data.pitch_program_engaged ) {
		cJSON_AddNumberToObject( data, "pitch_program_engaged", telemetry_data.pitch_program_engaged );
		cached_telemetry_data.pitch_program_engaged = telemetry_data.pitch_program_engaged;
	}
	
	if( no_cache == 1 || cached_telemetry_data.roll_program_engaged != telemetry_data.roll_program_engaged ) {
		cJSON_AddNumberToObject( data, "roll_program_engaged", telemetry_data.roll_program_engaged );
		cached_telemetry_data.roll_program_engaged = telemetry_data.roll_program_engaged;
	}
	
	if( no_cache == 1 || cached_telemetry_data.yaw_program_engaged != telemetry_data.yaw_program_engaged ) {
		cJSON_AddNumberToObject( data, "yaw_program_engaged", telemetry_data.yaw_program_engaged );
		cached_telemetry_data.yaw_program_engaged = telemetry_data.yaw_program_engaged;
	}

	if( no_cache == 1 || cached_telemetry_data.s_ic_fuel != telemetry_data.s_ic_fuel ) {
		cJSON_AddNumberToObject( data, "s_ic_fuel", telemetry_data.s_ic_fuel );
		cached_telemetry_data.s_ic_fuel = telemetry_data.s_ic_fuel;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ic_attached != telemetry_data.s_ic_attached ) {
		cJSON_AddNumberToObject( data, "s_ic_attached", telemetry_data.s_ic_attached );
		cached_telemetry_data.s_ic_attached = telemetry_data.s_ic_attached;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ic_thrust != telemetry_data.s_ic_thrust ) {
		cJSON_AddNumberToObject( data, "s_ic_thrust", telemetry_data.s_ic_thrust );
		cached_telemetry_data.s_ic_thrust = telemetry_data.s_ic_thrust;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ic_burn_time != telemetry_data.s_ic_burn_time ) {
		cJSON_AddNumberToObject( data, "s_ic_burn_time", telemetry_data.s_ic_burn_time );
		cached_telemetry_data.s_ic_burn_time = telemetry_data.s_ic_burn_time;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ic_center_engine_available != telemetry_data.s_ic_center_engine_available ) {
		cJSON_AddNumberToObject( data, "s_ic_center_engine_available", telemetry_data.s_ic_center_engine_available );
		cached_telemetry_data.s_ic_center_engine_available = telemetry_data.s_ic_center_engine_available;
	}

	if( no_cache == 1 || cached_telemetry_data.s_ii_fuel != telemetry_data.s_ii_fuel ) {
		cJSON_AddNumberToObject( data, "s_ii_fuel", telemetry_data.s_ii_fuel );
		cached_telemetry_data.s_ii_fuel = telemetry_data.s_ii_fuel;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ii_attached != telemetry_data.s_ii_attached ) {
		cJSON_AddNumberToObject( data, "s_ii_attached", telemetry_data.s_ii_attached );
		cached_telemetry_data.s_ii_attached = telemetry_data.s_ii_attached;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ii_thrust != telemetry_data.s_ii_thrust ) {
		cJSON_AddNumberToObject( data, "s_ii_thrust", telemetry_data.s_ii_thrust );
		cached_telemetry_data.s_ii_thrust = telemetry_data.s_ii_thrust;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ii_burn_time != telemetry_data.s_ii_burn_time ) {
		cJSON_AddNumberToObject( data, "s_ii_burn_time", telemetry_data.s_ii_burn_time );
		cached_telemetry_data.s_ii_burn_time = telemetry_data.s_ii_burn_time;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ii_center_engine_available != telemetry_data.s_ii_center_engine_available ) {
		cJSON_AddNumberToObject( data, "s_ii_center_engine_available", telemetry_data.s_ii_center_engine_available );
		cached_telemetry_data.s_ii_center_engine_available = telemetry_data.s_ii_center_engine_available;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ii_interstage_mass != telemetry_data.s_ii_interstage_mass ) {
		cJSON_AddNumberToObject( data, "s_ii_interstage_mass", telemetry_data.s_ii_interstage_mass );
		cached_telemetry_data.s_ii_interstage_mass = telemetry_data.s_ii_interstage_mass;
	}

	if( no_cache == 1 || cached_telemetry_data.s_ivb_fuel != telemetry_data.s_ivb_fuel ) {
		cJSON_AddNumberToObject( data, "s_ivb_fuel", telemetry_data.s_ivb_fuel );
		cached_telemetry_data.s_ivb_fuel = telemetry_data.s_ivb_fuel;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ivb_attached != telemetry_data.s_ivb_attached ) {
		cJSON_AddNumberToObject( data, "s_ivb_attached", telemetry_data.s_ivb_attached );
		cached_telemetry_data.s_ivb_attached = telemetry_data.s_ivb_attached;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ivb_thrust != telemetry_data.s_ivb_thrust ) {
		cJSON_AddNumberToObject( data, "s_ivb_thrust", telemetry_data.s_ivb_thrust );
		cached_telemetry_data.s_ivb_thrust = telemetry_data.s_ivb_thrust;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ivb_burn_time != telemetry_data.s_ivb_burn_time ) {
		cJSON_AddNumberToObject( data, "s_ivb_burn_time", telemetry_data.s_ivb_burn_time );
		cached_telemetry_data.s_ivb_burn_time = telemetry_data.s_ivb_burn_time;
	}
	
	if( no_cache == 1 || cached_telemetry_data.s_ivb_center_engine_available != telemetry_data.s_ivb_center_engine_available ) {
		cJSON_AddNumberToObject( data, "s_ivb_center_engine_available", telemetry_data.s_ivb_center_engine_available );
		cached_telemetry_data.s_ivb_center_engine_available = telemetry_data.s_ivb_center_engine_available;
	}

	if( no_cache == 1 || cached_telemetry_data.active_stage != telemetry_data.active_stage ) {
		cJSON_AddNumberToObject( data, "active_stage", telemetry_data.active_stage );
		cached_telemetry_data.active_stage = telemetry_data.active_stage;
	}

	output = cJSON_Print( root );
	strncpy( dst, output, dst_len );
	cJSON_Delete( root );
	root = NULL;

	free( output );
	output = NULL;
}

void TELEMETRY_send_ondemand_data( CONNECTED_CLIENT *client) {
	char *output;

	output = ( char * )calloc( LARGE_BUFF_SIZE, sizeof( char ) );

	TELEMETRY_prepare_data( output, LARGE_BUFF_SIZE, client->no_cache );
	SOCKET_send( &communication_session_, client, output, -1 );
	if( client->no_cache == 1 ) {
		client->no_cache = 0;	
	}

	free( output );
	output = NULL;
}

/*
void* TELEMETRY_send_live_data(void* data)
Funkcja przesy³a do wszystkich pod³¹czonych klientów informacje o telemetrii. */
void* TELEMETRY_send_live_data( void* data ) {
	int i;

	while(1) {
		Sleep( 100 );
		for( i = 0; i < MAX_CLIENTS; i++ ) {
			TELEMETRY_update();
			if( connected_clients[ i ].socket_descriptor > 0 ) {
				if( connected_clients[ i ].authorized == 1 && connected_clients[ i ].binded == 1 ) {
					TELEMETRY_send_ondemand_data( &connected_clients[ i ] );
				}
			}
		}
	}
}

void TELEMETRY_update( void ) {
	strncpy( telemetry_data.current_time_gmt, TIME_get_gmt(), TIME_BUFF_SIZE );

	telemetry_data.pitch_program_engaged = pitch_program.running;
	telemetry_data.roll_program_engaged = roll_program.running;
	telemetry_data.yaw_program_engaged = yaw_program.running;

	telemetry_data.pitch = pitch_program.current_value;
	telemetry_data.dest_pitch = pitch_program.dest_value;
	telemetry_data.roll = roll_program.current_value;
	telemetry_data.dest_roll = roll_program.dest_value;
	telemetry_data.yaw = yaw_program.current_value;

	telemetry_data.internal_guidance_engaged = ROCKET_ENGINE_get_engaged( &internal_guidance );
	telemetry_data.main_engine_engaged = ROCKET_ENGINE_get_engaged( &main_engine );

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
	telemetry_data.s_ii_interstage_mass = system_s2.interstage_mass;

	telemetry_data.s_ivb_fuel = system_s3.fuel;
	telemetry_data.s_ivb_attached = ROCKET_STAGE_get_attached( &system_s3 );
	telemetry_data.s_ivb_thrust = system_s3.current_thrust;
	telemetry_data.s_ivb_burn_time = system_s3.burn_time;
	telemetry_data.s_ivb_center_engine_available = system_s3.center_engine_available;
}

