/*
 * C++ Programming 2 Assignment 1
 *
 * Version 1.1 (GNU-Linux)
 *
 * Author: Rafael Margarido Sabe
 * CCC ID: 340378
 * Email: 340378@colum.edu
 */

#include "globldef.h"
#include "delay.h"
#include "cstrdef.h"
#include "strdef.hpp"

#include "shared.hpp"
#include "AlObj.hpp"

#include <iostream>

#define __FILEIN_DIR "data.txt"
#define __FILEOUT_DIR "result.txt"
#define __N_DIGITS 13U

AlObj *p_algorithm = nullptr;

/*app_init and app_deinit might be empty functions, but I'll still declare and use them just for protocol...*/

extern bool app_init(void);
extern void app_deinit(void);

int main(void)
{
	if(!app_init()) goto _l_main_error;

	p_algorithm = new AlObj(__FILEIN_DIR, __FILEOUT_DIR, __N_DIGITS);
	if(p_algorithm == nullptr)
	{
		std::cout << "Error: algorithmic object instance failed\n";
		goto _l_main_error;
	}

	if(!p_algorithm->runAlgorithm())
	{
		std::cout << p_algorithm->err_msg << std::endl;
		goto _l_main_error;
	}

	std::cout << "Algorithm Process Finished\n";

	delete p_algorithm;
	p_algorithm = nullptr;

	app_deinit();
	return 0;

_l_main_error:
	app_deinit();
	return 1;
}

bool app_init(void)
{
	return true;
}

void app_deinit(void)
{
	if(p_algorithm != nullptr)
	{
		delete p_algorithm;
		p_algorithm = nullptr;
	}

	return;
}

void __attribute__((__noreturn__)) app_exit(int exit_code, const char *exit_msg)
{
	if(exit_msg != nullptr) std::cout << "\nPROCESS EXIT CALLED\n" << exit_msg << std::endl;

	app_deinit();
	exit(exit_code);

	while(true) delay_ms(1); /*Not really necessary, but just to be safe.*/
}

