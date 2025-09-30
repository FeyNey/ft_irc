#include <ClientSocketClass.hpp>
#include "RoomClass.hpp"

ClientSocket::ClientSocket(int listenFd, std::string pwd, std::vector<Room*> *rooms) : _listenFd(listenFd), _unlocked(false), _connected(false), _response(""), _pwd(pwd), _nick(""), _username(""), _rooms(rooms)
{
	std::cout << "new client has been created" << std::endl;
	cmdsMap["PING"] = &ClientSocket::ping;
	cmdsMap["MODE"] = &ClientSocket::mode;
	cmdsMap["USER"] = &ClientSocket::user;
	cmdsMap["JOIN"] = &ClientSocket::join;
	cmdsMap["PRIVMSG"] = &ClientSocket::privmsg;
	_len = sizeof(_addr);
}

int ClientSocket::connect()
{
	_fd = accept(_listenFd, (struct sockaddr *)&_addr, &_len);
	fcntl(_fd, F_SETFL, O_NONBLOCK);
	if (_fd == -1)
		throw std::runtime_error("ERROR : Accept failed");
	return (_fd);
}

void ClientSocket::_unlock(Response	&response, std::string cmd, std::string args)
{
	if(cmd.compare("CAP") == 0)
	{
		response.makeResponse(_unlocked, false, "", cmd, args);
		(*pollVec)[pollIndex].events = POLLOUT;

		return;
	}
	else if (cmd.compare("PASS") == 0)
	{
		if (args == _pwd)
		{
			_unlocked = true;
		}
		else
		{
			response.makeResponse(_unlocked, false, "", cmd, args);
			(*pollVec)[pollIndex].events = POLLOUT;
		}
	}
	else
	{
		response.makeResponse(_unlocked, false, "", cmd, args);
		(*pollVec)[pollIndex].events = POLLOUT;
	}
}


void ClientSocket::execute(std::string cmd, std::string args, Response	&response)
{
	if (_unlocked == false)
	{
		_unlock(response, cmd, args);
		_response += response.str().c_str();
		(*pollVec)[pollIndex].events = POLLOUT;
	}
	else
	{
		if(cmd.compare("CAP") == 0)
		{
			response.makeResponse(true, _connected, _nick, cmd, args);
			_response += response.str().c_str();
			(*pollVec)[pollIndex].events = POLLOUT;
		}
		else if(cmd.compare("NICK") == 0)
			_nick = args;
		else if(cmd.compare("USER") == 0)
			(this->*cmdsMap["USER"])(args, response);
		else if (_connected)
		{
			interactcmd(cmd, args, response);
			_response += response.str();
			(*pollVec)[pollIndex].events = POLLOUT;
			return ;
		}
		if(_nick.compare("") != 0 && _username.compare("") != 0 && !_connected)
		{
			_connected = true;
			response.makeResponse(true, _connected, _nick, cmd, args);
			_response += response.str().c_str();
			(*pollVec)[pollIndex].events = POLLOUT;
		}

	}
}

void ClientSocket::interact()
{
	std::string	cmd;
	std::string	args;
	_request.receive(_fd);
	_request.show();
	Response	response(_request);
	for (size_t i = 0; i < _request.size(); i++)
	{
		cmd = _request.getCmd();
		args = _request.getArgs();
		execute(cmd, args, response);
	}
}

void	ClientSocket::sendResponse()
{
	if (_response.size() > 0)
	{
		send(_fd, _response.c_str(), _response.size(), 0);
		std::cout << "Reponse : " << _response << std::endl;
	}
	(*pollVec)[pollIndex].events = POLLIN;
	_response.clear();
	_request.clear();
}

ClientSocket::~ClientSocket()
{
	std::cout << "Client Destroyed" << std::endl;
	close(_fd);
}

std::string	ClientSocket::getpwd()
{
	return(_pwd);
}
std::string	ClientSocket::getnick()
{
	return(_nick);
}
std::string	ClientSocket::getusername()
{
	return(_username);
}

bool	ClientSocket::isacmd(std::string cmd)
{
	static const char* commands[] = {
		"PING", "MODE", "USER", "JOIN", "PRIVMSG"/*, "PART", "QUIT", "PASS", "NOTICE",
		"TOPIC", "NAMES", "LIST", "WHO", "WHOIS", "WHOWAS", "NICK", "KICK", "INVITE",
		"OPER", "DIE", "RESTARD", "KILL", "SQUIT", "CONNECT" */ };

	for (int i = 0; i < 5; i++)
	{
		if (cmd == commands[i])
			return 1;
	}
	return (0);
}

std::vector<std::string> ClientSocket::split(std::string str)
{
	int							pos = 0;
	std::vector<std::string>	requests;

	for(size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == ' ')
		{
			requests.push_back(str.substr(pos, i - pos));
			pos = i + 1;
		}
	}
	return (requests);
}
void	ClientSocket::interactcmd(std::string cmd, std::string args, Response &response)
{
	if (!isacmd(cmd))
		(*pollVec)[pollIndex].events = response.addResponse(":myserver 421 " + _username + " " + cmd + " :Unknown command");
	else
		(this->*cmdsMap[cmd])(args, response);
}

int	ClientSocket::ping(std::string args, Response &response)
{
	if (args.empty() == 1)
		(*pollVec)[pollIndex].events = response.addResponse(":myserver 409 " + _username + "No origin specified");
	else //not found
	{
		(*pollVec)[pollIndex].events = response.addResponse(":monserv PONG monserv :" + args);
		return (1);
	}
	return(1);
}

int	ClientSocket::mode(std::string args, Response &response)
{
	int find = 0;
	if (args.empty() == 1)
		(*pollVec)[pollIndex].events = response.addResponse(":monserv 409 " + _username + ":No origin specified");
	std::string nick = args.substr(0, args.find(" "));
	for (size_t i = 0; i < clientSocks->size(); i++)
		if (nick.compare((*clientSocks)[i]->getnick()) == 0)
			find = 1;
	if (find == 0)
		return((*pollVec)[pollIndex].events = response.addResponse(":monserv 401 " + _username + " " + nick + ":No such nick"), 1);
	else if (nick.compare(_nick) != 0)
		return((*pollVec)[pollIndex].events = response.addResponse(":monserv 502 " + _username + ":Cant change mode for other users"), 1);
	else if (nick.size() == args.size())
		return((*pollVec)[pollIndex].events = response.addResponse(":monserv 221 " + _username + " :+i"), 1); // A CODER (RENVOYER TOUT LES MODES ACTIFS)
	if (args.find("+i") != std::string::npos)
		(*pollVec)[pollIndex].events = response.addResponse(":" + _nick + "!" + _username + "@monserv MODE " + _nick +" +i");
	else if (args.find("+i") != std::string::npos)
		(*pollVec)[pollIndex].events = response.addResponse(":" + _nick + "!" + _username + "@monserv MODE " + _nick +" +i");
	else
		(*pollVec)[pollIndex].events = response.addResponse(":" + _username + "@monserv MODE " + _nick +" +i");
	return(1);
}

int	ClientSocket::user(std::string args, Response &response)
{
	(void)response;
	std::vector<std::string> argsVec = split(args);
	_username = argsVec[0];
	_hostname = argsVec[1];
	_servername = argsVec[2];
	_realname = argsVec[3];
	return(0);
}

int ClientSocket::_isRoom(std::string roomName)
{
	for (size_t i = 0; i < _rooms->size(); i++)
	{
		if ((*_rooms)[i]->getName() == roomName)
			return(1);
	}
	return(0);
}

int	ClientSocket::join(std::string args, Response &response)
{
	(void) response;
	if(args[0] != '#')
		return(_response += ":monserv : erreur a coder", 1);

	std::string roomName;
	roomName = args.substr(1, args.size() - 1);
	_roomsNames.push_back(roomName);
	for (size_t i = 0; i < (*_rooms).size(); i++)
	{
		if ((*_rooms)[i]->getName() == roomName)
		{
			(*_rooms)[i]->join(this);
			return (0);
		}
	}
	(*_rooms).push_back(new Room(roomName, this));
	return(0);
}

int	ClientSocket::privmsg(std::string args, Response &response)
{
	(void)response;
	std::string msg;
	if(args[0] == '#')
	{
		std::string roomName;
		roomName = args.substr(1, args.find(' ') - 1);
		std::cout << roomName << std::endl;
		for (size_t i = 0; i < _roomsNames.size(); i++)
		{
			if (roomName == _roomsNames[i])
			{
				for (size_t j = 0; j < (*_rooms).size(); j++)
				{
					if ((*_rooms)[j]->getName() == roomName)
						(*_rooms)[j]->sendMsg(args.substr(args.find(":"), args.size() - args.find(":")), this);
				}
			}
		}
	}
	return (0);

}

void ClientSocket::addResponse(std::string response)
{
	_response += response + "\r\n";
	(*pollVec)[pollIndex].events = POLLOUT;
}
