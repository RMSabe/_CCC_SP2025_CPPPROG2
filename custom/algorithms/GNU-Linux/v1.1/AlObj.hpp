/*
 * C++ Programming 2 Assignment 1
 *
 * Version 1.1 (GNU-Linux)
 *
 * Author: Rafael Margarido Sabe
 * CCC ID: 340378
 * Email: 340378@colum.edu
 */

#ifndef ALOBJ_HPP
#define ALOBJ_HPP

/*Define Algorithmic Object*/

#include "globldef.h"
#include "filedef.h"
#include "strdef.hpp"

#include "shared.hpp"

class AlObj {
	public:
		AlObj(const char *filein_dir, const char *fileout_dir, size_t n_digits);
		~AlObj(void);

		bool setParameters(const char *filein_dir, const char *fileout_dir, size_t n_digits);
		bool validateParameters(void);

		bool runAlgorithm(void);

		std::string err_msg = "";

	private:
		int h_filein = -1;
		int h_fileout = -1;

		__offset filein_size = 0;

		const char *filein_dir = nullptr;
		const char *fileout_dir = nullptr;

		size_t bytebuffer_size = 0u;
		std::uint8_t *p_bytebuffer = nullptr;

		const size_t N_DIGITS_MAX = 20u; /*Sequences above 20 digits could overflow an uint64*/
		size_t n_digits = 0u;

		std::uint64_t result_value = 0u;
		size_t result_start_index = 0u;
		char *p_result_digits = nullptr;

		std::string str = "";

		bool fileout_create(void);
		bool filein_open(void);
		void file_close(void);

		bool buffer_alloc(void);
		void buffer_free(void);

		bool proc_init(void);
		bool proc_loop(void);
		bool proc_saveresult(void);
};

#endif /*ALOBJ_HPP*/

