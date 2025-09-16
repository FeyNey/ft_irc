#include <ServerClass.hpp>

bool Server::signal = false;

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
	ClientSocket *csock = new ClientSocket(_fdLSock, _pwd);
	pollfd	pollNodeTmp;
	pollNodeTmp.fd = csock->connect();
	pollNodeTmp.events = POLLIN;
	pollNodeTmp.revents = 0;
	_pollVec.push_back(pollNodeTmp);
	csock->_poll = &_pollVec.back();
	_clientSocks.push_back(csock);
	_nbClients++;
}

void	Server::pollLoop()
{
	poll(&_pollVec[0], _nbClients + 1, 1000);
	for (size_t i = 0; i < _nbClients + 1; i++)
		if (_pollVec[i].revents == POLLIN)
		{
			i == 0 ?  _createClient() : _clientSocks[i-1]->interact();
		}
		else if (_pollVec[i].revents == POLLOUT)
		{
			_clientSocks[i-1]->sendResponse();
			std::cout << _clientSocks[i - 1]->getusername() << "<-- username of my client" << std::endl;
		}
}

void	Server::signal_handler(int sig)
{
	(void)sig;
	signal = true;
}
