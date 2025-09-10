#ifndef CLIENT_SOCKETS_CLASS_HPP
#define CLIENT_SOCKETS_CLASS_HPP

#include "ASocketClass.hpp"
#include "RequestClass.hpp"
#include "ResponseClass.hpp"
#include <fstream>
#include <sstream>

class ClientSocket : public ASocket {

	public:
		ClientSocket(int listenFd);
		~ClientSocket();

		int connect();
		void interact();

	private:
		socklen_t 	_len;
		int			_listenFd;

};

#endif
