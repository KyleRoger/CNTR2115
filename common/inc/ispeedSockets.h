/*
*  FILE          :	ipeedSockets.h
*  SYSTEM PROJECT:	ispeed
*  PROGRAMMER    :	Arie Kraayenbrink
*  Editors	 	 :	Arie Kraayenbrink
*  FIRST VERSION :	Jan. 8, 2019
*  DESCRIPTION   :	This file contains the files, constants function prototype and even ...
*/



/********PRE-PROCESSORS*******/
#ifdef _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN	//disable winsock1.1 in windows.h
	#endif

	#include <windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <iphlpapi.h>	//for IP Helper APIs. May not be needed...

	#pragma comment(lib, "Ws2_32.lib")
#endif


#ifdef linux

#endif