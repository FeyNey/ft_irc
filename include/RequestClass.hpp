#ifndef REQUEST_CLASS_HPP
#define REQUEST_CLASS_HPP

#include <map>
#include <string>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>
#include <vector>

class Request {

	public :
		Request();
		~Request();

		void	show();
		size_t	size();
		void	receive(int fd);
		int		ft_find(std::string str, char c);
		void	disconnected();
		std::string getCmd();
		std::string getArgs();
		void		clear();
		std::vector<std::string> split(std::string str);



	private :
		char	_buffer[4096];
		std::vector<std::string>	_requests;
		std::vector<std::string>	_cmds;
		std::vector<std::string>	_args;


};

#endif
