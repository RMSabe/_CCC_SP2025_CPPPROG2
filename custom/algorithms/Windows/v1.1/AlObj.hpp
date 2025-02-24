/*
	C++ Programming 2 Assignment 1

	Version 1.1 (Windows)

	Author: Rafael Margarido Sabe
	CCC ID: 340378
	Email: 340378@colum.edu
*/

#ifndef ALOBJ_HPP
#define ALOBJ_HPP

/*Algorithmic Object Class*/

#include "globldef.h"
#include "cstrdef.h"
#include "strdef.hpp"

#include "shared.hpp"

class AlObj {
	public:
		AlObj(const TCHAR *filein_dir, const TCHAR *fileout_dir, SIZE_T n_digits);
		~AlObj(VOID);

		BOOL WINAPI setParameters(const TCHAR *filein_dir, const TCHAR *fileout_dir, SIZE_T n_digits);
		BOOL WINAPI validateParameters(VOID);
		BOOL WINAPI runAlgorithm(VOID);

		__string err_msg = TEXT("");

	private:
		HANDLE h_filein = INVALID_HANDLE_VALUE;
		HANDLE h_fileout = INVALID_HANDLE_VALUE;

		ULONG64 filein_size_64 = 0u;
		ULONG32* const p_filein_size_l32 = (ULONG32*) &this->filein_size_64;
		ULONG32* const p_filein_size_h32 = &((ULONG32*) &this->filein_size_64)[1];

		const TCHAR *filein_dir = nullptr;
		const TCHAR *fileout_dir = nullptr;

		SIZE_T bytebuffer_size = 0u;
		UINT8 *p_bytebuffer = nullptr;

		const SIZE_T N_DIGITS_MAX = 20u; /*Above that could cause an uint64 overflow.*/
		SIZE_T n_digits = 0u;

		__string str = TEXT("");

		ULONG64 result_value = 0u;
		SIZE_T result_start_index = 0u;
		TCHAR *p_result_digits = nullptr;

		BOOL WINAPI fileout_create(VOID);
		BOOL WINAPI filein_open(VOID);
		VOID WINAPI file_close(VOID);

		BOOL WINAPI buffer_alloc(VOID);
		VOID WINAPI buffer_free(VOID);

		BOOL WINAPI proc_init(VOID);
		BOOL WINAPI proc_loop(VOID);
		BOOL WINAPI proc_saveresult(VOID);
};

#endif /*ALOBJ_HPP*/
