#ifndef CLIENT_SOCKETS_CLASS_HPP
#define CLIENT_SOCKETS_CLASS_HPP

#include "ASocketClass.hpp"
#include "RequestClass.hpp"
#include "ResponseClass.hpp"
#include <cstdlib>
#include "poll.h"
#include <fstream>
#include <sstream>
#include "utils.hpp"
#include <climits>

class Room;

class ClientSocket : public ASocket {

	public:
		ClientSocket(int listenFd, std::string pwd, std::vector<Room*> *rooms);
		~ClientSocket();

		int 						connect();
		std::vector<ClientSocket*>	*clientSocks;
		std::vector<pollfd>			*pollVec;
		size_t						pollIndex;

		void						sendResponse();
		void						addResponse(std::string);
		void							addRoom(std::string roomName);
		void						interact();
		void						execute(std::string cmd, std::string args, Response	&response);
		void						interactcmd(std::string cmd, std::string args, Response &response);
		static bool					isacmd(std::string cmd);
		std::vector<std::string>	split(std::string str);
		std::vector<std::string>	kick_split(std::string str);

		std::string getpwd();
		std::string getnick();
		std::string getusername();

		int	ping(std::string args, Response& response);
		int	mode(std::string args, Response& response);
		int	user(std::string args, Response& response);
		int	join(std::string args, Response &response);
		int	privmsg(std::string args, Response &response);
		int	part(std::string args, Response &response);
		int	kick(std::string args, Response &response);
		int	kick_user_check(std::string user, Room *salon);
		int	kick_user(std::string user, std::string comment, Room *salon, Response &response);

		private:

		socklen_t 	_len;
		int			_listenFd;
		bool		_unlocked;
		bool		_connected;
		void		_unlock(Response	&response, std::string cmd, std::string args);
		int			_isRoom(std::string roomName);
		void		_interactMode(std::string modes, std::string modesArgs, Room *room);

		std::vector< std::pair<std::string, std::string> >	_parseJoinArgs(std::string args);
		std::vector<std::string>				_parsePartArgs(std::string args);
		size_t			_nbRooms;
		const size_t	_maxNbRooms;
		std::map<std::string, int (ClientSocket::*)(std::string, Response&)> cmdsMap;
		Request		_request;
		std::string _response;
		std::string	_pwd;
		std::string _nick;
		std::string	_username;
		std::string _hostname;
		std::string _servername;
		std::string _realname;
		std::vector<Room*> *_rooms;
		std::vector<std::string> _roomsNames;



};

#endif
