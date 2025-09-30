#ifndef ROOM_HPP
#define ROOM_HPP

#include "ClientSocketClass.hpp"

class Room {

	public:
		Room(std::string name, ClientSocket* clientSock);
		~Room();

		int	join(ClientSocket* clientSock);
		int	sendMsg(std::string msg, ClientSocket* sender);
		std::string	getName();

	private:
		std::vector<ClientSocket*>	_clientSocks;
		std::vector<std::string>	_opsNick;
		std::string _name;
		std::string _topic;

};

#endif
