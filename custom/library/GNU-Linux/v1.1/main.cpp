/*
 * C++ Programming 2 Assignment 2:
 * Library Management Software Project
 * Version 1.1 (GNU-Linux)
 *
 * Author: Rafael Margarido Sabe
 * CCC ID: 340378
 * Email: 340378@colum.edu
 */

#include "globldef.h"
#include "cstrdef.h"
#include "strdef.hpp"

#include "LibraryManager.hpp"

#include <iostream>

std::string str = "";
std::string usrinput = "";
LibraryManager *p_libmgr = NULL;

extern bool app_init(void);
extern void app_deinit(void);

extern void app_exit(int exit_code, const char *exit_msg) __attribute__((__noreturn__));

extern bool runtime_loop(void);

extern void prompt_booklistall(void);
extern void prompt_booklisttitle(void);
extern void prompt_booklistauthor(void);
extern void prompt_bookliststatus(void);
extern void prompt_booklistrenter(void);
extern void prompt_bookinfo(void);
extern void prompt_bookaddnew(void);
extern void prompt_bookremove(void);
extern void prompt_bookrent(void);
extern void prompt_bookreturn(void);
extern void prompt_bookcount(void);
extern void prompt_userlistall(void);
extern void prompt_userlistname(void);
extern void prompt_userinfo(void);
extern void prompt_useraddnew(void);
extern void prompt_userremove(void);
extern void prompt_usercount(void);

extern void on_error(void);
extern void wait_keyenter(void);

int main(void)
{
	if(!app_init())
	{
		std::cout << "Error: app init failed\n";
		app_deinit();
		return 1;
	}

	while(runtime_loop());

	app_deinit();

	return 0;
}

bool app_init(void)
{
	p_libmgr = new LibraryManager();
	if(p_libmgr == NULL) return false;

	return (p_libmgr->getStatus() >= 1);
}

void app_deinit(void)
{
	if(p_libmgr != NULL)
	{
		delete p_libmgr;
		p_libmgr = NULL;
	}

	return;
}

void __attribute__((__noreturn__)) app_exit(int exit_code, const char *exit_msg)
{
	if(exit_msg != NULL) std::cout << "\n\nPROCESS EXIT CALLED\n" << exit_msg << std::endl;

	app_deinit();
	exit(exit_code);

	while(true) asm("\n\tnop\n\tnop\n\tnop\n\tnop"); /*It could just be "while(true);" , but I don't like doing empty loop. It feels like things could go very wrong... So I just threw in some void instructions (NOP).*/
}

bool runtime_loop(void)
{
	std::cout << "\n\nLibrary Management (v1.1 GNU-Linux)\n\n";

	std::cout << "Enter command:\n\n";

	std::cout << "Enter \"booklistall\" to view all books on file\n";
	std::cout << "Enter \"booklisttitle\" to search books by title on file\n";
	std::cout << "Enter \"booklistauthor\" to search books by author on file\n";
	std::cout << "Enter \"bookliststatus\" to search books by current status on file\n";
	std::cout << "Enter \"booklistrenter\" to search books by current renter on file\n";

	std::cout << "Enter \"bookinfo\" to look up a specific book info on file\n";

	std::cout << "Enter \"bookaddnew\" to add a new book to file\n";
	std::cout << "Enter \"bookremove\" to remove a book from file\n";

	std::cout << "Enter \"bookrent\" to rent a book on file\n";
	std::cout << "Enter \"bookreturn\" to return a book on file\n";

	std::cout << "Enter \"bookcount\" to view the number of books currently on file\n";

	std::cout << "Enter \"userlistall\" to view all users registered on file\n";
	std::cout << "Enter \"userlistname\" to search users by name on file\n";

	std::cout << "Enter \"userinfo\" to look up a specific user info on file\n";

	std::cout << "Enter \"useraddnew\" to register new user on file\n";
	std::cout << "Enter \"userremove\" to remove a user from file\n";

	std::cout << "Enter \"usercount\" to view the number of users currently registered on file\n";

	std::cout << "\n\nEnter \"quit\" to quit application\n\n";

	usrinput = "";
	std::cin >> usrinput;

	usrinput = str_tolower(usrinput);

	if(!usrinput.compare("quit")) return false;
	else if(!usrinput.compare("booklistall")) prompt_booklistall();
	else if(!usrinput.compare("booklisttitle")) prompt_booklisttitle();
	else if(!usrinput.compare("booklistauthor")) prompt_booklistauthor();
	else if(!usrinput.compare("bookliststatus")) prompt_bookliststatus();
	else if(!usrinput.compare("booklistrenter")) prompt_booklistrenter();
	else if(!usrinput.compare("bookinfo")) prompt_bookinfo();
	else if(!usrinput.compare("bookaddnew")) prompt_bookaddnew();
	else if(!usrinput.compare("bookremove")) prompt_bookremove();
	else if(!usrinput.compare("bookrent")) prompt_bookrent();
	else if(!usrinput.compare("bookreturn")) prompt_bookreturn();
	else if(!usrinput.compare("bookcount")) prompt_bookcount();
	else if(!usrinput.compare("userlistall")) prompt_userlistall();
	else if(!usrinput.compare("userlistname")) prompt_userlistname();
	else if(!usrinput.compare("userinfo")) prompt_userinfo();
	else if(!usrinput.compare("useraddnew")) prompt_useraddnew();
	else if(!usrinput.compare("userremove")) prompt_userremove();
	else if(!usrinput.compare("usercount")) prompt_usercount();
	else
	{
		std::cout << "Error: invalid command entered\n";
		wait_keyenter();
	}

	return true;
}

void prompt_booklistall(void)
{
	if(!p_libmgr->listBooks(&str))
	{
		on_error();
		return;
	}

	std::cout << "\n\nBook List:\n\n";
	std::cout << str;
	std::cout << "\n\n";

	wait_keyenter();
	return;
}

void prompt_booklisttitle(void)
{
	std::cout << "\n\nEnter search title (do not include any whitespaces): ";

	usrinput = "";
	std::cin >> usrinput;

	if(!p_libmgr->listBooksByTitle(usrinput, &str))
	{
		on_error();
		return;
	}

	std::cout << "\n\n";
	std::cout << str;
	std::cout << "\n\n";

	wait_keyenter();
	return;
}

void prompt_booklistauthor(void)
{
	std::cout << "\n\nEnter search author (do not include any whitespaces): ";

	usrinput = "";
	std::cin >> usrinput;

	if(!p_libmgr->listBooksByAuthor(usrinput, &str))
	{
		on_error();
		return;
	}

	std::cout << "\n\n";
	std::cout << str;
	std::cout << "\n\n";

	wait_keyenter();
	return;
}

void prompt_bookliststatus(void)
{
	while(true)
	{
		std::cout << "\n\nEnter search status option:\n\n";
		std::cout << "Enter \"available\" to list available books\n";
		std::cout << "Enter \"rented\" to list rented books\n";
		std::cout << "Enter \"debug\" to list void entries\n";
		std::cout << "Enter \"return\" to return to main menu\n";

		usrinput = "";
		std::cin >> usrinput;

		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;
		else if(!usrinput.compare("available"))
		{
			if(!p_libmgr->listBooksByStatus(p_libmgr->BOOKENTRY_STATUS_AVAILABLE, &str))
			{
				on_error();
				return;
			}

			break;
		}
		else if(!usrinput.compare("rented"))
		{
			if(!p_libmgr->listBooksByStatus(p_libmgr->BOOKENTRY_STATUS_RENTED, &str))
			{
				on_error();
				return;
			}

			break;
		}
		else if(!usrinput.compare("debug"))
		{
			if(!p_libmgr->listBooksByStatus(p_libmgr->BOOKENTRY_STATUS_VOID, &str))
			{
				on_error();
				return;
			}

			break;
		}
		else
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
		}
	}

	std::cout << "\n\n";
	std::cout << str;
	std::cout << "\n\n";

	wait_keyenter();
	return;
}

void prompt_booklistrenter(void)
{
	long long renter_id = 0ll;

	while(true)
	{
		std::cout << "\n\nEnter Renter User ID number\nEnter \"return\" to return to main menu\n\n";

		usrinput = "";
		std::cin >> usrinput;
		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;

		try
		{
			renter_id = std::stoll(usrinput);
		}
		catch(...)
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
			continue;
		}

		break;
	}

	if(!p_libmgr->listBooksByRenter((std::int64_t) renter_id, &str))
	{
		on_error();
		return;
	}

	std::cout << "\n\n";
	std::cout << str;
	std::cout << "\n\n";

	wait_keyenter();
	return;
}

void prompt_bookinfo(void)
{
	long long book_index = 0ll;

	while(true)
	{
		std::cout << "\n\nEnter book index number\nEnter \"return\" to return to main menu\n\n";

		usrinput = "";
		std::cin >> usrinput;
		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;

		try
		{
			book_index = std::stoll(usrinput);
		}
		catch(...)
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
			continue;
		}

		break;
	}

	if(!p_libmgr->getBookInfo((std::int64_t) book_index, &str, NULL, NULL, NULL, NULL))
	{
		on_error();
		return;
	}

	std::cout << "\n\n";
	std::cout << str;
	std::cout << "\n\n";

	wait_keyenter();
	return;
}

void prompt_bookaddnew(void)
{
	std::string title = "";
	std::string author = "";

	std::cout << "\n\nEnter new book title (do not include any whitespaces): ";
	std::cin >> title;

	std::cout << "\nEnter new book author (do not include any whitespaces): ";
	std::cin >> author;

	if(!p_libmgr->addNewBook(title, author))
	{
		on_error();
		return;
	}

	std::cout << "\n\nNew book added to file\n\n";

	wait_keyenter();
	return;
}

void prompt_bookremove(void)
{
	long long book_index = 0ll;

	while(true)
	{
		std::cout << "\n\nEnter book index number to be removed\nEnter \"return\" to return to main menu\n\n";

		usrinput = "";
		std::cin >> usrinput;
		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;

		try
		{
			book_index = std::stoll(usrinput);
		}
		catch(...)
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
			continue;
		}

		break;
	}

	if(!p_libmgr->removeBook((std::int64_t) book_index))
	{
		on_error();
		return;
	}

	std::cout << "\n\nBook removed from file\n\n";

	wait_keyenter();
	return;
}

void prompt_bookrent(void)
{
	long long book_index = 0ll;
	long long renter_id = 0ll;

	while(true)
	{
		std::cout << "\n\nEnter book index number to be rented\nEnter \"return\" to return to main menu\n\n";

		usrinput = "";
		std::cin >> usrinput;
		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;

		try
		{
			book_index = std::stoll(usrinput);
		}
		catch(...)
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
			continue;
		}

		std::cout << "\n\nEnter book renter ID number\nEnter \"return\" to return to main menu\n\n";

		usrinput = "";
		std::cin >> usrinput;
		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;

		try
		{
			renter_id = std::stoll(usrinput);
		}
		catch(...)
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
			continue;
		}

		break;
	}

	if(!p_libmgr->rentBook((std::int64_t) book_index, (std::int64_t) renter_id))
	{
		on_error();
		return;
	}

	std::cout << "\n\nRental successful\n\n";

	wait_keyenter();
	return;
}

void prompt_bookreturn(void)
{
	long long book_index = 0ll;
	long long renter_id = 0ll;

	while(true)
	{
		std::cout << "\n\nEnter book index number\nEnter \"return\" to return to main menu\n\n";

		usrinput = "";
		std::cin >> usrinput;
		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;

		try
		{
			book_index = std::stoll(usrinput);
		}
		catch(...)
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
			continue;
		}

		std::cout << "\n\nEnter book renter ID number\nEnter \"return\" to return to main menu\n\n";

		usrinput = "";
		std::cin >> usrinput;
		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;

		try
		{
			renter_id = std::stoll(usrinput);
		}
		catch(...)
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
			continue;
		}

		break;
	}

	if(!p_libmgr->returnBook((std::int64_t) book_index, (std::int64_t) renter_id))
	{
		on_error();
		return;
	}

	std::cout << "\n\nBook returned successfully\n\n";

	wait_keyenter();
	return;
}

void prompt_bookcount(void)
{
	std::int64_t n_books;

	n_books = p_libmgr->getBookCount();
	if(n_books < 0)
	{
		on_error();
		return;
	}

	std::cout << "\n\nThere are " << std::to_string(n_books) << " books currently on file\n\n";

	wait_keyenter();
	return;
}

void prompt_userlistall(void)
{
	if(!p_libmgr->listUsers(&str))
	{
		on_error();
		return;
	}

	std::cout << "\n\nUser List:\n\n";
	std::cout << str;
	std::cout << "\n\n";

	wait_keyenter();
	return;
}

void prompt_userlistname(void)
{
	std::cout << "\n\nEnter search user name (do not include any white spaces): ";

	usrinput = "";
	std::cin >> usrinput;

	if(!p_libmgr->listUsersByName(usrinput, &str))
	{
		on_error();
		return;
	}

	std::cout << "\n\n";
	std::cout << str;
	std::cout << "\n\n";

	wait_keyenter();
	return;
}

void prompt_userinfo(void)
{
	long long numll = 0ll;
	bool search_id = false;

	while(true)
	{
		std::cout << "\n\nEnter \"index\" to search user by index number\nEnter \"id\" to search user by ID number\nEnter \"return\" to return to main menu\n\n";

		usrinput = "";
		std::cin >> usrinput;
		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;
		else if(!usrinput.compare("index")) search_id = false;
		else if(!usrinput.compare("id")) search_id = true;
		else
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
			continue;
		}

		break;
	}

	while(true)
	{
		if(search_id) std::cout << "\n\nEnter user ID number\n";
		else std::cout << "\n\nEnter user index number\n";

		std::cout << "Enter \"return\" to return to main menu\n\n";

		usrinput = "";
		std::cin >> usrinput;
		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;

		try
		{
			numll = std::stoll(usrinput);
		}
		catch(...)
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
			continue;
		}

		break;
	}

	if(search_id)
	{
		if(!p_libmgr->getUserInfoId((std::int64_t) numll, &str, NULL, NULL))
		{
			on_error();
			return;
		}
	}
	else
	{
		if(!p_libmgr->getUserInfoIndex((std::int32_t) numll, &str, NULL, NULL))
		{
			on_error();
			return;
		}
	}

	std::cout << "\n\n";
	std::cout << str;
	std::cout << "\n\n";

	wait_keyenter();
	return;
}

void prompt_useraddnew(void)
{
	std::string user_name = "";
	long long user_id = 0ll;

	while(true)
	{
		std::cout << "\n\nEnter user name (do not include any white spaces): ";
		std::cin >> user_name;

		std::cout << "\n\nEnter user ID number\nEnter \"return\" to return to main menu\n\n";

		usrinput = "";
		std::cin >> usrinput;
		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;

		try
		{
			user_id = std::stoll(usrinput);
		}
		catch(...)
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
			continue;
		}

		break;
	}

	if(!p_libmgr->addNewUser(user_name, (std::int64_t) user_id))
	{
		on_error();
		return;
	}

	std::cout << "\n\nNew user registered successfully\n\n";

	wait_keyenter();
	return;
}

void prompt_userremove(void)
{
	long long numll = 0ll;
	bool search_id = false;

	while(true)
	{
		std::cout << "\n\nEnter \"index\" to remove user by index number\nEnter \"id\" to remove user by ID number\nEnter \"return\" to return to main menu\n\n";

		usrinput = "";
		std::cin >> usrinput;
		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;
		else if(!usrinput.compare("index")) search_id = false;
		else if(!usrinput.compare("id")) search_id = true;
		else
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
			continue;
		}

		break;
	}

	while(true)
	{
		if(search_id) std::cout << "\n\nEnter user ID number\n";
		else std::cout << "\n\nEnter user index number\n";

		std::cout << "Enter \"return\" to return to main menu\n\n";

		usrinput = "";
		std::cin >> usrinput;
		usrinput = str_tolower(usrinput);

		if(!usrinput.compare("return")) return;

		try
		{
			numll = std::stoll(usrinput);
		}
		catch(...)
		{
			std::cout << "Error: invalid command entered\n";
			wait_keyenter();
			continue;
		}

		break;
	}

	if(search_id)
	{
		if(!p_libmgr->removeUserId((std::int64_t) numll))
		{
			on_error();
			return;
		}
	}
	else
	{
		if(!p_libmgr->removeUserIndex((std::int32_t) numll))
		{
			on_error();
			return;
		}
	}

	std::cout << "\n\nUser successfully removed\n\n";

	wait_keyenter();
	return;
}

void prompt_usercount(void)
{
	std::int32_t n_users;

	n_users = p_libmgr->getUserCount();
	if(n_users < 0)
	{
		on_error();
		return;
	}

	std::cout << "\n\nThere are " + std::to_string(n_users) + " users currently registered on file\n\n";

	wait_keyenter();
	return;
}

void on_error(void)
{
	if(p_libmgr->getStatus() < 1)
	{
		str = "Critical Internal Error\n" + p_libmgr->getLastErrorMessage();
		app_exit(1, str.c_str());
	}

	std::cout << "\n\nError Occurred\nError Message: " << p_libmgr->getLastErrorMessage() << "\n\n";
	wait_keyenter();
	return;
}

void wait_keyenter(void)
{
	std::cout << "Enter any character to continue...\n\n";

	usrinput = "";
	std::cin >> usrinput;
	return;
}

