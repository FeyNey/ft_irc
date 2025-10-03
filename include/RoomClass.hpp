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
		std::vector<std::string>	_inviteNick;
		std::string _name;
		std::string _topic;
		size_t		_maxUser;
		size_t		_nbUser;
		bool		_iMode;
		/* bool		_tMode;
		bool		_kMode;
		bool		_oMode;
		bool		_lMode; */

		bool	_isOp(std::string nick);
		bool	_isInvited(std::string nick);

};

#endif
