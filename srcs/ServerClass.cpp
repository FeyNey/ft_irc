/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoste <acoste@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:51:04 by acoste            #+#    #+#             */
/*   Updated: 2025/10/27 13:40:00 by acoste           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerClass.hpp>

bool Server::signal = false;
bool Server::flag = false;

Server::Server() : _nbClients(0), _fdLSock(0)
{

}

Server::Server(int port, std::string pwd) : _nbClients(0), _fdLSock(0)
{
	_pwd = pwd;
	_listenSock.setport(port);
}

Server::~Server()
{
	//liberer memoire client; TODO
	// std::cout << "Serveur Destructor called" << std::endl;
}

void	Server::launchListenSocket()
{
	pollfd pnode;

	_fdLSock = _listenSock.connect();
	pnode.fd = _fdLSock;
	pnode.events = POLLIN;
	pnode.revents = 0;
	_pollVec.push_back(pnode);
}

void	Server::_createClient()
{
	ClientSocket *csock = new ClientSocket(_fdLSock, _pwd, &_rooms);
	pollfd	pollNodeTmp;
	std::cout << "CLIENT CONNECTED" << std::endl;
	pollNodeTmp.fd = csock->connect();
	pollNodeTmp.events = POLLIN;
	pollNodeTmp.revents = 0;
	_pollVec.push_back(pollNodeTmp);
	csock->pollIndex = _pollVec.size() - 1;
	csock->pollVec = &_pollVec;
	_clientSocks.push_back(csock);
	csock->clientSocks = &_clientSocks;
	_nbClients++;
}

void	Server::pollLoop()
{
	poll(&_pollVec[0], _nbClients + 1, 1000);
	for (size_t i = 0; i < _nbClients + 1; i++)
	{
		if (flag == true)
		{
				for (std::vector<ClientSocket *>::iterator it = _clientSocks.begin(); it != _clientSocks.end(); ++it)
				{
					(*it)->quitting();
				}
		}
		if (_pollVec[i].revents == POLLIN)
		{
			i == 0 ?  _createClient() : _clientSocks[i-1]->interact();
		}
		else if (_pollVec[i].revents == POLLOUT)
		{
			_clientSocks[i-1]->sendResponse();
			if (_clientSocks[i-1]->getQuit())
			{
				std::cout << "CLIENT HAS QUITTED" << std::endl;
				delete(_clientSocks[i-1]);
				_clientSocks.erase(_clientSocks.begin() + i - 1);
				for (std::vector<ClientSocket *>::iterator it = _clientSocks.begin(); it != _clientSocks.end(); ++it)
				{
					if ((*it)->pollIndex > i)
						(*it)->pollIndex--;
				}
				_pollVec.erase(_pollVec.begin() + i);
				_nbClients--;
				// if (flag == true && _nbClients == 0)
					// signal = true;
			}
		}
	}
	if (flag == true && _nbClients == 0)
		signal = true;
}

void	Server::signal_handler(int sig)
{
	(void)sig;
	flag = true;
}
