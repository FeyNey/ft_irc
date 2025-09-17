#include "RequestClass.hpp"

Request::Request()
{
	std::memset(_buffer, 0, 4096);
}

Request::~Request()
{
}

std::vector<std::string> Request::split(std::string str)
{
	int							pos = 0;
	std::vector<std::string>	requests;

	for(size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '\n' && str[i - 1] == '\r')
		{
			requests.push_back(str.substr(pos, i - pos - 1));
			pos = i + 1;
		}
	}
	return (requests);
}

//TODO verif le continue du buffer et si il est plein.

void	Request:: receive(int fd)
{
	int		check;
	size_t	next = 0;

	//pas de \r dans la request ??? avec nc* //ended with \r\n normaly
	check = recv(fd, &_buffer, 4096, 0); // TODO Verif
	if (check == 0)
	{
		std::cout << "Client disconected" << std::endl;
		this->clear(); // set all args to 0, empty
		return ;
	}
	if (check < 0)
	{
		std::cout << "Recv failed" << std::endl;
		this->clear(); // set all args to 0
		return ;
	}
	_requests = split(_buffer);

	// std::cout << "request[0] : " << _requests[0] << std::endl;

	for (size_t i = 0; i < _requests.size(); i++)
	{
		_cmds.push_back(_requests[i].substr(0, next));
		_args.push_back(_requests[i].substr(next + 1, _requests[i].size() - next - 1));
	}
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