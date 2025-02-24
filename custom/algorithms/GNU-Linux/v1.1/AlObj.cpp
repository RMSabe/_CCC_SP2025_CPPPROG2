/*
 * C++ Programming 2 Assignment 1
 *
 * Version 1.1 (GNU-Linux)
 *
 * Author: Rafael Margarido Sabe
 * CCC ID: 340378
 * Email: 340378@colum.edu
 */

#include "AlObj.hpp"
#include "cstrdef.h"

#include <string.h>

AlObj::AlObj(const char *filein_dir, const char *fileout_dir, size_t n_digits)
{
	this->setParameters(filein_dir, fileout_dir, n_digits);
}

AlObj::~AlObj(void)
{
	this->file_close();
	this->buffer_free();
}

bool AlObj::setParameters(const char *filein_dir, const char *fileout_dir, size_t n_digits)
{
	if(filein_dir == nullptr) return false;
	if(fileout_dir == nullptr) return false;
	if(!n_digits || (n_digits > this->N_DIGITS_MAX)) return false;

	this->filein_dir = filein_dir;
	this->fileout_dir = fileout_dir;
	this->n_digits = n_digits;

	return true;
}

bool AlObj::validateParameters(void)
{
	if(this->filein_dir == nullptr) return false;
	if(this->fileout_dir == nullptr) return false;
	if(!this->n_digits || (this->n_digits > this->N_DIGITS_MAX)) return false;

	return true;
}

bool AlObj::runAlgorithm(void)
{
	if(!this->validateParameters())
	{
		this->err_msg = "Error: invalid parameters.";
		return false;
	}

	if(!this->filein_open())
	{
		this->err_msg = "Error: could not open input file.";
		return false;
	}

	if(!this->buffer_alloc())
	{
		this->err_msg = "Error: memory allocation failed.";
		return false;
	}

	if(!this->proc_init())
	{
		this->err_msg = "Error: algorithm procedure failed.";
		return false;
	}

	if(!this->proc_loop())
	{
		this->err_msg = "Error: algorithm procedure failed.";
		return false;
	}

	if(!this->proc_saveresult()) return false;

	this->file_close();
	this->buffer_free();

	return true;
}

bool AlObj::fileout_create(void)
{
	if(this->fileout_dir == nullptr) return false;

	if(this->h_fileout >= 0) close(this->h_fileout); /*close existing file descriptor*/

	this->h_fileout = open(this->fileout_dir, (O_WRONLY | O_CREAT | O_TRUNC), (S_IRUSR | S_IWUSR));
	return (this->h_fileout >= 0);
}

bool AlObj::filein_open(void)
{
	if(this->filein_dir == nullptr) return false;

	if(this->h_filein >= 0) close(this->h_filein); /*close existing file descriptor*/

	this->h_filein = open(this->filein_dir, O_RDONLY);
	if(this->h_filein < 0) return false;

	this->filein_size = __LSEEK(this->h_filein, 0, SEEK_END);
	return true;
}

void AlObj::file_close(void)
{
	if(this->h_filein >= 0)
	{
		close(this->h_filein);
		this->h_filein = -1;
		this->filein_size = 0;
	}

	if(this->h_fileout >= 0)
	{
		close(this->h_fileout);
		this->h_fileout = -1;
	}

	return;
}

bool AlObj::buffer_alloc(void)
{
	this->buffer_free(); /*Clear any previous memory allocations*/

	this->bytebuffer_size = _get_closest_power2_ceil((size_t) this->filein_size);
	if(this->bytebuffer_size < 1u) return false;

	this->p_bytebuffer = (std::uint8_t*) std::malloc(this->bytebuffer_size);
	this->p_result_digits = (char*) std::malloc(this->n_digits + 1u);

	if(this->p_bytebuffer == nullptr) return false;
	if(this->p_result_digits == nullptr) return false;

	memset(this->p_bytebuffer, 0, this->bytebuffer_size);
	memset(this->p_result_digits, 0, (this->n_digits + 1u));

	return true;
}

void AlObj::buffer_free(void)
{
	if(this->p_bytebuffer != nullptr)
	{
		std::free(this->p_bytebuffer);
		this->p_bytebuffer = nullptr;
		this->bytebuffer_size = 0u;
	}

	if(this->p_result_digits != nullptr)
	{
		std::free(this->p_result_digits);
		this->p_result_digits = nullptr;
	}

	return;
}

bool AlObj::proc_init(void)
{
	memset(this->p_bytebuffer, 0u, this->bytebuffer_size);

	__LSEEK(this->h_filein, 0, SEEK_SET);
	read(this->h_filein, this->p_bytebuffer, (size_t) this->filein_size);

	return true;
}

bool AlObj::proc_loop(void)
{
	std::uint64_t _u64 = 0u;
	size_t bytebuffer_index_start = 0u;
	size_t bytebuffer_index_rel = 0u;
	size_t str_index = 0u;
	char _char = 0;
	std::uint8_t _byte = 0u;

	while((bytebuffer_index_start + this->n_digits) < this->bytebuffer_size)
	{
		this->str = "";
		str_index = 0u;
		bytebuffer_index_rel = bytebuffer_index_start;

		while(str_index < this->n_digits)
		{
			if(bytebuffer_index_rel >= this->bytebuffer_size) break;

			_byte = this->p_bytebuffer[bytebuffer_index_rel];

			if(!std::isdigit((char) _byte))
			{
				bytebuffer_index_rel++;
				continue;
			}

			this->str += (char) _byte;
			str_index++;
			bytebuffer_index_rel++;
		}

		_u64 = 1u;
		for(str_index = 0u; str_index < ((size_t) this->str.length()); str_index++)
		{
			_char = this->str[str_index];
			_char &= 0xf; /*Turn digit character value into digit numeric value*/

			_u64 *= (std::uint64_t) _char;
		}

		if(_u64 > this->result_value)
		{
			this->result_value = _u64;
			this->result_start_index = bytebuffer_index_start;
			snprintf(this->p_result_digits, (this->n_digits + 1u), "%s", this->str.c_str());
		}

		bytebuffer_index_start++;
	}

	return true;
}

bool AlObj::proc_saveresult(void)
{
	this->str = "Algorithm: greatest product from " + std::to_string(this->n_digits) + " consecutive digits\n";
	this->str += "Version: 1.1 (GNU-Linux)\n\n";

	this->str += "Product value: " + std::to_string(this->result_value) + "\n";
	this->str += "Digit Sequence: \"";
	this->str += this->p_result_digits;
	this->str += "\"\nDigit Sequence Start Index: " + std::to_string(this->result_start_index) + "\n";

	if(!this->fileout_create())
	{
		this->err_msg = "Error: could not create output file\n";
		return false;
	}

	__LSEEK(this->h_fileout, 0, SEEK_SET);
	write(this->h_fileout, this->str.c_str(), (size_t) this->str.length());

	return true;
}

