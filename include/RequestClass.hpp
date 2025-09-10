#ifndef REQUEST_CLASS_HPP
#define REQUEST_CLASS_HPP

#include <map>
#include <string>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>

class Request {

	public :
		Request();
		~Request();

		void show();
		void receive(int fd);


	private :
		char	_buffer[4096];
		std::string	_str;
		std::string	_cmd;
		std::string	_args;


};

#endif
