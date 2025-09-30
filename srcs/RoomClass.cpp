#include "RoomClass.hpp"

Room::Room(std::string name, ClientSocket* clientSock) : _name(name)
{
	_opsNick.push_back(clientSock->getnick());
	join(clientSock);
	std::cout << "Room #" << name << " created" << std::endl;
}

int	Room::join(ClientSocket* clientSock)
{
	std::string response;

	response = ":" + clientSock->getnick() + "!" + clientSock->getusername()
	+ "@monserv JOIN :#" + _name;
	_clientSocks.push_back(clientSock);
	for (size_t i = 0; i < _clientSocks.size(); i++)
		_clientSocks[i]->addResponse(response);
	return (0);
}

int	Room::sendMsg(std::string msg, ClientSocket* sender)
{
	std::string response;

	response = ":" + sender->getnick() + "!" + sender->getusername()
	+ "@monserv PRIVMSG #" + _name + " :" + msg;
	for (size_t i = 0; i < _clientSocks.size(); i++)
	{
		if (_clientSocks[i] != sender)
		_clientSocks[i]->addResponse(response);
	}
	return (1);
}

std::string	Room::getName()
{
	return(_name);
}

Room::~Room()
{

}
