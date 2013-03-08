/*******************************************************************

Projekt Saturn V Main Computer

Plik: spacecraft_components.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef SPACECRAFT_COMPONENTS_H
#define SPACECRAFT_COMPONENTS_H

#define MAX_THRUST					100.0

/**
 Program komputerowy
**/
typedef struct COMPUTER_PROGRAM		COMPUTER_PROGRAM;
struct COMPUTER_PROGRAM {
	short running;
	long int start_time;
	double running_time;
	char name[ SMALL_BUFF_SIZE ];
	double current_value;
	double dest_value;

};

char* COMPUTER_PROGRAM_get_possible_action( COMPUTER_PROGRAM *cp );
void COMPUTER_PROGRAM_start( COMPUTER_PROGRAM *cp );
void COMPUTER_PROGRAM_stop( COMPUTER_PROGRAM *cp );


/**
 Człon rakiety
**/
typedef struct ROCKET_STAGE			ROCKET_STAGE;
struct ROCKET_STAGE {
	short id;
	long double fuel;
	short attached;
	double staging_time;
	int dry_mass;
	long double max_fuel;
	int instrument_mass;
	int max_fuel_burn;
	long int max_thrust_n;
	long int current_thrust;
	short variable_thrust;
	long double initial_thrust;
	long double thrust_step;
	int burn_start;
	double burn_time;
	short center_engine_available;
	char name[ MICRO_BUFF_SIZE ];
};

int ROCKET_STAGE_get_burn_time( ROCKET_STAGE *rs );
int ROCKET_STAGE_get_fuel( ROCKET_STAGE *rs );
int ROCKET_STAGE_get_total_mass( ROCKET_STAGE *rs );
char* ROCKET_STAGE_get_fuel_possible_action( ROCKET_STAGE *rs );
short ROCKET_STAGE_get_attached( ROCKET_STAGE *rs );
void ROCKET_STAGE_do_attach( ROCKET_STAGE *rs );
void ROCKET_STAGE_do_detach( ROCKET_STAGE *rs );
void ROCKET_STAGE_set_fuel( ROCKET_STAGE *rs, long double value, short do_mod );

/**
 Silnik
**/
typedef struct ROCKET_ENGINE		ROCKET_ENGINE;
struct ROCKET_ENGINE {
	short engaged;
	int thrust;
	char* name[ SMALL_BUFF_SIZE ];
};

void ROCKET_ENGINE_do_engage( ROCKET_ENGINE *re );
void ROCKET_ENGINE_do_disengage( ROCKET_ENGINE *re );
void ROCKET_ENGINE_set_thrust( ROCKET_ENGINE *re, int value );
int ROCKET_ENGINE_get_thrust( ROCKET_ENGINE *re );
short ROCKET_ENGINE_get_engaged( ROCKET_ENGINE *re );

/**
 Człony rakiety
**/
ROCKET_STAGE system_s1;
ROCKET_STAGE system_s2;
ROCKET_STAGE system_s3;
ROCKET_STAGE system_null;
ROCKET_STAGE *current_system;

void STAGE_1_init( void );
void STAGE_2_init( void );
void STAGE_3_init( void );
void STAGE_null_init( void );
void STAGES_init( void );

/**
 Programy komputerowe
**/

COMPUTER_PROGRAM pitch_program;
COMPUTER_PROGRAM roll_program;
COMPUTER_PROGRAM yaw_program;

void PITCH_init( void );
void ROLL_init( void );
void YAW_init( void );
void COMPUTER_PROGRAMS_init( void );

#endif
