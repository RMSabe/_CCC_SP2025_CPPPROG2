/*
 * C++ Programming 2 Assignment 2:
 * Library Management Software Project
 * Version 1.1 (GNU-Linux)
 *
 * Author: Rafael Margarido Sabe
 * CCC ID: 340378
 * Email: 340378@colum.edu
 */

#include "LibraryManager.hpp"
#include <string.h>

LibraryManager::LibraryManager(void)
{
	this->initialize();
}

LibraryManager::~LibraryManager(void)
{
	this->fileobj_close(&this->fobj_bookentry);
	this->fileobj_close(&this->fobj_userentry);
}

bool LibraryManager::initialize(void)
{
	if(this->fileobj_exists(&this->fobj_bookentry))
	{
		if(!this->fileobj_open(&this->fobj_bookentry))
		{
			this->status = this->STATUS_ERROR_FILEOP;
			this->err_msg = "initialize: Error: could not open book entry file.";
			return false;
		}
	}
	else
	{
		if(!this->fileobj_create(&this->fobj_bookentry))
		{
			this->status = this->STATUS_ERROR_FILEOP;
			this->err_msg = "initialize: Error: could not create book entry file.";
			return false;
		}
	}

	if(this->fileobj_exists(&this->fobj_userentry))
	{
		if(!this->fileobj_open(&this->fobj_userentry))
		{
			this->status = this->STATUS_ERROR_FILEOP;
			this->err_msg = "initialize: Error: could not open user entry file.";
			return false;
		}
	}
	else
	{
		if(!this->fileobj_create(&this->fobj_userentry))
		{
			this->status = this->STATUS_ERROR_FILEOP;
			this->err_msg = "initialize: Error: could not create user entry file.";
			return false;
		}
	}

	if(!this->fobj_bookentry_verify()) return false;
	if(!this->fobj_userentry_verify()) return false;

	this->status = this->STATUS_OK;
	return true;
}

bool LibraryManager::addNewBook(std::string title, std::string author)
{
	bookentry_t book;

	if(this->status < 1) return false;

	book.title = title;
	book.author = author;

	return this->fobj_bookentry_add_entry(&book);
}

bool LibraryManager::removeBook(std::int64_t index)
{
	if(this->status < 1) return false;

	return this->fobj_bookentry_remove_entry(index);
}

bool LibraryManager::getBookInfo(std::int64_t index, std::string *p_info, std::string *p_title, std::string *p_author, int *p_status, std::int64_t *p_renterid)
{
	bookentry_t book;

	if(this->status < 1) return false;

	if(!this->fobj_bookentry_get_entry(index, &book)) return false;

	if(p_info != NULL)
	{
		*p_info = ("Book Index: " + std::to_string(index) + '\n');
		*p_info += ("Book Title: \"" + book.title + "\"\n");
		*p_info += ("Book Author: \"" + book.author + "\"\n");

		switch(book.status)
		{
			case this->BOOKENTRY_STATUS_AVAILABLE:
				*p_info += "Book Status: available for rental";
				break;

			case this->BOOKENTRY_STATUS_RENTED:
				*p_info += "Book Status: rented\nRenter ID: " + std::to_string(book.renter_id);
				break;
		}
	}

	if(p_title != NULL) *p_title = book.title;
	if(p_author != NULL) *p_author = book.author;
	if(p_status != NULL) *p_status = book.status;
	if(p_renterid != NULL) *p_renterid = book.renter_id;

	return true;
}

bool LibraryManager::listBooks(std::string *p_output)
{
	std::int64_t n_books = 0;
	std::int64_t n_index = 0;
	std::string book_info = "";

	if(this->status < 1) return false;

	if(p_output == NULL)
	{
		this->err_msg = "listBooks: Error: given string object pointer is null.";
		return false;
	}

	n_books = this->fobj_bookentry_get_entry_count(NULL);
	if(n_books < 0) return false;

	if(n_books == 0)
	{
		*p_output = "There are currently no books on file.";
		return true;
	}

	*p_output = "There are currently " + std::to_string(n_books) + " on file.";

	for(n_index = 0; n_index < n_books; n_index++)
	{
		if(!this->getBookInfo(n_index, &book_info, NULL, NULL, NULL, NULL)) return false;

		*p_output += "\n\n";
		*p_output += book_info;
	}

	return true;
}

bool LibraryManager::listBooksByTitle(std::string title, std::string *p_output)
{
	std::int64_t n_books = 0;
	std::int64_t n_index = 0;
	std::int64_t n_matches = 0;
	std::string book_title = "";
	std::string book_info = "";
	std::string book_list = "";

	if(this->status < 1) return false;

	if(p_output == NULL)
	{
		this->err_msg = "listBooksByTitle: Error: given string object pointer is null.";
		return false;
	}

	n_books = this->fobj_bookentry_get_entry_count(NULL);
	if(n_books < 0) return false;

	title = str_toupper(title);

	n_index = 0;
	n_matches = 0;
	book_list = "";

	while(n_index < n_books)
	{
		if(!this->getBookInfo(n_index, &book_info, &book_title, NULL, NULL, NULL)) return false;

		if(!book_title.compare(title))
		{
			n_matches++;
			book_list += "\n\n";
			book_list += book_info;
		}

		n_index++;
	}

	if(n_matches == 0)
	{
		*p_output = "No results match the given title.";
		return true;
	}

	*p_output = std::to_string(n_matches) + " results found for the given title:";
	*p_output += book_list;

	return true;
}

bool LibraryManager::listBooksByAuthor(std::string author, std::string *p_output)
{
	std::int64_t n_books = 0;
	std::int64_t n_index = 0;
	std::int64_t n_matches = 0;
	std::string book_author = "";
	std::string book_info = "";
	std::string book_list = "";

	if(this->status < 1) return false;

	if(p_output == NULL)
	{
		this->err_msg = "listBooksByAuthor: Error: given string object pointer is null.";
		return false;
	}

	n_books = this->fobj_bookentry_get_entry_count(NULL);
	if(n_books < 0) return false;

	author = str_toupper(author);

	n_index = 0;
	n_matches = 0;
	book_list = "";

	while(n_index < n_books)
	{
		if(!this->getBookInfo(n_index, &book_info, NULL, &book_author, NULL, NULL)) return false;

		if(!book_author.compare(author))
		{
			n_matches++;
			book_list += "\n\n";
			book_list += book_info;
		}

		n_index++;
	}

	if(n_matches == 0)
	{
		*p_output = "No results match the given author.";
		return true;
	}

	*p_output = std::to_string(n_matches) + " results found for the given author:";
	*p_output += book_list;

	return true;
}

bool LibraryManager::listBooksByStatus(int status, std::string *p_output)
{
	std::int64_t n_books = 0;
	std::int64_t n_index = 0;
	std::int64_t n_matches = 0;
	int book_status = 0;
	std::string book_info = "";
	std::string book_list = "";

	if(this->status < 1) return false;

	if(p_output == NULL)
	{
		this->err_msg = "listBooksByStatus: Error: given string object pointer is null.";
		return false;
	}

	if((status > this->BOOKENTRY_STATUS_MAX) || (status < this->BOOKENTRY_STATUS_MIN))
	{
		this->err_msg = "listBooksByStatus: Error: given status is invalid.";
		return false;
	}

	n_books = this->fobj_bookentry_get_entry_count(NULL);
	if(n_books < 0) return false;

	n_index = 0;
	n_matches = 0;
	book_list = "";

	while(n_index < n_books)
	{
		if(!this->getBookInfo(n_index, &book_info, NULL, NULL, &book_status, NULL)) return false;

		if(book_status == status)
		{
			n_matches++;
			book_list += "\n\n";
			book_list += book_info;
		}

		n_index++;
	}

	if(n_matches == 0)
	{
		*p_output = "No results match the given status.";
		return true;
	}

	*p_output = std::to_string(n_matches) + " results match the given status:";
	*p_output += book_list;

	return true;
}

bool LibraryManager::listBooksByRenter(std::int64_t renter_id, std::string *p_output)
{
	std::int64_t n_books = 0;
	std::int64_t n_index = 0;
	std::int64_t n_matches = 0;
	std::int64_t book_renterid = 0;
	std::string book_info = "";
	std::string book_list = "";

	if(this->status < 1) return false;

	if(p_output == NULL)
	{
		this->err_msg = "listBooksByRenter: Error: given string object pointer is null.";
		return false;
	}

	if(renter_id < 0)
	{
		this->err_msg = "listBooksByRenter: Error: given renter ID is invalid.";
		return false;
	}

	n_books = this->fobj_bookentry_get_entry_count(NULL);
	if(n_books < 0) return false;

	n_index = 0;
	n_matches = 0;
	book_list = "";

	while(n_index < n_books)
	{
		if(!this->getBookInfo(n_index, &book_info, NULL, NULL, NULL, &book_renterid)) return false;

		if(book_renterid == renter_id)
		{
			n_matches++;
			book_list += "\n\n";
			book_list += book_info;
		}

		n_index++;
	}

	if(n_matches == 0)
	{
		*p_output = "No results match the given renter ID.";
		return true;
	}

	*p_output = std::to_string(n_matches) + " results match the given renter ID:";
	*p_output += book_list;

	return true;
}

bool LibraryManager::rentBook(std::int64_t index, std::int64_t renter_id)
{
	int n_ret = 0;
	int book_status = 0;

	if(this->status < 1) return false;

	if(!this->getBookInfo(index, NULL, NULL, NULL, &book_status, NULL)) return false;

	if(book_status != this->BOOKENTRY_STATUS_AVAILABLE)
	{
		this->err_msg = "rentBook: Error: requested book is currently not available.";
		return false;
	}

	n_ret = this->fobj_userentry_user_id_exists(renter_id, NULL);
	if(n_ret < 0) return false;

	if(!n_ret)
	{
		this->err_msg = "rentBook: Error: given renter id is not registered on file. Add new user to register renter id.";
		return false;
	}

	if(!this->fobj_bookentry_set_entry_renterid(index, renter_id)) return false;
	if(!this->fobj_bookentry_set_entry_status(index, this->BOOKENTRY_STATUS_RENTED)) return false;

	return true;
}

bool LibraryManager::returnBook(std::int64_t index)
{
	int book_status = 0;

	if(this->status < 1) return false;

	if(!this->getBookInfo(index, NULL, NULL, NULL, &book_status, NULL)) return false;

	if(book_status != this->BOOKENTRY_STATUS_RENTED)
	{
		this->err_msg = "returnBook: Error: book is not marked as rented.";
		return false;
	}

	if(!this->fobj_bookentry_set_entry_status(index, this->BOOKENTRY_STATUS_AVAILABLE)) return false;
	if(!this->fobj_bookentry_set_entry_renterid(index, -1)) return false;

	return true;
}

bool LibraryManager::returnBook(std::int64_t index, std::int64_t renter_id)
{
	int n_ret = 0;
	int book_status = 0;
	std::int64_t book_renterid = 0;

	if(this->status < 1) return false;

	if(!this->getBookInfo(index, NULL, NULL, NULL, &book_status, &book_renterid)) return false;

	n_ret = this->fobj_userentry_user_id_exists(renter_id, NULL);
	if(n_ret < 0) return false;

	if(book_status != this->BOOKENTRY_STATUS_RENTED)
	{
		this->err_msg = "returnBook: Error: book is not marked as rented.";
		return false;
	}

	if(!n_ret)
	{
		this->err_msg = "returnBook: Error: given user ID is not registered on file. Add new user to register user ID.";
		return false;
	}

	if(book_renterid != renter_id)
	{
		this->err_msg = "returnBook: Error: book was not rented by the given user ID. Cannot be returned.";
		return false;
	}

	if(!this->fobj_bookentry_set_entry_status(index, this->BOOKENTRY_STATUS_AVAILABLE)) return false;
	if(!this->fobj_bookentry_set_entry_renterid(index, -1)) return false;

	return true;
}

std::int64_t LibraryManager::getBookCount(void)
{
	if(this->status < 1) return -1;

	return this->fobj_bookentry_get_entry_count(NULL);
}

bool LibraryManager::addNewUser(std::string name, std::int64_t id)
{
	userentry_t user;

	if(this->status < 1) return false;

	user.name = name;
	user.id = id;

	return this->fobj_userentry_add_entry(&user);
}

bool LibraryManager::removeUserIndex(std::int32_t index)
{
	if(this->status < 1) return false;

	return this->fobj_userentry_remove_entry(index);
}

bool LibraryManager::removeUserId(std::int64_t id)
{
	int n_ret;
	std::int32_t n_index;

	if(this->status < 1) return false;

	n_ret = this->fobj_userentry_user_id_exists(id, &n_index);
	if(n_ret < 0) return false;

	if(!n_ret)
	{
		this->err_msg = "removeUser: Error: given user ID is not on file.";
		return false;
	}

	return this->fobj_userentry_remove_entry(n_index);
}

bool LibraryManager::getUserInfoIndex(std::int32_t index, std::string *p_info, std::string *p_name, std::int64_t *p_id)
{
	userentry_t user;

	if(this->status < 1) return false;

	if(!this->fobj_userentry_get_entry(index, &user)) return false;

	if(p_info != NULL)
	{
		*p_info = "User Index: " + std::to_string(index) + '\n';
		*p_info += "User Name: " + user.name + '\n';
		*p_info += "User ID: " + std::to_string(user.id);
	}

	if(p_name != NULL) *p_name = user.name;
	if(p_id != NULL) *p_id = user.id;

	return true;
}

bool LibraryManager::getUserInfoId(std::int64_t id, std::string *p_info, std::string *p_name, std::int32_t *p_index)
{
	std::int32_t n_index;
	int n_ret;
	userentry_t user;

	if(this->status < 1) return false;

	n_ret = this->fobj_userentry_user_id_exists(id, &n_index);
	if(n_ret < 0) return false;

	if(!n_ret)
	{
		this->err_msg = "getUserInfo: Error: given user ID is not registered on file. Add new user to register user ID.";
		return false;
	}

	if(!this->getUserInfoIndex(n_index, p_info, p_name, NULL)) return false;

	if(p_index != NULL) *p_index = n_index;

	return true;
}

bool LibraryManager::listUsers(std::string *p_output)
{
	std::int32_t n_users = 0;
	std::int32_t n_index = 0;
	std::string user_info = "";

	if(this->status < 1) return false;

	if(p_output == NULL)
	{
		this->err_msg = "listUsers: Error: given string object pointer is null.";
		return false;
	}

	n_users = this->fobj_userentry_get_entry_count(NULL);
	if(n_users < 0) return false;

	if(n_users == 0)
	{
		*p_output = "There are no users registered on file.";
		return true;
	}

	n_index = 0;
	*p_output = "There are " + std::to_string(n_users) + " registered on file.";

	while(n_index < n_users)
	{
		if(!this->getUserInfoIndex(n_index, &user_info, NULL, NULL)) return false;

		*p_output += "\n\n";
		*p_output += user_info;

		n_index++;
	}

	return true;
}

bool LibraryManager::listUsersByName(std::string name, std::string *p_output)
{
	std::int32_t n_users = 0;
	std::int32_t n_index = 0;
	std::int32_t n_matches = 0;
	std::string user_name = "";
	std::string user_info = "";
	std::string user_list = "";

	if(this->status < 1) return false;

	if(p_output == NULL)
	{
		this->err_msg = "listUsersByName: Error: given string object pointer is null.";
		return false;
	}

	n_users = this->fobj_userentry_get_entry_count(NULL);
	if(n_users < 0) return false;

	if(name.length() < 1) name = "<UNDISCLOSED>";
	else name = str_toupper(name);

	n_index = 0;
	n_matches = 0;
	user_list = "";

	while(n_index < n_users)
	{
		if(!this->getUserInfoIndex(n_index, &user_info, &user_name, NULL)) return false;

		if(!user_name.compare(name))
		{
			n_matches++;
			user_list += "\n\n";
			user_list += user_info;
		}

		n_index++;
	}

	if(n_matches == 0)
	{
		*p_output = "No results match the given name.";
		return true;
	}

	*p_output = std::to_string(n_matches) + " results match the given name:";
	*p_output += user_list;

	return true;
}

std::int32_t LibraryManager::getUserCount(void)
{
	if(this->status < 1) return -1;

	return this->fobj_userentry_get_entry_count(NULL);
}

int LibraryManager::getStatus(void)
{
	return this->status;
}

std::string LibraryManager::getLastErrorMessage(void)
{
	if(this->status == this->STATUS_UNINITIALIZED) return "Error: Library Manager object has not been properly initialized.";

	return this->err_msg;
}

bool LibraryManager::fileobj_exists(const fileobj_t *p_fobj)
{
	if(p_fobj == NULL) return false;
	if(p_fobj->dir == NULL) return false;

	return (bool) (!access(p_fobj->dir, F_OK));
}

bool LibraryManager::fileobj_create(fileobj_t *p_fobj)
{
	if(p_fobj == NULL) return false;
	if(p_fobj->dir == NULL) return false;
	if(p_fobj->fd >= 0) this->fileobj_close(p_fobj);

	p_fobj->fd = open(p_fobj->dir, (O_WRONLY | O_CREAT | O_TRUNC), (S_IRUSR | S_IWUSR));
	if(p_fobj->fd < 0) return false;

	p_fobj->size = 0;
	return true;
}

bool LibraryManager::fileobj_open(fileobj_t *p_fobj)
{
	if(p_fobj == NULL) return false;
	if(p_fobj->dir == NULL) return false;
	if(p_fobj->fd >= 0) this->fileobj_close(p_fobj);

	p_fobj->fd = open(p_fobj->dir, O_RDWR);
	if(p_fobj->fd < 0) return false;

	p_fobj->size = __LSEEK(p_fobj->fd, 0, SEEK_END);
	return true;
}

void LibraryManager::fileobj_close(fileobj_t *p_fobj)
{
	if(p_fobj == NULL) return;
	if(p_fobj->fd < 0) return;

	close(p_fobj->fd);
	p_fobj->fd = -1;
	p_fobj->size = 0;
	return;
}

bool LibraryManager::fileobj_save(fileobj_t *p_fobj)
{
	if(p_fobj == NULL) return false;
	if(p_fobj->dir == NULL) return false;
	if(p_fobj->fd < 0) return false;

	this->fileobj_close(p_fobj);

	return this->fileobj_open(p_fobj);
}

/*
 * Check the book entries on file. Attempt to correct any minor errors.
 * returns true if successful, false otherwise.
 */

bool LibraryManager::fobj_bookentry_verify(void)
{
	const size_t BUFFER_SIZE = 4096u;
	size_t buffer_index = 0u;
	char *p_buffer = NULL;

	std::uint64_t entry_index;
	std::uint32_t entry_size;
	std::int32_t entry_status;
	std::int64_t entry_renter;

	std::uint64_t current_index = 0u;
	std::uint32_t current_size = 0u;

	size_t entry_title_len = 0u;
	size_t entry_author_len = 0u;
	bool fix_entry = false;

	p_buffer = (char*) std::malloc(BUFFER_SIZE);
	if(p_buffer == NULL)
	{
		this->status = this->STATUS_ERROR_MEMALLOC;
		this->err_msg = "fobj_bookentry_verify: Error: memory allocate failed.";
		return false;
	}

	this->fobj_bookentry.pos = 0;
	current_index = 0u;

	while(this->fobj_bookentry.pos < this->fobj_bookentry.size)
	{
		memset(p_buffer, 0, BUFFER_SIZE);

		__LSEEK(this->fobj_bookentry.fd, this->fobj_bookentry.pos, SEEK_SET);
		read(this->fobj_bookentry.fd, p_buffer, BUFFER_SIZE);

		entry_title_len = 0u;
		entry_author_len = 0u;

		buffer_index = 24u;

		while(buffer_index < BUFFER_SIZE)
		{
			if(p_buffer[buffer_index] == '\0') break;

			entry_title_len++;
			buffer_index++;
		}

		buffer_index++;

		while(buffer_index < BUFFER_SIZE)
		{
			if(p_buffer[buffer_index] == '\0') break;

			entry_author_len++;
			buffer_index++;
		}

		/*Error Check: title and author strings shouldn't be that long. File is likely broken.*/
		if(buffer_index >= BUFFER_SIZE)
		{
			this->status = this->STATUS_ERROR_BROKENFILE;
			this->err_msg = "fobj_bookentry_verify: Error: file is corrupted.";
			std::free(p_buffer);
			return false;
		}

		entry_status = *((std::int32_t*) &p_buffer[12]);
		entry_renter = *((std::int64_t*) &p_buffer[16]);

		/*Error Check: status claims this book is rented but renter id is invalid. File is likely broken.*/
		if((entry_status == this->BOOKENTRY_STATUS_RENTED) && (entry_renter < 0))
		{
			this->status = this->STATUS_ERROR_BROKENFILE;
			this->err_msg = "fobj_bookentry_verify: Error: file is corrupted.";
			std::free(p_buffer);
			return false;
		}

		current_size = this->BOOKENTRY_MINSIZE + ((std::uint32_t) (entry_title_len + entry_author_len));

		entry_size = *((std::uint32_t*) &p_buffer[8]);

		if(current_size != entry_size)
		{
			__LSEEK(this->fobj_bookentry.fd, (this->fobj_bookentry.pos + 8), SEEK_SET);
			write(this->fobj_bookentry.fd, &current_size, 4);

			fix_entry = true;
		}

		entry_index = *((std::uint64_t*) &p_buffer[0]);

		if(current_index != entry_index)
		{
			__LSEEK(this->fobj_bookentry.fd, this->fobj_bookentry.pos, SEEK_SET);
			write(this->fobj_bookentry.fd, &current_index, 8);

			fix_entry = true;
		}

		current_index++;
		this->fobj_bookentry.pos += (__offset) current_size;
	}

	std::free(p_buffer);

	if(fix_entry)
	{
		if(!this->fileobj_save(&this->fobj_bookentry))
		{
			this->status = this->STATUS_ERROR_FILEOP;
			this->err_msg = "fobj_bookentry_verify: Error: failed to save file changes.";
			return false;
		}
	}

	return true;
}

/*
 * Clean up the book entry file by removing void entries.
 * returns true if successful, false otherwise.
 */

bool LibraryManager::fobj_bookentry_cleanup(void)
{
	const size_t BUFFER_SIZE = 4096u;
	char *p_buffer = NULL;

	int out_fd = -1;
	__offset outfd_pos = 0;
	std::string outfd_dir = "";

	std::uint64_t entry_index = 0u;
	std::uint32_t entry_size = 0u;
	std::int32_t entry_status = 0;

	std::uint64_t input_entry_index = 0u;
	std::int64_t n_books = 0;

	int n_ret = 0;

	n_books = this->fobj_bookentry_get_entry_count(NULL);
	if(n_books < 0) return false;

	p_buffer = (char*) std::malloc(BUFFER_SIZE);
	if(p_buffer == NULL)
	{
		this->status = this->STATUS_ERROR_MEMALLOC;
		this->err_msg = "fobj_bookentry_cleanup: Error: memory allocate failed.";
		return false;
	}

	outfd_dir = ".tmp.";
	outfd_dir += this->fobj_bookentry.dir;

	out_fd = open(outfd_dir.c_str(), (O_WRONLY | O_CREAT | O_TRUNC), (S_IRUSR | S_IWUSR));
	if(out_fd < 0)
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_bookentry_cleanup: Error: failed to create output file.";
		std::free(p_buffer);
		return false;
	}

	this->fobj_bookentry.pos = 0;
	outfd_pos = 0;
	input_entry_index = 0u;
	entry_index = 0u;

	while(input_entry_index < ((std::uint64_t) n_books))
	{
		memset(p_buffer, 0, BUFFER_SIZE);

		__LSEEK(this->fobj_bookentry.fd, this->fobj_bookentry.pos, SEEK_SET);
		read(this->fobj_bookentry.fd, p_buffer, BUFFER_SIZE);

		entry_size = *((std::uint32_t*) &p_buffer[8]);
		entry_status = *((std::int32_t*) &p_buffer[12]);

		if(entry_status != this->BOOKENTRY_STATUS_VOID)
		{
			*((std::uint64_t*) &p_buffer[0]) = entry_index;

			__LSEEK(out_fd, outfd_pos, SEEK_SET);
			write(out_fd, p_buffer, entry_size);

			entry_index++;
			outfd_pos += (__offset) entry_size;
		}

		this->fobj_bookentry.pos += (__offset) entry_size;
		input_entry_index++;
	}

	std::free(p_buffer);

	close(out_fd);
	this->fileobj_close(&this->fobj_bookentry);

	n_ret = unlink(this->fobj_bookentry.dir);
	if(n_ret)
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_bookentry_cleanup: Error: failed to delete outdated book entry file.";
		return false;
	}

	n_ret = rename(outfd_dir.c_str(), this->fobj_bookentry.dir);
	if(n_ret < 0)
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_bookentry_cleanup: Error: failed to rename updated book entry file.";
		return false;
	}

	if(!this->fileobj_open(&this->fobj_bookentry))
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_bookentry_cleanup: Error: failed to open book entry file.";
		return false;
	}

	return this->fobj_bookentry_verify();
}

/*
 * Get the current book entry count on file.
 * returns the entry count value if successful, -1 otherwise (Although in the current implementation it doesn't ever return -1... anyway...)
 *
 * p_file_eod_index is an output. It receives the "end of data" position of the entry file.
 * It may be set to NULL if not used.
 */

std::int64_t LibraryManager::fobj_bookentry_get_entry_count(__offset *p_file_eod_index)
{
	std::uint64_t entry_count = 0u;
	std::uint32_t entry_size = 0u;

	this->fobj_bookentry.pos = 0;
	entry_count = 0u;

	while(this->fobj_bookentry.pos < this->fobj_bookentry.size)
	{
		entry_size = 0u;

		__LSEEK(this->fobj_bookentry.fd, (this->fobj_bookentry.pos + 8), SEEK_SET);
		read(this->fobj_bookentry.fd, &entry_size, 4);

		if(entry_size <= this->BOOKENTRY_MINSIZE) break;

		this->fobj_bookentry.pos += (__offset) entry_size;
		entry_count++;
	}

	if(p_file_eod_index != NULL) *p_file_eod_index = this->fobj_bookentry.pos;

	return (std::int64_t) entry_count;
}

/*
 * Add a book entry to the file.
 * returns true if successful, false otherwise.
 */

bool LibraryManager::fobj_bookentry_add_entry(const bookentry_t *p_book)
{
	std::int64_t n_books = 0;
	std::uint64_t entry_index = 0u;
	std::uint32_t entry_size = 0u;
	std::int32_t entry_status = 0;
	std::int64_t entry_renter = 0;

	__offset file_eod = 0;

	std::string title = "";
	std::string author = "";

	if(p_book == NULL)
	{
		this->err_msg = "fobj_bookentry_add_entry: Error: given bookentry object is null.";
		return false;
	}

	n_books = this->fobj_bookentry_get_entry_count(&file_eod);
	if(n_books < 0) return false;

	entry_index = (std::uint64_t) n_books;

	if(p_book->title.length() < 1) title = "UNTITLED";
	else title = p_book->title;

	if(p_book->author.length() < 1) author = "UNKNOWN";
	else author = p_book->author;

	title = str_toupper(title);
	author = str_toupper(author);

	entry_size = this->BOOKENTRY_MINSIZE + ((std::uint32_t) (title.length() + author.length()));

	entry_status = this->BOOKENTRY_STATUS_AVAILABLE;
	entry_renter = -1;

	/*
	 * I really don't know if when reading or writing to a file descriptor in fcntl.h, the file position is automatically incremented.
	 * Anyway, better safe than sorry. I'm manually setting the file position for each consecutive write operation.
	 */

	this->fobj_bookentry.pos = file_eod;

	__LSEEK(this->fobj_bookentry.fd, this->fobj_bookentry.pos, SEEK_SET);
	write(this->fobj_bookentry.fd, &entry_index, 8);
	this->fobj_bookentry.pos += 8;

	__LSEEK(this->fobj_bookentry.fd, this->fobj_bookentry.pos, SEEK_SET);
	write(this->fobj_bookentry.fd, &entry_size, 4);
	this->fobj_bookentry.pos += 4;

	__LSEEK(this->fobj_bookentry.fd, this->fobj_bookentry.pos, SEEK_SET);
	write(this->fobj_bookentry.fd, &entry_status, 4);
	this->fobj_bookentry.pos += 4;

	__LSEEK(this->fobj_bookentry.fd, this->fobj_bookentry.pos, SEEK_SET);
	write(this->fobj_bookentry.fd, &entry_renter, 8);
	this->fobj_bookentry.pos += 8;

	/*string.c_str() automatically adds a null terminator to the const char* text, that means it should be safe to use (string.length() + 1) to include the null terminator.*/

	__LSEEK(this->fobj_bookentry.fd, this->fobj_bookentry.pos, SEEK_SET);
	write(this->fobj_bookentry.fd, title.c_str(), (title.length() + 1));
	this->fobj_bookentry.pos += (__offset) (title.length() + 1);

	__LSEEK(this->fobj_bookentry.fd, this->fobj_bookentry.pos, SEEK_SET);
	write(this->fobj_bookentry.fd, author.c_str(), (author.length() + 1));

	if(!this->fileobj_save(&this->fobj_bookentry))
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_bookentry_add_entry: Error: failed to save changes to file.";
		return false;
	}

	return this->fobj_bookentry_verify();
}

/*
 * Remove the book entry with the given index.
 * returns true if successful, false otherwise.
 */

bool LibraryManager::fobj_bookentry_remove_entry(std::int64_t index)
{
	if(!this->fobj_bookentry_set_entry_status(index, this->BOOKENTRY_STATUS_VOID)) return false;

	return this->fobj_bookentry_cleanup();
}

/*
 * Retrieve the file position of the entry with the given index.
 * returns true if successful, false otherwise.
 *
 * p_filepos is an output. It receives the file position value for the entry.
 * if set to NULL, file position value will be stored in fobj_bookentry.pos .
 */

bool LibraryManager::fobj_bookentry_get_entry_filepos(std::int64_t index, __offset *p_filepos)
{
	std::int64_t n_books = 0;
	std::int64_t curr_index = 0;
	std::uint32_t entry_size = 0u;

	if(index < 0)
	{
		this->err_msg = "fobj_bookentry_get_entry_filepos: Error: given index is invalid.";
		return false;
	}

	n_books = this->fobj_bookentry_get_entry_count(NULL);
	if(n_books < 0) return false;

	if(index >= n_books)
	{
		this->err_msg = "fobj_bookentry_get_entry_filepos: Error: given index is out of bounds.";
		return false;
	}

	this->fobj_bookentry.pos = 0;
	curr_index = 0;

	while(curr_index < index)
	{
		__LSEEK(this->fobj_bookentry.fd, (this->fobj_bookentry.pos + 8), SEEK_SET);
		read(this->fobj_bookentry.fd, &entry_size, 4);

		this->fobj_bookentry.pos += (__offset) entry_size;
		curr_index++;
	}

	if((p_filepos != NULL) && (p_filepos != &this->fobj_bookentry.pos)) *p_filepos = this->fobj_bookentry.pos;

	return true;
}

/*
 * Set the status for the book entry with the given index
 * returns true if successful, false otherwise.
 */

bool LibraryManager::fobj_bookentry_set_entry_status(std::int64_t index, int status)
{
	std::int32_t entry_status;

	if((status < this->BOOKENTRY_STATUS_MIN) || (status > this->BOOKENTRY_STATUS_MAX))
	{
		this->err_msg = "fobj_bookentry_set_entry_status: Error: given status is invalid.";
		return false;
	}

	if(!this->fobj_bookentry_get_entry_filepos(index, NULL)) return false;

	entry_status = (std::int32_t) status;

	__LSEEK(this->fobj_bookentry.fd, (this->fobj_bookentry.pos + 12), SEEK_SET);
	write(this->fobj_bookentry.fd, &entry_status, 4);

	if(!this->fileobj_save(&this->fobj_bookentry))
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_bookentry_set_entry_status: Error: failed to save changes to file.";
		return false;
	}

	return true;
}

/*
 * Set the renter_id for the book entry with the given index.
 * returns true if successful, false otherwise.
 */

bool LibraryManager::fobj_bookentry_set_entry_renterid(std::int64_t index, std::int64_t renter_id)
{
	if(!this->fobj_bookentry_get_entry_filepos(index, NULL)) return false;

	__LSEEK(this->fobj_bookentry.fd, (this->fobj_bookentry.pos + 16), SEEK_SET);
	write(this->fobj_bookentry.fd, &renter_id, 8);

	if(!this->fileobj_save(&this->fobj_bookentry))
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_bookentry_set_entry_renterid: Error: failed to save changes to file.";
		return false;
	}

	return true;
}

/*
 * Retrieve the core data from a book entry
 * returns true if successful, false otherwise.
 */

bool LibraryManager::fobj_bookentry_get_entry(std::int64_t index, bookentry_t *p_book)
{
	const size_t BUFFER_SIZE = 4096u;
	char *p_buffer = NULL;

	size_t title_index = 0u;
	size_t author_index = 0u;

	if(p_book == NULL)
	{
		this->err_msg = "fobj_bookentry_get_entry: Error: given bookentry object pointer is null.";
		return false;
	}

	if(!this->fobj_bookentry_get_entry_filepos(index, NULL)) return false;

	p_buffer = (char*) std::malloc(BUFFER_SIZE);
	if(p_buffer == NULL)
	{
		this->status = this->STATUS_ERROR_MEMALLOC;
		this->err_msg = "fobj_bookentry_get_entry: Error: memory allocate failed.";
		return false;
	}

	__LSEEK(this->fobj_bookentry.fd, this->fobj_bookentry.pos, SEEK_SET);
	read(this->fobj_bookentry.fd, p_buffer, BUFFER_SIZE);

	p_book->status = *((std::int32_t*) &p_buffer[12]);
	p_book->renter_id = *((std::int64_t*) &p_buffer[16]);

	title_index = 24u;
	author_index = 25u + ((size_t) cstr_getlength(&p_buffer[title_index]));

	p_book->title = (const char*) &p_buffer[title_index];
	p_book->author = (const char*) &p_buffer[author_index];

	std::free(p_buffer);
	return true;
}

/*
 * Check the user entry file data.
 * Make sure everything is in usable state, attempt to correct minor errors.
 *
 * returns true if successful, false otherwise
 */

bool LibraryManager::fobj_userentry_verify(void)
{
	const size_t BUFFER_SIZE = 2048u;
	size_t buffer_index = 0u;
	char *p_buffer = NULL;

	std::uint32_t entry_index = 0u;
	std::uint32_t entry_size = 0u;

	std::uint32_t current_index = 0u;
	std::uint32_t current_size = 0u;

	size_t entry_name_len = 0u;

	bool fix_entry = false;

	p_buffer = (char*) std::malloc(BUFFER_SIZE);
	if(p_buffer == NULL)
	{
		this->status = this->STATUS_ERROR_MEMALLOC;
		this->err_msg = "fobj_userentry_verify: Error: memory allocate failed.";
		return false;
	}

	this->fobj_userentry.pos = 0;
	current_index = 0u;

	while(this->fobj_userentry.pos < this->fobj_userentry.size)
	{
		memset(p_buffer, 0, BUFFER_SIZE);

		__LSEEK(this->fobj_userentry.fd, this->fobj_userentry.pos, SEEK_SET);
		read(this->fobj_userentry.fd, p_buffer, BUFFER_SIZE);

		entry_name_len = 0u;

		buffer_index = 16u;
		while(buffer_index < BUFFER_SIZE)
		{
			if(p_buffer[buffer_index] == '\0') break;

			entry_name_len++;
			buffer_index++;
		}

		if(buffer_index >= BUFFER_SIZE)
		{
			this->status = this->STATUS_ERROR_BROKENFILE;
			this->err_msg = "fobj_userentry_verify: Error: file is corrupted.";
			std::free(p_buffer);
			return false;
		}

		current_size = this->USERENTRY_MINSIZE + ((std::uint32_t) entry_name_len);

		entry_size = *((std::uint32_t*) &p_buffer[4]);

		if(entry_size != current_size)
		{
			__LSEEK(this->fobj_userentry.fd, (this->fobj_userentry.pos + 4), SEEK_SET);
			write(this->fobj_userentry.fd, &current_size, 4);

			fix_entry = true;
		}

		entry_index = *((std::uint32_t*) &p_buffer[0]);

		if(entry_index != current_index)
		{
			__LSEEK(this->fobj_userentry.fd, this->fobj_userentry.pos, SEEK_SET);
			write(this->fobj_userentry.fd, &current_index, 4);

			fix_entry = true;
		}

		this->fobj_userentry.pos += (__offset) current_size;
		current_index++;
	}

	std::free(p_buffer);

	if(fix_entry)
	{
		if(!this->fileobj_save(&this->fobj_userentry))
		{
			this->status = this->STATUS_ERROR_FILEOP;
			this->err_msg = "fobj_userentry_verify: Error: failed to save changes to file.";
			return false;
		}
	}

	return true;
}

/*
 * Clean up user entry file by removing any invalid entries in the file
 * returns true if successful, false otherwise.
 */

bool LibraryManager::fobj_userentry_cleanup(void)
{
	const size_t BUFFER_SIZE = 2048u;
	char *p_buffer = NULL;

	std::uint32_t entry_index = 0u;
	std::uint32_t entry_size = 0u;
	std::int64_t entry_id = 0;

	std::uint32_t input_entry_index = 0u;
	std::int32_t n_users = 0;

	int out_fd = -1;
	__offset outfd_pos = 0;

	std::string outfd_dir = "";

	int n_ret = 0;

	n_users = this->fobj_userentry_get_entry_count(NULL);
	if(n_users < 0) return false;

	p_buffer = (char*) std::malloc(BUFFER_SIZE);
	if(p_buffer == NULL)
	{
		this->status = this->STATUS_ERROR_MEMALLOC;
		this->err_msg = "fobj_userentry_cleanup: Error: memory allocate failed.";
		return false;
	}

	outfd_dir = ".tmp.";
	outfd_dir += this->fobj_userentry.dir;

	out_fd = open(outfd_dir.c_str(), (O_WRONLY | O_CREAT | O_TRUNC), (S_IRUSR | S_IWUSR));
	if(out_fd < 0)
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_userentry_cleanup: Error: failed to create output file.";
		std::free(p_buffer);
		return false;
	}

	this->fobj_userentry.pos = 0;
	outfd_pos = 0;
	input_entry_index = 0u;
	entry_index = 0u;

	while(input_entry_index < ((std::uint32_t) n_users))
	{
		memset(p_buffer, 0, BUFFER_SIZE);

		__LSEEK(this->fobj_userentry.fd, this->fobj_userentry.pos, SEEK_SET);
		read(this->fobj_userentry.fd, p_buffer, BUFFER_SIZE);

		entry_size = *((std::uint32_t*) &p_buffer[4]);
		entry_id = *((std::int64_t*) &p_buffer[8]);

		if(entry_id >= 0)
		{
			*((std::uint32_t*) &p_buffer[0]) = entry_index;

			__LSEEK(out_fd, outfd_pos, SEEK_SET);
			write(out_fd, p_buffer, entry_size);

			outfd_pos += (__offset) entry_size;
			entry_index++;
		}

		this->fobj_userentry.pos += (__offset) entry_size;
		input_entry_index++;
	}

	std::free(p_buffer);

	close(out_fd);
	this->fileobj_close(&this->fobj_userentry);

	n_ret = unlink(this->fobj_userentry.dir);
	if(n_ret)
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_userentry_cleanup: Error: failed to delete outdated user entry file.";
		return false;
	}

	n_ret = rename(outfd_dir.c_str(), this->fobj_userentry.dir);
	if(n_ret < 0)
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_userentry_cleanup: Error: failed to rename updated user entry file.";
		return false;
	}

	if(!this->fileobj_open(&this->fobj_userentry))
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_userentry_cleanup: Error: failed to open updated user entry file.";
		return false;
	}

	return this->fobj_userentry_verify();
}

/*
 * Get the current count of user entries on file.
 * return the count value if successful, -1 otherwise. (Although the current implementation of it won't ever return -1... anyway...)
 *
 * p_file_eod_index is an output. It receives the "end of data" position of the entry file.
 * It may be set to NULL if not used.
 */

std::int32_t LibraryManager::fobj_userentry_get_entry_count(__offset *p_file_eod_index)
{
	std::uint32_t entry_count = 0u;
	std::uint32_t entry_size = 0u;

	this->fobj_userentry.pos = 0;
	entry_count = 0u;

	while(this->fobj_userentry.pos < this->fobj_userentry.size)
	{
		entry_size = 0u;

		__LSEEK(this->fobj_userentry.fd, (this->fobj_userentry.pos + 4), SEEK_SET);
		read(this->fobj_userentry.fd, &entry_size, 4);

		if(entry_size <= this->USERENTRY_MINSIZE) break;

		this->fobj_userentry.pos += (__offset) entry_size;
		entry_count++;
	}

	if(p_file_eod_index != NULL) *p_file_eod_index = this->fobj_userentry.pos;

	return (std::int32_t) entry_count;
}

/*
 * Add new user entry to the file.
 * returns true if successful, false otherwise.
 */

bool LibraryManager::fobj_userentry_add_entry(const userentry_t *p_user)
{
	std::int32_t n_users = 0;
	std::uint32_t entry_index = 0u;
	std::uint32_t entry_size = 0u;
	std::string name = "";

	__offset file_eod = 0;
	int n_ret = 0;

	if(p_user == NULL)
	{
		this->err_msg = "fobj_userentry_add_entry: Error: given userentry object pointer is null.";
		return false;
	}

	if(p_user->id < 0)
	{
		this->err_msg = "fobj_userentry_add_entry: Error: given user id is invalid.";
		return false;
	}

	n_users = this->fobj_userentry_get_entry_count(&file_eod);
	if(n_users < 0) return false;

	n_ret = this->fobj_userentry_user_id_exists(p_user->id, NULL);
	if(n_ret < 0) return false;

	if(n_ret)
	{
		this->err_msg = "fobj_userentry_add_entry: Error: given user id already exists.";
		return false;
	}

	if(p_user->name.length() < 1) name = "<UNDISCLOSED>";
	else name = p_user->name;

	name = str_toupper(name);

	this->fobj_userentry.pos = file_eod;

	entry_index = (std::uint32_t) n_users;
	entry_size = this->USERENTRY_MINSIZE + ((std::uint32_t) name.length());

	__LSEEK(this->fobj_userentry.fd, this->fobj_userentry.pos, SEEK_SET);
	write(this->fobj_userentry.fd, &entry_index, 4);
	this->fobj_userentry.pos += 4;

	__LSEEK(this->fobj_userentry.fd, this->fobj_userentry.pos, SEEK_SET);
	write(this->fobj_userentry.fd, &entry_size, 4);
	this->fobj_userentry.pos += 4;

	__LSEEK(this->fobj_userentry.fd, this->fobj_userentry.pos, SEEK_SET);
	write(this->fobj_userentry.fd, &p_user->id, 8);
	this->fobj_userentry.pos += 8;

	__LSEEK(this->fobj_userentry.fd, this->fobj_userentry.pos, SEEK_SET);
	write(this->fobj_userentry.fd, name.c_str(), (name.length() + 1));

	if(!this->fileobj_save(&this->fobj_userentry))
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_userentry_add_entry: Error: failed to save changes to file.";
		return false;
	}

	return this->fobj_userentry_verify();
}

/*
 * Remove the entry with the corresponding index.
 * returns true if successful, false otherwise.
 */

bool LibraryManager::fobj_userentry_remove_entry(std::int32_t index)
{
	std::int64_t user_id = 0;

	if(!this->fobj_userentry_get_entry_filepos(index, NULL)) return false;

	user_id = -1;

	__LSEEK(this->fobj_userentry.fd, (this->fobj_userentry.pos + 8), SEEK_SET);
	write(this->fobj_userentry.fd, &user_id, 8);

	if(!this->fileobj_save(&this->fobj_userentry))
	{
		this->status = this->STATUS_ERROR_FILEOP;
		this->err_msg = "fobj_userentry_remove_entry: Error: failed to save changes to file.";
		return false;
	}

	return this->fobj_userentry_cleanup();
}

/*
 * Retrieve the starting file position for a given entry index.
 * returns true if successful, false otherwise.
 *
 * p_filepos is an output. It receives the file position for the given entry index.
 * If set to NULL, the value will be stored in fobj_userentry.pos .
 */

bool LibraryManager::fobj_userentry_get_entry_filepos(std::int32_t index, __offset *p_filepos)
{
	std::int32_t n_users;
	std::int32_t curr_index;
	std::uint32_t entry_size;

	if(index < 0)
	{
		this->err_msg = "fobj_userentry_get_entry_filepos: Error: given index is invalid.";
		return false;
	}

	n_users = this->fobj_userentry_get_entry_count(NULL);
	if(n_users < 0) return false;

	if(index >= n_users)
	{
		this->err_msg = "fobj_userentry_get_entry_filepos: Error: given index is out of bounds.";
		return false;
	}

	this->fobj_userentry.pos = 0;
	curr_index = 0;

	while(curr_index < index)
	{
		__LSEEK(this->fobj_userentry.fd, (this->fobj_userentry.pos + 4), SEEK_SET);
		read(this->fobj_userentry.fd, &entry_size, 4);

		this->fobj_userentry.pos += (__offset) entry_size;
		curr_index++;
	}

	if((p_filepos != NULL) && (p_filepos != &this->fobj_userentry.pos)) *p_filepos = this->fobj_userentry.pos;

	return true;
}

/*
 * Retrieves the core data from an user entry.
 * returns true if successful, false otherwise.
 */

bool LibraryManager::fobj_userentry_get_entry(std::int32_t index, userentry_t *p_user)
{
	const size_t BUFFER_SIZE = 2048u;
	char *p_buffer = NULL;

	if(p_user == NULL)
	{
		this->err_msg = "fobj_userentry_get_entry: Error: userentry object pointer is null.";
		return false;
	}

	if(!this->fobj_userentry_get_entry_filepos(index, NULL)) return false;

	p_buffer = (char*) std::malloc(BUFFER_SIZE);
	if(p_buffer == NULL)
	{
		this->status = this->STATUS_ERROR_MEMALLOC;
		this->err_msg = "fobj_userentry_get_entry: Error: memory allocate failed.";
		return false;
	}

	__LSEEK(this->fobj_userentry.fd, this->fobj_userentry.pos, SEEK_SET);
	read(this->fobj_userentry.fd, p_buffer, BUFFER_SIZE);

	p_user->id = *((std::int64_t*) &p_buffer[8]);
	p_user->name = (const char*) &p_buffer[16];

	std::free(p_buffer);

	return true;
}

/*
 * Check if given user ID already exists on file.
 *
 * return value:
 * 1 = true
 * 0 = false
 * -1 = error
 *
 * p_index is an output. It receives the index for the matching user id entry.
 * it can be set to NULL if not used.
 */

int LibraryManager::fobj_userentry_user_id_exists(std::int64_t user_id, std::int32_t *p_index)
{
	std::int32_t n_users = 0;
	std::int32_t curr_index = 0;
	userentry_t user;

	if(user_id < 0)
	{
		this->err_msg = "fobj_userentry_user_id_exists: Error: given user id is invalid.";
		return -1;
	}

	n_users = this->fobj_userentry_get_entry_count(NULL);
	if(n_users < 0) return -1;

	for(curr_index = 0; curr_index < n_users; curr_index++)
	{
		if(!this->fobj_userentry_get_entry(curr_index, &user)) return -1;

		if(user_id == user.id)
		{
			if(p_index != NULL) *p_index = curr_index;
			return 1;
		}
	}

	return 0;
}

