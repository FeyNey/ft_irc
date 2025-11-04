/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocketClass.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoste <acoste@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:50:50 by acoste            #+#    #+#             */
/*   Updated: 2025/11/04 15:52:19 by acoste           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ListenSocketClass.hpp>

ListenSocket::ListenSocket()
{
}

ListenSocket::~ListenSocket()
{
}

int ListenSocket::connect()
{
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(_port);
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(_fd, F_SETFL, O_NONBLOCK);
	int opt = 1;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (_fd == -1)
		throw(std::runtime_error("ERROR : Socket creation failed"));
	if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
		throw std::runtime_error("ERROR : Bind failed");
	if (listen(_fd, SOMAXCONN) == -1)
		throw std::runtime_error("ERROR : Listen failed");
	_connected = true;
	return (_fd);
}

void ListenSocket::setport(int port)
{
	_port = port;
	if (port == 0)
	{
		_port = 6667;
	}
	std::cout << "Port that would be connected is number " << _port << std::endl;
}