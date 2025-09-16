#include "RequestClass.hpp"

Request::Request()
{
	std::memset(_buffer, 0, 4096);
}

Request::~Request()
{
}

//TODO verif le continue du buffer et si il est plein.

void	Request:: receive(int fd)
{
	int		check;
	size_t	next = 0;

	//pas de \r dans la request ??? avec nc* //ended with \r\n normaly
	check = recv(fd, &_buffer, 4096, 0);
	if (check == 0)
	{
		std::cout << "Client disconnected" << std::endl;
		this->clear(); // set all args to 0, empty
		return ;
	}
	if (check < 0)
	{
		std::cout << "Recv failed" << std::endl;
		this->clear(); // set all args to 0
		return ;
	}

	_str = _buffer;

	next = ft_find(_buffer, ' ');
	//std::cout << next << std::endl;

	_cmd = _str.substr(0, next);
	_args = _str.substr(next + 1, _str.size() - next - 1);

	//std::cout << "Buffer : " << _buffer << std::endl;
	//std::cout << "next = " << next << std::endl;
	//std::cout << "Command : " << _cmd << std::endl;
	//std::cout << "args = " << _args << std::endl;

	// next = ft_find(_buffer, (char)13);
	// std::cout << "? : " << next << std::endl;

	// next = ft_find(_buffer, (char)10);
	// std::cout << "? : " << next << std::endl;

}

void Request::show()
{
	std::cout << "----REQUEST----\n"
	<< "CMD : " << _cmd
	<< "\nARGS : " << _args
	<< std::endl;
}


// return 0 if c is not found, i when c is found

int	Request::ft_find(char *str, char c)
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
	return	(_cmd);
}

std::string Request::getArgs()
{
	return	(_args.substr(0, _args.size() - 1)); // 2?
}

void	Request::clear()
{
	_args.clear();
	_cmd.clear();
	_str.clear();
	std::memset(_buffer, 0, 4096);
}

