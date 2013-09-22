/*******************************************************************

Projekt Saturn V Main Computer

Plik: portable.h

Przeznaczenie:
Dzi�ki definicjom w tym pliku mo�liwe jest ujednolicenie kodu dla r�nych platform.
Definicje, kt�re nie s� uniwersalne swoje miejsce znalaz�y w adekwatnych plikach �r�d�owychh.

Autor: Marcin Kelar ( marcin.kelar@gmail.com )
*******************************************************************/
#ifndef SERVER_PORTABLE_H
#define SERVER_PORTABLE_H

/* #undef _WIN32 */

#ifdef _WIN32
	#include <direct.h>
/* �cie�ki do plik�w/folder�w na systemach WIN32 s� oddzielone "\" */
	#define SLASH		"\\"
	#define C_SLASH		'\\'
/* Dla zachowania kompatybilno�ci z Linux */
/* Potoki - dla uruchamiania skrypt�w CGI */
	#define READ_BINARY	"rb"
	#define popen		_popen
	#define pclose		_pclose
/* Zmienne systemowe */
	#define putenv		_putenv
/* Operacje na plikach i folderach */
	#define chdir		_chdir
	#define mkdir( a,b )	mkdir( a )
	#define S_IREAD		_S_IREAD
/* W�a�ciwo�ci socket�w */
	#define fcntl		ioctlsocket
	#define close		closesocket
	#define SHUT_RDWR	SD_BOTH
	#define F_SETFL		FIONBIO
	#define S_IXOTH		S_IEXEC
	#define sighandler	__p_sig_fn_t
	#define MSG_NOSIGNAL	0
	#define sleep		Sleep

	#define APP_NAME	"Saturn V Computer (Win32)"
#else
	#define WSAGetLastError() errno
/* �cie�ki do plik�w/folder�w na systemach LINUX s� oddzielone "/" */
	#define SLASH		"/"
	#define C_SLASH		'/'
/* Dla zachowania kompatybilno�ci z WinSock ( ! )*/
	#define SOCKET_ERROR ( -1 )
	#define EXIT_FAILURE ( 1 )
	#define READ_BINARY	"re"
	#define sighandler	__sighandler_t
	#define Sleep(x)	usleep(x*1000)

	#define APP_NAME	"Saturn V Computer (Linux)"
#endif

#ifndef NI_MAXHOST
# define NI_MAXHOST			1025
#endif
#ifndef NI_MAXSERV
# define NI_MAXSERV			32
#endif
#ifndef NI_NUMERICHOST
# define NI_NUMERICHOST		1
#endif
#ifndef NI_NUMERICSERV
# define NI_NUMERICSERV		2
#endif
#ifndef NI_NOFQDN
# define NI_NOFQDN			4
#endif
#ifndef NI_NAMEREQD
# define NI_NAMEREQD		8
#endif
#ifndef NI_DGRAM
# define NI_DGRAM			16
#endif

#endif
