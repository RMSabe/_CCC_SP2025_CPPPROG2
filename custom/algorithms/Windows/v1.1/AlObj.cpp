/*
	C++ Programming 2 Assignment 1

	Version 1.1 (Windows)

	Author: Rafael Margarido Sabe
	CCC ID: 340378
	Email: 340378@colum.edu
*/

#include "AlObj.hpp"

AlObj::AlObj(const TCHAR *filein_dir, const TCHAR *fileout_dir, SIZE_T n_digits)
{
	this->setParameters(filein_dir, fileout_dir, n_digits);
}

AlObj::~AlObj(VOID)
{
	this->file_close();
	this->buffer_free();
}

BOOL WINAPI AlObj::setParameters(const TCHAR *filein_dir, const TCHAR *fileout_dir, SIZE_T n_digits)
{
	if(filein_dir == nullptr) return FALSE;
	if(fileout_dir == nullptr) return FALSE;
	if(!n_digits || (n_digits > this->N_DIGITS_MAX)) return FALSE;

	this->filein_dir = filein_dir;
	this->fileout_dir = fileout_dir;
	this->n_digits = n_digits;

	return TRUE;
}

BOOL WINAPI AlObj::validateParameters(VOID)
{
	if(this->filein_dir == nullptr) return FALSE;
	if(this->fileout_dir == nullptr) return FALSE;
	if(!this->n_digits || (this->n_digits > this->N_DIGITS_MAX)) return FALSE;

	return TRUE;
}

BOOL WINAPI AlObj::runAlgorithm(VOID)
{
	if(!this->validateParameters())
	{
		this->err_msg = TEXT("Error: invalid parameters.");
		return FALSE;
	}

	if(!this->filein_open())
	{
		this->err_msg = TEXT("Error: could not open input file.");
		return FALSE;
	}

	if(!this->buffer_alloc())
	{
		this->err_msg = TEXT("Error: memory allocation failed.");
		return FALSE;
	}

	if(!this->proc_init())
	{
		this->err_msg = TEXT("Error: algorithm process failed.");
		return FALSE;
	}

	if(!this->proc_loop())
	{
		this->err_msg = TEXT("Error: algorithm process failed.");
		return FALSE;
	}

	if(!this->proc_saveresult()) return FALSE;

	this->file_close();
	this->buffer_free();

	return TRUE;
}

BOOL WINAPI AlObj::fileout_create(VOID)
{
	if(this->fileout_dir == nullptr) return FALSE;

	if(this->h_fileout != INVALID_HANDLE_VALUE) CloseHandle(this->h_fileout); /*Close existing file descriptor.*/

	this->h_fileout = CreateFile(this->fileout_dir, (GENERIC_READ | GENERIC_WRITE), FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, 0);
	return (this->h_fileout != INVALID_HANDLE_VALUE);
}

BOOL WINAPI AlObj::filein_open(VOID)
{
	if(this->filein_dir == nullptr) return FALSE;

	if(this->h_filein != INVALID_HANDLE_VALUE) CloseHandle(this->h_filein); /*Close existing file descriptor.*/

	this->h_filein = CreateFile(this->filein_dir, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	if(this->h_filein == INVALID_HANDLE_VALUE) return FALSE;

	*this->p_filein_size_l32 = (ULONG32) GetFileSize(this->h_filein, (DWORD*) this->p_filein_size_h32);
	return TRUE;
}

VOID WINAPI AlObj::file_close(VOID)
{
	if(this->h_filein != INVALID_HANDLE_VALUE)
	{
		CloseHandle(this->h_filein);
		this->h_filein = INVALID_HANDLE_VALUE;
		this->filein_size_64 = 0u;
	}

	if(this->h_fileout != INVALID_HANDLE_VALUE)
	{
		CloseHandle(this->h_fileout);
		this->h_fileout = INVALID_HANDLE_VALUE;
	}

	return;
}

BOOL WINAPI AlObj::buffer_alloc(VOID)
{
	this->buffer_free(); /*Clear any previous buffer allocations.*/

	this->bytebuffer_size = _get_closest_power2_ceil((SIZE_T) filein_size_64);
	if(this->bytebuffer_size < 1u) return FALSE;

	this->p_bytebuffer = (UINT8*) HeapAlloc(p_processheap, HEAP_ZERO_MEMORY, this->bytebuffer_size);
	this->p_result_digits = (TCHAR*) HeapAlloc(p_processheap, HEAP_ZERO_MEMORY, (this->n_digits + 1u)*sizeof(TCHAR));

	if(this->p_bytebuffer == nullptr) return FALSE;
	if(this->p_result_digits == nullptr) return FALSE;

	return TRUE;
}

VOID WINAPI AlObj::buffer_free(VOID)
{
	if(this->p_bytebuffer != nullptr)
	{
		HeapFree(p_processheap, 0u, this->p_bytebuffer);
		this->p_bytebuffer = nullptr;
		this->bytebuffer_size = 0u;
	}

	if(this->p_result_digits != nullptr)
	{
		HeapFree(p_processheap, 0u, this->p_result_digits);
		this->p_result_digits = nullptr;
	}

	return;
}

BOOL WINAPI AlObj::proc_init(VOID)
{
	DWORD dummy_32;

	ZeroMemory(this->p_bytebuffer, this->bytebuffer_size);

	SetFilePointer(this->h_filein, 0, NULL, FILE_BEGIN);
	ReadFile(this->h_filein, this->p_bytebuffer, (DWORD) this->filein_size_64, &dummy_32, NULL);

	return TRUE;
}

BOOL WINAPI AlObj::proc_loop(VOID)
{
	ULONG64 _u64 = 0u;
	SIZE_T bytebuffer_index_start = 0u;
	SIZE_T bytebuffer_index_rel = 0u;
	SIZE_T str_index = 0u;
	TCHAR _tchar = 0;
	UINT8 _byte = 0u;

	while((bytebuffer_index_start + this->n_digits) < this->bytebuffer_size)
	{
		this->str = TEXT("");
		str_index = 0u;
		bytebuffer_index_rel = bytebuffer_index_start;

		while(str_index < this->n_digits)
		{
			if(bytebuffer_index_rel >= this->bytebuffer_size) break;

			_byte = this->p_bytebuffer[bytebuffer_index_rel];

			if((_byte < 0x30) || (_byte > 0x39)) /*Not a digit*/
			{
				bytebuffer_index_rel++;
				continue;
			}

			this->str += (TCHAR) _byte;
			str_index++;
			bytebuffer_index_rel++;
		}

		_u64 = 1u;
		for(str_index = 0u; str_index < ((SIZE_T) this->str.length()); str_index++)
		{
			_tchar = this->str[str_index];

			_tchar &= 0xf; /*Turn digit character value to digit numeric value.*/

			_u64 *= (ULONG64) _tchar;
		}

		if(_u64 > this->result_value)
		{
			this->result_value = _u64;
			this->result_start_index = bytebuffer_index_start;
			cstr_copy(this->str.c_str(), this->p_result_digits, (this->n_digits + 1u));
		}

		bytebuffer_index_start++;
	}

	return TRUE;
}

BOOL WINAPI AlObj::proc_saveresult(VOID)
{
	/*Even if application is using UTF-16, output file should be encoded in UTF-8 (const CHAR* / std::string)*/

	std::string _char_str = "";
	CHAR _text_result_digits[this->n_digits + 1u];
	DWORD dummy_32;

	cstr_tchar_to_char(this->p_result_digits, _text_result_digits, (this->n_digits + 1u));

	_char_str = "Algorithm: greatest product from " + std::to_string(this->n_digits) + " consecutive digits\r\n";
	_char_str += "Version 1.1 (Windows)\r\n\r\n";

	_char_str += "Product value: " + std::to_string(this->result_value) + "\r\n";
	_char_str += "Digit sequence: \"";
	_char_str += _text_result_digits;
	_char_str += "\"\r\nDigit sequence start index: " + std::to_string(this->result_start_index) + "\r\n";

	if(!this->fileout_create())
	{
		this->err_msg = TEXT("Error: could not create output file.");
		return FALSE;
	}

	SetFilePointer(this->h_fileout, 0, NULL, FILE_BEGIN);
	WriteFile(this->h_fileout, _char_str.c_str(), (DWORD) _char_str.length(), &dummy_32, NULL);

	return TRUE;
}
