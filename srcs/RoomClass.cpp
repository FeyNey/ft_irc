#include "RoomClass.hpp"

Room::Room(std::string name, ClientSocket* clientSock) : _name(name), _topic(""), _pwd(""), _maxUser(UINT_MAX), _nbUser(0), _iMode(false), _kMode(false), _tMode(false), _lMode(false)
{
	_opsNick.push_back(clientSock->getnick());
	join(clientSock, "x");
	std::cout << "Room #" << name << " created" << std::endl;
}

bool	Room::isOp(std::string nick)
{
	for (size_t i = 0; i < _opsNick.size(); i++)
		if(nick.compare(_opsNick[i]) == 0)
			return (true);
	return (false);
}

bool	Room::is_in_room(std::string nick)
{
	for (size_t i = 0; i < _clientSocks.size(); i++)
	if (nick.compare(_clientSocks[i]->getnick()) == 0)
		return (true);
	return (false);
}
ClientSocket	*Room::user_on_room(std::string nick)
{
	for (size_t i = 0; i < _clientSocks.size(); i++)
	if (nick.compare(_clientSocks[i]->getnick()) == 0)
		return (_clientSocks[i]);
	return (NULL);

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

	if (_kMode && _nbUser > 0 && pwd.compare(_pwd) != 0)
		return(	clientSock->addResponse(":monserv 475 " + clientSock->getnick() + " #" + _name + " :Cannot join channel (+k)" ), 1);
	if(_lMode && _nbUser == _maxUser)
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
		if(isOp(_clientSocks[i]->getnick()))
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

int	Room::sendPartMsg(std::string msg, ClientSocket* sender)
{
	std::string response;

	if (msg.empty())
	{
		response = ":" + sender->getnick() + "!" + sender->getusername()
		+ "@monserv PART #" + _name;
	}
	else
	{
		response = ":" + sender->getnick() + "!" + sender->getusername()
		+ "@monserv PART #" + _name + " :" + msg;
	}
	for (size_t i = 0; i < _clientSocks.size(); i++)
			_clientSocks[i]->addResponse(response);
	return (0);
}

/* int	Room::sendPartMsgKick(std::string msg, ClientSocket* sender)
{
	std::string response;

	if (msg.empty())
	{
		response = ":" + sender->getnick() + "!" + sender->getusername()
		+ "@monserv PART #" + _name;
	}
	else
	{
		response = ":" + sender->getnick() + "!" + sender->getusername()
		+ "@monserv PART #" + _name + " :" + msg;
	}
	for (size_t i = 0; i < _clientSocks.size(); i++)
			_clientSocks[i]->addResponse(response);
	return (0);
} */

void	Room::sendModesChange(std::string modes, std::string modesArgs, ClientSocket* sender)
{
	std::string response;

	response = ":" + sender->getnick() + "!" + sender->getusername()
	+ "@monserv MODE #" + _name + " " + modes;
	if(!modesArgs.empty())
		response += " " + modesArgs;
	for (size_t i = 0; i < _clientSocks.size(); i++)
	{
		_clientSocks[i]->addResponse(response);
	}
}


void	Room::k(std::string pwd, char op)
{
	if (op == '-')
	{
		_kMode = false;
		_pwd.clear();
	}
	else if (op == '+')
	{
		_kMode = true;
		_pwd = pwd;
	}
}

void	Room::i(char op)
{
	if (op == '-')
		_iMode = false;
	else if (op == '+')
		_iMode = true;
}

void	Room::t(char op)
{
	if (op == '-')
		_tMode = false;
	else if (op == '+')
		_tMode = true;
}

void	Room::o(std::string nick, char op)
{
	if (op == '-')
	{
		for (std::vector<std::string>::iterator it = _opsNick.begin(); it != _opsNick.end(); ++it)
			if (it->compare(nick) == 0)
				_opsNick.erase(it);
	}
	else if (op == '+')
		_opsNick.push_back(nick);
}

void	Room::l(std::string limit, char op)
{
	if (op == '-')
	{
		_lMode = false;
	}
	else if (op == '+')
	{
		_lMode = true;
		_maxUser = std::atoi(limit.c_str());
	}
}


std::string	Room::getName()
{
	return(_name);
}

std::string	Room::getModes()
{
	std::ostringstream oss;
	oss << _maxUser;
	std::string str = "+";

	if (_kMode)
		str += "k";
	if (_lMode)
		str += "l";
	if (_iMode)
		str += "i";
	if (_tMode)
		str += "t";
	if (_kMode)
		str += " " + _pwd;
	if (_lMode)
		str += " " + oss.str();
	return (str);
}

void	Room::part(ClientSocket *clientSock, std::string msg)
{
	std::string nick = clientSock->getnick();
	if(isOp(nick))
	{
		for (std::vector<std::string>::iterator it = _opsNick.begin(); it != _opsNick.end(); ++it)
		{
			if((*it).compare(nick) == 0)
			{
				_opsNick.erase(it);
				break;
			}
		}
	}
	_nbUser--;
	sendPartMsg(msg, clientSock);
	for (std::vector<ClientSocket*>::iterator it = _clientSocks.begin(); it != _clientSocks.end(); ++it)
	{
		if((*it)->getnick().compare(nick) == 0)
		{
			_clientSocks.erase(it);
			break;
		}
	}
}

Room::~Room()
{

}
