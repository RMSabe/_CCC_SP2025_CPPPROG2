/*
 * C++ Programming 2 Assignment 2:
 * Library Management Software Project
 * Version 1.1 (GNU-Linux)
 *
 * Author: Rafael Margarido Sabe
 * CCC ID: 340378
 * Email: 340378@colum.edu
 */

#ifndef LIBRARYMANAGER_HPP
#define LIBRARYMANAGER_HPP

#include "globldef.h"
#include "filedef.h"
#include "cstrdef.h"

#include "strdef.hpp"

/*
 * Book Entry File Encoding:
 *
 * ENTRY:
 * uint64_t index
 * uint32_t size
 * int32_t status
 * int64_t renter_id
 * char[] (title + '\0')
 * char[] (author + '\0')
 *
 * User Entry File Encoding:
 *
 * ENTRY:
 * uint32_t index
 * uint32_t size
 * int64_t id
 * char[] (name + '\0')
 */

struct _bookentry {
	std::string title;
	std::string author;
	std::int32_t status;
	std::int64_t renter_id;
};

struct _userentry {
	std::string name;
	std::int64_t id;
};

struct _fileobj {
	const char *dir;
	__offset size;
	__offset pos;
	int fd;
};

typedef struct _bookentry bookentry_t;
typedef struct _userentry userentry_t;
typedef struct _fileobj fileobj_t;

class LibraryManager {
	public:
		LibraryManager(void);
		~LibraryManager(void);

		/*
		 * Initialize the files, make sure everything is OK.
		 * If some procedure fails at any point, try to call initialize to restore class activity.
		 *
		 * returns true if successful, false otherwise.
		 */
		bool initialize(void);

		/*
		 * Add/Remove Books
		 *
		 * returns true if successful, false otherwise.
		 */
		bool addNewBook(std::string title, std::string author);
		bool removeBook(std::int64_t index);

		/*
		 * Retrieve a book info
		 * returns true if successful, false otherwise.
		 *
		 * p_info, p_title, p_author, p_status and p_renterid are pointers that receive data.
		 * They may be set to NULL if not used.
		 *
		 * p_info is the user friendly description of the book (title, author, current status, current renter)
		 * all other pointers receive the absolute data from the file.
		 */
		bool getBookInfo(std::int64_t index, std::string *p_info, std::string *p_title, std::string *p_author, int *p_status, std::int64_t *p_renterid);

		/*
		 * List Books.
		 * returns true if successful, false otherwise.
		 */

		bool listBooks(std::string *p_output);					/*List All Books*/
		bool listBooksByTitle(std::string title, std::string *p_output);	/*List books with matching title*/
		bool listBooksByAuthor(std::string author, std::string *p_output);	/*List books with matching author*/
		bool listBooksByStatus(int status, std::string *p_output);		/*List books with matching status*/
		bool listBooksByRenter(std::int64_t renter_id, std::string *p_output);	/*List books with matching renter id*/

		/*Rent/Return books. Returns true if successful, false otherwise.*/

		bool rentBook(std::int64_t index, std::int64_t renter_id);

		bool returnBook(std::int64_t index);				/*Return a book, no ID required.*/
		bool returnBook(std::int64_t index, std::int64_t renter_id);	/*Return a book. ID required to match the book renter's ID*/

		/*Get Book Count. returns book count if successful, -1 otherwise.*/
		std::int64_t getBookCount(void);

		/*Add/Remove users. Returns true if successful, false otherwise.*/

		bool addNewUser(std::string name, std::int64_t id);
		bool removeUserIndex(std::int32_t index);				/*Remove User by Index*/
		bool removeUserId(std::int64_t id);					/*Remove User by ID*/

		/*
		 * Return user info (by file index or by ID)
		 * returns true if successful, false otherwise.
		 *
		 * pointer arguments are pointers that receive data.
		 * They may be set to NULL if not used.
		 *
		 * p_info receives a user friendly description of the user.
		 * all other pointers receive the absolute data from the file.
		 */

		bool getUserInfoIndex(std::int32_t index, std::string *p_info, std::string *p_name, std::int64_t *p_id);
		bool getUserInfoId(std::int64_t id, std::string *p_info, std::string *p_name, std::int32_t *p_index);

		/*List users. returns true if successful, false otherwise.*/

		bool listUsers(std::string *p_output);				/*List all users*/
		bool listUsersByName(std::string name, std::string *p_output);	/*List users with matching name*/

		/*Get User Count. returns user count if successful, -1 otherwise*/
		std::int32_t getUserCount(void);

		int getStatus(void);
		std::string getLastErrorMessage(void);

		enum Status {
			STATUS_ERROR_BROKENFILE = -4,
			STATUS_ERROR_MEMALLOC = -3,
			STATUS_ERROR_FILEOP = -2,
			STATUS_ERROR_GENERIC = -1,
			STATUS_UNINITIALIZED = 0,
			STATUS_OK = 1
		};

		enum BookEntryStatus {
			BOOKENTRY_STATUS_MIN = -1,
			BOOKENTRY_STATUS_VOID = -1,
			BOOKENTRY_STATUS_AVAILABLE = 0,
			BOOKENTRY_STATUS_RENTED = 1,
			BOOKENTRY_STATUS_MAX = 1
		};

	private:
		const char *BOOKENTRYFILE_DIR = "book.dat";
		const char *USERENTRYFILE_DIR = "user.dat";

		const std::uint32_t BOOKENTRY_MINSIZE = 26u;
		const std::uint32_t USERENTRY_MINSIZE = 17u;

		int status = this->STATUS_UNINITIALIZED;

		fileobj_t fobj_bookentry = {
			.dir = this->BOOKENTRYFILE_DIR,
			.size = 0,
			.pos = 0,
			.fd = -1
		};

		fileobj_t fobj_userentry = {
			.dir = this->USERENTRYFILE_DIR,
			.size = 0,
			.pos = 0,
			.fd = -1
		};

		std::string err_msg = "";

		bool fileobj_exists(const fileobj_t *p_fobj);
		bool fileobj_create(fileobj_t *p_fobj);
		bool fileobj_open(fileobj_t *p_fobj);
		void fileobj_close(fileobj_t *p_fobj);
		bool fileobj_save(fileobj_t *p_fobj);

		bool fobj_bookentry_verify(void);
		bool fobj_bookentry_cleanup(void);
		std::int64_t fobj_bookentry_get_entry_count(__offset *p_file_eod_index);
		bool fobj_bookentry_add_entry(const bookentry_t *p_book);
		bool fobj_bookentry_remove_entry(std::int64_t index);
		bool fobj_bookentry_get_entry_filepos(std::int64_t index, __offset *p_filepos);
		bool fobj_bookentry_set_entry_status(std::int64_t index, int status);
		bool fobj_bookentry_set_entry_renterid(std::int64_t index, std::int64_t renter_id);
		bool fobj_bookentry_get_entry(std::int64_t index, bookentry_t *p_book);

		bool fobj_userentry_verify(void);
		bool fobj_userentry_cleanup(void);
		std::int32_t fobj_userentry_get_entry_count(__offset *p_file_eod_index);
		bool fobj_userentry_add_entry(const userentry_t *p_user);
		bool fobj_userentry_remove_entry(std::int32_t index);
		bool fobj_userentry_get_entry_filepos(std::int32_t index, __offset *p_filepos);
		bool fobj_userentry_get_entry(std::int32_t index, userentry_t *p_user);
		int fobj_userentry_user_id_exists(std::int64_t user_id, std::int32_t *p_index);
};

#endif /*LIBRARYMANAGER_HPP*/

