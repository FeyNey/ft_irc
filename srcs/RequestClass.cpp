/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestClass.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoste <acoste@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:50:35 by acoste            #+#    #+#             */
/*   Updated: 2025/10/26 20:44:29 by acoste           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestClass.hpp"
#include "ClientSocketClass.hpp"

Request::Request() : _msg_end(0)
{
	std::memset(_buffer, 0, 4096);
}

Request::~Request()
{
}

bool	Request::getmsg_end()
{
	return (_msg_end);
}

std::vector<std::string> Request::split(std::string str)
{
	//static char	buff How to do a buff in cppp

	int							pos = 0;
	std::vector<std::string>	requests;
	// std::string					buffer;
	// int							res = -1;

	// _msg_end = 0;
	// buffer += str;
	// res = buffer.find("\r\n");
	// if (res == 0)
	// 	return (requests);

	for(size_t i = 1; i < str.size(); i++)
	{
		if (str[i] == '\n' && str[i - 1] == '\r')
		{
			requests.push_back(str.substr(pos, i - pos - 1));
			pos = i + 1;
			// _msg_end = 1;
		}
	}
	if (requests.size() == 0)
		requests.push_back(" ");
	return (requests);
}

void	Request:: receive(int fd, ClientSocket *client)
{
	int		check;
	size_t	next = 0;

	check = recv(fd, &_buffer, 4096, 0);

	std::cout << "buffer: " << _buffer << std::endl;
	std::cout << "Check " << check << std::endl;

	if (check == 0)
	{
		std::cout << "Client disconnected" << std::endl;
		this->clear(); // set all args to 0, empty
		client->quitting();
		return ;
	}
	if (check < 0)
	{
		std::cout << "Recv failed" << std::endl;
		this->clear(); // set all args to 0
		return ;
	}
	_requests = split(_buffer);
	for (size_t i = 0; i < _requests.size(); i++)
	{
		next = ft_find(_requests[i], ' ');
		std::cout << "2" << std::endl;
		if (next == 0)
		{
			_cmds.push_back(_requests[i].substr(0, _requests[i].length()));
			_args.push_back(" ");
			continue;
		}
		else
		{
			_cmds.push_back(_requests[i].substr(0, next));
			_args.push_back(_requests[i].substr(next + 1, _requests[i].size() - next - 1));
		}
	}
	std::memset(_buffer, 0, 4096);
}

void Request::show()
{
	for (size_t i = 0; i < _requests.size(); i++)
	{
		std::cout << "----REQUEST----\n"
		<< "CMD : " << _cmds[i]
		<< "\nARGS : " << _args[i]
		<< std::endl;
	}
}

size_t Request::size()
{
	return(_requests.size());
}


// return 0 if c is not found, i when c is found

int	Request::ft_find(std::string str, char c)
{
	for(int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == c)
			return (i);
	}
	return (0);
}

std::string Request::getCmd()
{
	std::string tmp = _cmds[0];
	_cmds.erase(_cmds.begin());
	return	(tmp);
}

std::string Request::getArgs()
{
	std::string tmp = _args[0];
	_args.erase(_args.begin());
	return	(tmp);
}

void	Request::clear()
{
	_args.clear();
	_cmds.clear();
	_requests.clear();
	std::memset(_buffer, 0, 4096);
}