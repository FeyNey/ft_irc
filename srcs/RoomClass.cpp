#include "RoomClass.hpp"

Room::Room(std::string name, ClientSocket* clientSock) : _name(name), _topic(""), _pwd(""), _maxUser(UINT_MAX), _nbUser(0), _iMode(false), _kMode(false)
{
	_opsNick.push_back(clientSock->getnick());
	join(clientSock, "x");
	std::cout << "Room #" << name << " created" << std::endl;
}

bool	Room::_isOp(std::string nick)
{
	for (size_t i = 0; i < _opsNick.size(); i++)
		if(nick.compare(_opsNick[i]) == 0)
			return (true);
	return (false);
}

bool	Room::_isInvited(std::string nick)
{
	for (size_t i = 0; i < _inviteNick.size(); i++)
		if(nick.compare(_inviteNick[i]) == 0)
			return (true);
	return (false);
}


int	Room::join(ClientSocket* clientSock, std::string pwd)
{
	std::string response;
	std::string nameList;

	std::cout << _name << "|" << _pwd << "|" << pwd << "|" << _kMode << std::endl;
	if (_kMode && _nbUser > 0 && pwd.compare(_pwd) != 0)
		return(	clientSock->addResponse(":monserv 475 " + clientSock->getnick() + " #" + _name + " :Cannot join channel (+k)" ), 1);
	if(_nbUser + 1 == _maxUser)
		return(	clientSock->addResponse(":monserv 471 " + clientSock->getnick() + "#" + _name + " :Cannot join channel (+l)" ), 1);
	else if(_iMode)
		if (!_isInvited(clientSock->getnick()))
			return(	clientSock->addResponse(":monserv 473 " + clientSock->getnick() + "#" + _name + " :Cannot join channel (+i)" ), 1);
	response = ":" + clientSock->getnick() + "!" + clientSock->getusername()
	+ "@monserv JOIN :#" + _name;
	_clientSocks.push_back(clientSock);
	clientSock->addRoom(_name);
	_nbUser++;
	for (size_t i = 0; i < _clientSocks.size(); i++)
		_clientSocks[i]->addResponse(response);
	if(!_topic.empty())
		clientSock->addResponse(":monserv 332 " + clientSock->getnick() + " #" + _name + " :" + _topic);
	for (size_t i = 0; i < _clientSocks.size(); i++)
	{
		if(_isOp(_clientSocks[i]->getnick()))
			nameList += "@";
		nameList += _clientSocks[i]->getnick();
		if (i + 1 < _clientSocks.size())
			nameList += " ";
	}
	clientSock->addResponse(":monserv 353 " + clientSock->getnick() + " = #" + _name + " :" + nameList);
	clientSock->addResponse(":monserv 366 " + clientSock->getnick() + " #" + _name + " :End of /NAMES list");
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
