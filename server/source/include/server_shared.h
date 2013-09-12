/*******************************************************************

Projekt voyager7computer

Plik: server_shared.h

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef SERVER_SHARED_H
#define SERVER_SHARED_H

#include "server_portable.h"
#include "server_strings_util.h"
#include "server_time_util.h"
#include "server_mem_manager.h"
#include "cJSON.h"
#include <stdio.h>
#include <time.h>

#define FD_SETSIZE	1024

/* Poni¿sze definicje s¹ konieczne z powodu "FIXME" w implementacji biblioteki Ws2tcpip.h w MinGW */
#ifdef _WIN32
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0501
	#endif
/* Obs³uga socketów na systemach WIN32 jest w bibliotece WinSock */
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

#ifdef _MSC_VER
#pragma comment( lib, "WS2_32.lib" )
#endif

#ifndef _MSC_VER
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#ifndef _WIN32
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/select.h>
#endif

#define APP_VER								"0.4"
#define SERVER_NAME							APP_NAME"/"APP_VER

#define LOGS_PATH							""SLASH

#define DEFAULT_PORT						1217

#define MAX_BUFFER							65535
#define MAX_BUFFER_CHAR						65535*sizeof( char )
#define UPLOAD_BUFFER						16384
#define UPLOAD_BUFFER_CHAR					16384*sizeof( char )
#define LOG_BUFFER							128
#define LOG_BUFFER_CHAR						128*sizeof( char )
#define BIG_BUFF_SIZE						2048
#define BIG_BUFF_SIZE_CHAR					2048*sizeof( char )
#define MEDIUM_BUFF_SIZE					1024
#define MEDIUM_BUFF_SIZE_CHAR				1024*sizeof( char )
#define STD_BUFF_SIZE						256
#define STD_BUFF_SIZE_CHAR					256*sizeof( char )
#define TIME_BUFF_SIZE						30
#define TIME_BUFF_SIZE_CHAR					30*sizeof( char )
#define SMALL_BUFF_SIZE						32
#define SMALL_BUFF_SIZE_CHAR				32*sizeof( char )
#define TINY_BUFF_SIZE						16
#define TINY_BUFF_SIZE_CHAR					16*sizeof( char )
#define PROTO_BUFF_SIZE						10
#define PROTO_BUFF_SIZE_CHAR				10*sizeof( char )
#define MICRO_BUFF_SIZE						8
#define MICRO_BUFF_SIZE_CHAR				8*sizeof( char )
#define EXT_LEN								8
#define EXT_LEN_CHAR						8*sizeof( char )

#define MAX_PATH_LENGTH						1024
#define MAX_PATH_LENGTH_CHAR				1024*sizeof( char )
#define MAX_CLIENTS							FD_SETSIZE

#define RFC1123FMT							"%a, %d %b %Y %H:%M:%S GMT"
#define DATETIME							"%d-%m-%Y %H:%M:%S"

#define LOGIN_STR							"{\"success\":false,\"msg\":\"authorization_required\",\"data_type\":\"command_response\"}\0"
#define LOGIN_SUCCESS						"{\"success\":true,\"msg\":\"user_authorized\",\"data_type\":\"command_response\"}\0"
#define INVALID_JSON						"{\"success\":true,\"msg\":\"invalid_message_format\",\"data_type\":\"command_response\"}\0"
#define MAIN_COMPUTER_RESPONSE_TEMPLATE		"{\"success\":%s,\"msg\":\"%s\",\"data_type\":\"command_response\"}\0"
#define NEW_USER_STR						"{\"success\":true,\"msg\":\"%s\",\"data_type\":\"new_user\"}\0"
#define DEL_USER_STR						"{\"success\":true,\"msg\":\"%s\",\"data_type\":\"del_user\"}\0"

#define DEG2RAD								( _PI / 180 )
#define RAD2DEG								( 180 / _PI ) //_PI / 180

extern char									app_path[ MAX_PATH_LENGTH ];
extern char									app_auth[ SMALL_BUFF_SIZE ];

typedef struct COMMUNICATION_SESSION		COMMUNICATION_SESSION;
typedef struct CONNECTED_CLIENT				CONNECTED_CLIENT;
typedef struct INTERPRETER_RESULT			INTERPRETER_RESULT;
typedef struct TELEMETRY					TELEMETRY;
typedef struct SOCKET_INFO					SOCKET_INFO;
typedef enum vDEVICE						vDEVICE;
typedef enum vCOMMAND						vCOMMAND;
typedef enum vCONTROL_MODE					vCONTROL_MODE;
typedef enum vCONNECTION_TYPE				vCONNECTION_TYPE;
typedef enum vCONNECTION_STATUS				vCONNECTION_STATUS;
typedef enum FLIGHT_STATUS					FLIGHT_STATUS;


/* Struktura przechowuj¹ca informacjê zwrotn¹ z interpretera poleceñ */
struct INTERPRETER_RESULT {
	short success;
	char message[ BIG_BUFF_SIZE ];
};

/* Struktura przechowuje informacje o telemetrii */
struct TELEMETRY {
	char					computer_message[ STD_BUFF_SIZE ];
	int						current_fuel_mass;
	long double				total_mass;
	long double				thrust_newtons;
	double					current_acceleration;
	double					current_gforce;
	double					current_distance;
	double					current_velocity;
	double					current_vertical_velocity;
	double					current_horizontal_velocity;
	double					current_fuel_burn;
	int						current_thrust;
	double					current_altitude;
	double					total_distance;
	double					last_velocity;
	double					mission_time;
	char					current_time_gmt[ TIME_BUFF_SIZE ];
	short					max_q_achieved;
	int						current_dynamic_pressure;
	short					stable_orbit_achieved;
	double					orbit_semi_major_axis;
	double					orbit_eccentrity;
	double					orbit_periapsis;
	double					orbit_apoapsis;
	double					orbit_inclination;
	short					launch_escape_tower_ready;
	double					pitch;
	double					dest_pitch;
	double					roll;
	double					dest_roll;
	double					yaw;
	short					countdown_in_progress;
	short					holddown_arms_released;
	short					auto_pilot_enabled;
	char					destination[ SMALL_BUFF_SIZE ];
	int						destination_altitude;
	short					internal_guidance_engaged;
	short					main_engine_engaged;
	short					pitch_program_engaged;
	short					roll_program_engaged;
	short					yaw_program_engaged;
	long double				s_ic_fuel;
	long double				s_ii_fuel;
	long double				s_ivb_fuel;
	short					s_ic_attached;
	short					s_ii_attached;
	short					s_ivb_attached;
	long double				s_ic_thrust;
	long double				s_ii_thrust;
	long double				s_ivb_thrust;
	double					s_ic_burn_time;
	double					s_ii_burn_time;
	double					s_ivb_burn_time;
	short					s_ic_center_engine_available;
	short					s_ii_center_engine_available;
	short					s_ivb_center_engine_available;
	long double				s_ii_interstage_mass;
	short					active_stage;
};

/* G³ówna struktura, która bêdzie przechowywa³a wszystkie informacje o po³¹czonym kliencie */
struct COMMUNICATION_SESSION {
	struct sockaddr_in		address;
#ifdef _WIN32
	SOCKET					socket;
#else
	int						socket;
#endif
	fd_set					socket_data;
#ifdef _WIN32
	int						data_length;
#else
	socklen_t				data_length;
#endif
	int						socket_descriptor;
	char*					content[ MAX_BUFFER ];
	short					keep_alive;
};

#ifdef _WIN32
	extern WSADATA				wsk;
	extern SOCKET				socket_server;
#else
	extern int					socket_server;
#endif
extern int					addr_size;
extern int					active_port;
extern struct sockaddr_in	server_address;
extern int					ip_proto_ver;
extern COMMUNICATION_SESSION			communication_session_;
extern fd_set				master;
char*						server_get_remote_hostname( COMMUNICATION_SESSION *communication_session );
TELEMETRY					telemetry_data;
FLIGHT_STATUS				MAIN_FLIGHT_STATUS;

enum vCONTROL_MODE {
	COMPUTER,
	CONNECTION
};

enum vDEVICE {
	INTERNAL_GUIDANCE,
	S1,
	S2,
	S3,
	MAIN_ENGINE,
	THRUST,
	PITCH_PROGRAM,
	ROLL_PROGRAM,
	YAW_PROGRAM,
	LET,
	AUTO_PILOT,
	HOLDDOWN_ARMS,
	COUNTDOWN,
	PITCH_MOD,
	ROLL_MOD,
	YAW_MOD
};

enum vCOMMAND {
	START,
	STOP,
	TANK,
	ATTACH,
	DETACH,
	CENTER_ENGINE_CUTOFF,
	RESTART,
	FULL_THRUST,
	NULL_THRUST,
	INCREASE,
	DECREASE,
	JETTISON,
	INTERSTAGE_JETTISON
};

enum vCONNECTION_TYPE {
	CUNKNOWN,
	CWEBSOCKET,
	CSOCKET
};

enum vCONNECTION_STATUS {
	CDISCONNECTED,
	CCONNECTED,
	CPENDING
};

enum FLIGHT_STATUS {
	LAUNCH_FROM_EARTH,
	STABLE_ORBIT,
	TRANSFER_ORBIT
};

/* Struktura przechowująca informacje o sockecie */
struct SOCKET_INFO {
	vCONNECTION_TYPE		type;
	vCONNECTION_STATUS		connection_status;
};

/* Struktura przechowuje informacje o pod³¹czonym kliencie */
struct CONNECTED_CLIENT {
	int					socket_descriptor;
	SOCKET_INFO			socket_info;
	short				binded;
	short				authorized;
	vCONTROL_MODE		mode;
	char				name[ STD_BUFF_SIZE ];
};

CONNECTED_CLIENT			connected_clients[ MAX_CLIENTS ];

#endif
