/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocketClass.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evella <evella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:51:14 by acoste            #+#    #+#             */
/*   Updated: 2025/10/31 14:30:40 by evella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include <ctime>

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
		void	sendMsgCli(ClientSocket *me, ClientSocket *receip, std::string response);

		std::string	getpwd();
		std::string	getnick();
		bool		getQuit();
		void		quitting();
		std::string	getusername();
		void	setPart(bool state);
		static std::string	stash;

		int	ping(std::string args, Response& response);
		int	mode(std::string args, Response& response);
		int	user(std::string args, Response& response);
		int	nick(std::string args, Response &response);
		int	join(std::string args, Response &response);
		int	privmsg(std::string args, Response &response);
		int	part(std::string args, Response &response);
		int	invite(std::string args, Response &response);
		int	topic(std::string args, Response &response);
		int	kick(std::string args, Response &response);
		int	quit(std::string args, Response &response);
		int	repeat(std::string args, Response &response);
		int	note(std::string args, Response &respomse);


		private:

		socklen_t 	_len;
		int			_listenFd;
		bool		_unlocked;
		bool		_connected;
		bool		_quit;
		bool		_part;
		void		_unlock(Response	&response, std::string cmd, std::string args);
		int			_isRoom(std::string roomName);
		void		_interactMode(std::string modes, std::string modesArgs, Room *room);
		int			kick_user(std::string user, std::string comment, Room *salon, Response &response);
		int			kick_user_check(std::string user, Room *salon);
		void		decrPoll();


		std::vector< std::pair<std::string, std::string> >	_parseJoinArgs(std::string args);
		std::vector<std::string>							_parsePartArgs(std::string args);
		std::vector<std::string> 							kick_split(std::string str);

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
