#include <ClientSocketClass.hpp>

ClientSocket::ClientSocket(int listenFd, std::string pwd) : _listenFd(listenFd), _unlocked(false), _connected(false), _pwd(pwd), _nick(""), _username("")
{
	std::cout << "new client has been created" << std::endl;
	_len = sizeof(_addr);
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

void ClientSocket::_unlock(Response	&response)
{
	std::string	cmd;
	std::string	args;
	cmd = _request.getCmd();
	args = _request.getArgs();
	if(cmd.compare("CAP") == 0)
	{
		response.makeResponse(_unlocked, false, "");
		_poll->events = POLLOUT;
		return;
	}
	else if (cmd.compare("PASS") == 0)
	{
		if (args == _pwd)
		{
			_unlocked = true;
			response.makeResponse(_unlocked, false, "");
			_poll->events = POLLOUT;

		}
		else
		{
			response.makeResponse(_unlocked, false, "");
			_poll->events = POLLOUT;
		}
	}
	else
	{
		response.makeResponse(_unlocked, false, "");
		_poll->events = POLLOUT;
	}
}

void ClientSocket::interact()
{
	std::string	cmd;
	std::string	args;
	_request.receive(_fd);
	_request.show();
	cmd = _request.getCmd();
	args = _request.getArgs();

	// if (cmd == "ADMIN")
	// {
	// 	_unlocked = true;
	// }


	Response	response(_request);

	if (_unlocked == false)
	{
		_unlock(response);
		_response = response.str().c_str(); //?
		_poll->events = POLLOUT;
		return ;
	}
	else
	{
		if(cmd.compare("CAP") == 0)
		{
			response.makeResponse(true, _connected, _nick);
			_response = response.str().c_str();
			_poll->events = POLLOUT;
		}
		else if(cmd.compare("NICK") == 0) // verify that the nickname is not used ? \ // Message confirmation
			_nick = args;
		else if(cmd.compare("USER") == 0)
			_username = args; // hostname and real name to set too
		if(_nick.compare("") != 0 && _username.compare("") != 0)
		{
			_connected = true;
			response.makeResponse(true, _connected, _nick);
			_response = response.str().c_str(); //?
			_poll->events = POLLOUT;
		}
		return ;
	}

	// if (_unlocked == true && Response::isacmd(cmd))
	// {
	// 	response.interactcmd(*this, cmd, args);
	// 	_response = response.str().c_str();
	// 	_poll->events = POLLOUT;
	// }
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