/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Room.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoste <acoste@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 00:11:43 by acoste            #+#    #+#             */
/*   Updated: 2025/09/26 03:10:39 by acoste           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOM_H
# define ROOM_H

#include "ClientSocketClass.hpp"
#include "ServerClass.hpp"

class Room {

	public :

	Room();
	Room(std::string name, ClientSocket *client);
	Room(Room const &cpy);
	Room	const &operator=(Room const &dupp);
	~Room();

	void	settopic(std::string topic);
	// void	manipclient(ClientSocket client, std::string param);
	// void	addcli(ClientSocket client);
	std::string getName();
	std::string getTopic();
	std::vector<ClientSocket> getClient();

	private :

	std::string _name;
	std::string _topic;
	std::vector<ClientSocket> _client;
	std::vector<ClientSocket> _admin;
	// std::map<int, std::pair<bool, ClientSocket*>> _map;

};


#endif