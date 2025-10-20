#ifndef ROOM_HPP
#define ROOM_HPP

#include "ClientSocketClass.hpp"

class Room {

	public:

		Room(std::string name, ClientSocket* clientSock);
		~Room();

		int		join(ClientSocket* clientSock, std::string pwd);
		int		sendMsg(std::string msg, ClientSocket* sender, std::string cmd);
		int		sendPartMsg(std::string msg, ClientSocket* sender);
		void	part(ClientSocket *clientSock, std::string msg);
		void	sendModesChange(std::string modes, std::string modesArgs, ClientSocket* sender);
		void	invite(ClientSocket *invited, ClientSocket *inviter);

		std::string	getName();
		std::string	getModes();
		std::string	getTopic();
		std::string	getTopicTime();
		std::string getTopicNick();
		bool		getTmode();
		void		setTopic(std::string newTopic, std::string nick);

		bool			isOp(std::string nick);
		bool			isOnRoom(std::string nick);
		void			changeOpNick(std::string newNick, std::string prevNick);
		ClientSocket*	user_on_room(std::string nick);
		bool			is_in_room(std::string nick);
		void			kickpart(ClientSocket *clientSock);
		void			kick(ClientSocket *user, ClientSocket *client, std::string msg);
		int				Kickmsg(std::string msg, ClientSocket* sender, ClientSocket *excluded);






		void	k(std::string pwd, char op);
		void	o(std::string nick, char op);
		void	l(std::string limit, char op);
		void	i(char op);
		void	t(char op);

	private:

		time_t					_topicTime;
		std::vector<ClientSocket*>	_clientSocks;
		std::vector<std::string>	_opsNick;
		std::vector<std::string>	_inviteNick;
		std::string					_topicNick;
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
