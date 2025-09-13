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

		void	show();
		void	receive(int fd);
		int		ft_find(char *str, char c);
		void	disconnected();
		std::string getCmd();
		std::string getArgs();
		void		clear();



	private :
		char	_buffer[4096];
		std::string	_str;
		std::string	_cmd;
		std::string	_args;


};

#endif
