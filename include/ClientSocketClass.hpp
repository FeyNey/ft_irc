#ifndef CLIENT_SOCKETS_CLASS_HPP
#define CLIENT_SOCKETS_CLASS_HPP

#include "ASocketClass.hpp"
#include "RequestClass.hpp"
#include "ResponseClass.hpp"
#include "poll.h"
#include <fstream>
#include <sstream>
#include "ServerClass.hpp" // ?
// #include <Room.hpp>

class Server;

class ClientSocket : public ASocket {

	public:
		ClientSocket(int listenFd, std::string pwd, Server *serv);
		ClientSocket(ClientSocket const &cpy);
		~ClientSocket();
		ClientSocket const &operator=(ClientSocket const &dupp);

		int		connect();
		void	sendResponse();
		void	interact();
		void	execute(std::string cmd, std::string args, Response	&response);
		void	interactcmd(std::string cmd, std::string args, Response &response);
		static bool	isacmd(std::string cmd);
		std::vector<std::string> split(std::string str);
		std::vector<std::string> ClientSocket::split_on_first(std::string str);


		void set_response(std::string response);
		pollfd		*_poll;
		std::string getpwd();
		std::string getnick();
		std::string getusername();
		int ft_find(std::string str, char c);

		int	ping(std::string args, Response& response);
		int	mode(std::string args, Response& response);
		int	user(std::string args, Response& response);
		int join(std::string args, Response &response);
		int prvmsg(std::string args, Response &response);

		private:

		socklen_t 	_len;
		int			_listenFd;
		bool		_unlocked;
		bool		_connected;
		void _unlock(Response	&response, std::string cmd, std::string args);
		std::map<std::string, int (ClientSocket::*)(std::string, Response&)> cmdsMap;
		Request		_request;
		std::string _response;
		std::string	_pwd;
		std::string _nick;
		std::string	_username;
		std::string _hostname;
		std::string _servername;
		std::string _realname;
		Server			*_serv;


};

#endif
