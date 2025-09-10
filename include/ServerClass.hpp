#ifndef SERVER_HPP
#define SERVER_HPP

#include "ClientSocketClass.hpp"
#include "ListenSocketClass.hpp"
#include "poll.h"
#include <vector>
#include <cstdlib>

class Server {

	public :

		Server();
		Server(int port, std::string pwd);
		~Server();

		void	launchListenSocket();
		void	pollLoop();
		static void signal_handler(int sig);

		static bool signal;

	private :

		ListenSocket				_listenSock;
		std::vector<ClientSocket*>	_clientSocks;
		std::vector<pollfd>			_pollVec;
		std::string					_pwd;
		size_t						_nbClients;
		int							_fdLSock;
		void						_launchListenSocket();
		void						_createClient();
		void						_pollLoop();
};

#endif
