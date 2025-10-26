/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClass.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoste <acoste@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:51:28 by acoste            #+#    #+#             */
/*   Updated: 2025/10/26 21:11:02 by acoste           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "ClientSocketClass.hpp"
#include "ListenSocketClass.hpp"
#include "RoomClass.hpp"
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
		static bool flag;

	private :

		ListenSocket				_listenSock;
		std::vector<ClientSocket*>	_clientSocks;
		std::vector<Room*>			_rooms;
		std::vector<std::string>	roomNames;
		std::vector<pollfd>			_pollVec;
		std::string					_pwd;
		size_t						_nbClients;
		int							_fdLSock;
		void						_launchListenSocket();
		void						_createClient();
		void						_pollLoop();
};

#endif
