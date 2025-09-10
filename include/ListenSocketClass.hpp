#ifndef LISTEN_SOCKETS_CLASS_HPP
#define LISTEN_SOCKETS_CLASS_HPP

#include "ASocketClass.hpp"

class ListenSocket : public ASocket {

	public:
		ListenSocket();
		~ListenSocket();

		int connect();
		void setport(int port);


	private :

	int _port;
};

#endif
