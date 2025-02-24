/*
	C++ Programming 2 Assignment 1

	Version 1.1 (Windows)

	Author: Rafael Margarido Sabe
	CCC ID: 340378
	Email: 340378@colum.edu
*/

#include "globldef.h"
#include "cstrdef.h"
#include "console.h"
#include "strdef.hpp"

#include "shared.hpp"
#include "AlObj.hpp"

#define __FILEIN_DIR TEXT("data.txt")
#define __FILEOUT_DIR TEXT("result.txt")

#define __N_DIGITS 13U

AlObj *p_algorithm = nullptr;

extern BOOL WINAPI app_init(VOID);
extern VOID WINAPI app_deinit(VOID);

INT main(VOID)
{
	if(!app_init()) goto _l_main_error;

	p_algorithm = new AlObj(__FILEIN_DIR, __FILEOUT_DIR, __N_DIGITS);
	if(p_algorithm == nullptr)
	{
		console_wait_keypress(TEXT("Error: algorithmic object instance failed\r\n"));
		goto _l_main_error;
	}

	if(!p_algorithm->runAlgorithm())
	{
		console_wait_keypress(p_algorithm->err_msg.c_str());
		goto _l_main_error;
	}

	console_wait_keypress(TEXT("Algorithm Processing Finished\r\n"));

	delete p_algorithm;
	p_algorithm = nullptr;

	app_deinit();
	return 0;

_l_main_error:
	app_deinit();
	return 1;
}

BOOL WINAPI app_init(VOID)
{
	p_processheap = GetProcessHeap();
	if(p_processheap == nullptr)
	{
		__SPRINTF(textbuf, TEXTBUF_SIZE_CHARS, TEXT("Error: Invalid Process Heap."));
		goto _l_app_init_error;
	}

	if(!console_init())
	{
		__SPRINTF(textbuf, TEXTBUF_SIZE_CHARS, TEXT("Error: Console Init Failed."));
		goto _l_app_init_error;
	}

	return TRUE;

_l_app_init_error:
	MessageBox(NULL, textbuf, TEXT("INIT ERROR"), (MB_ICONSTOP | MB_OK));
	return FALSE;
}

VOID WINAPI app_deinit(VOID)
{
	if(p_algorithm != nullptr)
	{
		delete p_algorithm;
		p_algorithm = nullptr;
	}

	console_deinit();
	return;
}

__declspec(noreturn) VOID WINAPI app_exit(UINT exit_code, const TCHAR *exit_msg)
{
	if(exit_msg != nullptr) MessageBox(NULL, exit_msg, TEXT("PROCESS EXIT CALLED"), (MB_ICONSTOP | MB_OK));

	app_deinit();
	ExitProcess(exit_code);

	while(TRUE) Sleep(1u); /*Not really necessary, but just to be safe.*/
}

INT WINAPI debug_msgbox(const TCHAR *text, UINT type)
{
	return MessageBox(NULL, text, TEXT("DEBUG"), type);
}
