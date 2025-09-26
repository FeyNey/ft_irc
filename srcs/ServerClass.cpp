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
	ClientSocket *csock = new ClientSocket(_fdLSock, _pwd, this);
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
			// std::cout << _clientSocks[i - 1]->getusername() << "<-- username of my client" << std::endl;
		}
}

void	Server::signal_handler(int sig)
{
	(void)sig;
	signal = true;
}

int	Server::isasalon(std::string arg)
{
	size_t i;

	i = 0;
	while (i < _salon.size())
	{
		if (_salon[i].getName() == arg)
			return (1);
		i++;
	}
	return (0);
}

std::vector<Room>	Server::getSalon()
{
	return(_salon);
}

void	Server::create_salon(ClientSocket *user, std::string name)
{
	Room Salon(name, user);

	this->_salon.push_back(Salon);
}

void	Server::sendmsgsalon(std::string salon, std::string msg)
{
	size_t i;

	i = 0;
	while (i < _salon.size())
	{
		if (_salon[i].getName() == salon)
		{
			for (size_t j; j = 0; j < _salon[i].getClient().size())
			{
				_salon[i].getClient()[j].set_response(msg);
				_salon[i].getClient()[j]._poll->revents = POLLOUT;
			}
			break;
		}
		i++;
	}
}


