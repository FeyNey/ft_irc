#ifndef CLIENT_SOCKETS_CLASS_HPP
#define CLIENT_SOCKETS_CLASS_HPP

#include "ASocketClass.hpp"
#include "RequestClass.hpp"
#include "ResponseClass.hpp"
#include "poll.h"
#include <fstream>
#include <sstream>

class ClientSocket : public ASocket {

	public:
		ClientSocket(int listenFd, std::string pwd);
		~ClientSocket();

		int connect();
		void	sendResponse();
		void interact();
		pollfd		*_poll;
		std::string getpwd();
		std::string getnick();
		std::string getusername();


		private:

		socklen_t 	_len;
		int			_listenFd;
		bool		_unlocked;
		bool		_connected;
		void _unlock(Response	&response);
		Request		_request;
		std::string _response;
		std::string	_pwd;
		std::string _nick;
		std::string	_username;


};

#endif
