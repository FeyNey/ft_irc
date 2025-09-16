#include <ClientSocketClass.hpp>

ClientSocket::ClientSocket(int listenFd, std::string pwd) : _listenFd(listenFd), _unlocked(false), _connected(false), _pwd(pwd), _nick(""), _username("")
{
	std::cout << "new client has been created" << std::endl;
	_len = sizeof(_addr);
	_key = 0;
}

int ClientSocket::connect()
{
	_fd = accept(_listenFd, (struct sockaddr *)&_addr, &_len);
	fcntl(_fd, F_SETFL, O_NONBLOCK);
	if (_fd == -1)
		throw std::runtime_error("ERROR : Accept failed");
	_connected = true;
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
	else if (cmd.compare("PASS") == 0 && (_key == 0))
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
	else if (cmd.compare("JOIN") != 0)
	{
		response.makeResponse(_unlocked, false, "", cmd, args);
		_poll->events = POLLOUT;
	}
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
		if(_nick.compare("") != 0 && _username.compare("") != 0)
		{
			_connected = true;
			response.makeResponse(true, _connected, _nick, cmd, args);
			_response = response.str().c_str();
			_poll->events = POLLOUT;
			_key = 1;
		}
		if (_key == 1)
		{
			response.interactcmd(this, cmd, args);
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
