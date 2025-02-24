/*
	C++ Programming 2 Assignment 1

	Version 1.1 (Windows)

	Author: Rafael Margarido Sabe
	CCC ID: 340378
	Email: 340378@colum.edu
*/

#ifndef SHARED_HPP
#define SHARED_HPP

#include "globldef.h"

/*These functions might not be used in the application, but it's good to have them available...*/

extern __declspec(noreturn) VOID WINAPI app_exit(UINT exit_code, const TCHAR *exit_msg);
extern INT WINAPI debug_msgbox(const TCHAR *text, UINT type);

#endif /*SHARED_HPP*/
