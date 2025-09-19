#include <ClientSocketClass.hpp>

ClientSocket::ClientSocket(int listenFd, std::string pwd) : _listenFd(listenFd), _unlocked(false), _connected(false), _pwd(pwd), _nick(""), _username("")
{
	std::cout << "new client has been created" << std::endl;
	cmdsMap["PING"] = &ClientSocket::ping;
	cmdsMap["MODE"] = &ClientSocket::mode;
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
		_poll->events = POLLOUT;

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
			_poll->events = POLLOUT;
		}
	}
	else
	{
		response.makeResponse(_unlocked, false, "", cmd, args);
		_poll->events = POLLOUT;
	}
}

bool	ClientSocket::isacmd(std::string cmd)
{
	static const char* commands[] = {
		"PING", "MODE"/*, "USER", "PASS", "JOIN", "PART", "QUIT", "PRIVMSG", "NOTICE",
		"TOPIC", "NAMES", "LIST", "WHO", "WHOIS", "WHOWAS", "NICK", "KICK", "INVITE",
		"OPER", "DIE", "RESTARD", "KILL", "SQUIT", "CONNECT" */ };

	for (int i = 0; i < 2; i++)
	{
		if (cmd == commands[i])
			return 1;
	}
	return (0);
}

void ClientSocket::execute(std::string cmd, std::string args, Response	&response)
{
	if (_unlocked == false)
	{
		_unlock(response, cmd, args);
		_response = response.str().c_str();
		_poll->events = POLLOUT;
	}
	else
	{
		if(cmd.compare("CAP") == 0)
		{
			response.makeResponse(true, _connected, _nick, cmd, args);
			_response = response.str().c_str();
			_poll->events = POLLOUT;
		}
		else if(cmd.compare("NICK") == 0)
		{
			_nick = args;
		}
		else if(cmd.compare("USER") == 0)
		{
			_username = args;
		}
		if(_nick.compare("") != 0 && _username.compare("") != 0 && !_connected)
		{
			_connected = true;
			response.makeResponse(true, _connected, _nick, cmd, args);
			_response = response.str().c_str();
			_poll->events = POLLOUT;
		}
		else if (_connected)
		{
			interactcmd(cmd, args, response);
			_response = response.str();
			_poll->events = POLLOUT;
			return ;
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
	_poll->events = POLLIN;
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

void	ClientSocket::interactcmd(std::string cmd, std::string args, Response &response)
{
	if (!isacmd(cmd))
		response.addResponse(":myserver 421 " + _username + " " + cmd + ":Unknown command", _poll);
	else
		(this->*cmdsMap[cmd])(args, response);
}

int	ClientSocket::ping(std::string args, Response &response)
{
	if (args.empty() == 1)
		response.addResponse(":myserver 409 " + _username + "No origin specified", _poll);
	else //not found
	{
		response.addResponse(":monserv PONG monserv :" + args, _poll);
		return (1);
	}
	return(1);
}

int	ClientSocket::mode(std::string args, Response &response)
{
	/* if (args.empty() == 1)
		response.addResponse(":myserver 409 " + _username + "No origin specified"); */
	if (args.find("+i")) //found
	{
		std::string nick = args.substr(0, args.find(" "));
		if (nick.compare(_nick) != 0)
			response.addResponse(":myserv 502" + nick + ":Cannot change mode for other users", _poll);
		else
			response.addResponse(":" + _nick + "!" + _username + "@monserv MODE " + _nick +" +i", _poll);
	}
	else //not found
	{
		response.addResponse(":myserv PONG server :" + args, _poll);
		return (1);
	}
	return(1);
}
