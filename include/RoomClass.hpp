#ifndef ROOM_HPP
#define ROOM_HPP

#include "ClientSocketClass.hpp"

class Room {

	public:

		Room(std::string name, ClientSocket* clientSock);
		~Room();

		int		join(ClientSocket* clientSock, std::string pwd);
		int		sendMsg(std::string msg, ClientSocket* sender);
		int		sendPartMsg(std::string msg, ClientSocket* sender);
		void	part(ClientSocket *clientSock, std::string msg);
		void	kickpart(ClientSocket *clientSock);
		void	kick(ClientSocket *user, ClientSocket *clientSock, std::string msg);
		// int		sendPartMsgKick(ClientSocket *user, std::string msg, ClientSocket* sender);
		void	sendModesChange(std::string modes, std::string modesArgs, ClientSocket* sender);
		int		Kickmsg(std::string msg, ClientSocket* sender, ClientSocket *excluded);


		std::string	getName();
		std::string	getModes();
		bool		isOp(std::string nick);
		bool		is_in_room(std::string nick);
		ClientSocket	*user_on_room(std::string nick);

		void	k(std::string pwd, char op);
		void	o(std::string nick, char op);
		void	l(std::string limit, char op);
		void	i(char op);
		void	t(char op);

	private:

		std::vector<ClientSocket*>	_clientSocks;
		std::vector<std::string>	_opsNick;
		std::vector<std::string>	_inviteNick;
		std::string					_name;
		std::string					_topic;
		std::string					_pwd;
		size_t						_maxUser;
		size_t						_nbUser;
		bool						_iMode;
		bool						_kMode;
		bool						_tMode;
		bool						_lMode;


		bool						_isInvited(std::string nick);

};

#endif
