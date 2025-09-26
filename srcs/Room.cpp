/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Room.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoste <acoste@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 00:11:46 by acoste            #+#    #+#             */
/*   Updated: 2025/09/26 03:10:54 by acoste           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Room.hpp>

Room::Room()
{
	std::cout << "Salon created" << std::endl;
}

Room::Room(std::string name, ClientSocket *client)
{
	_name = name;
	_client.push_back(*client);
	_admin.push_back(*client);
}


Room::Room(Room const &cpy)
{
	*this = cpy;
	std::cout << "operator cpy called" << std::endl;
}

Room::~Room()
{
	std::cout << "Salon destructor called" << std::endl;
}

Room	const &Room::operator=(Room const &dupp)
{
	if (this != &dupp)
	{
		this->_topic = dupp._topic;
	}
	std::cout << "= operand called on a salon" << std::endl;
	return (*this);
}

std::string	Room::getName(void)
{
	return(_name);
}

std::string Room::getTopic(void)
{
	return(_topic);
}

void	Room::settopic(std::string topic)
{
	_topic = topic;
}

std::vector<ClientSocket> Room::getClient()
{
	return(_client);
}

// void	Room::addcli(ClientSocket client)
// {

// }


/* void	Room::manipclient(ClientSocket client, std::string param)
{
	if (param == "add")
	{
	}
	if (param == "leave")
	{

	}
	if (param == "operator")
	{

	}
} */

