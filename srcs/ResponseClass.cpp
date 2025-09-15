#include "ResponseClass.hpp"

Response::Response(Request &request) : _request(request)
{

}

void Response::makeResponse(bool unlocked, bool connected, std::string user)
{
	std::string cmd;
	std::string args;


	_response = "";
	cmd = _request.getCmd();
	args = _request.getArgs();
	if (cmd.compare("CAP") == 0)
	{
		if (args[0] == 'L' && args[1] == 'S')
			_response = ":monserv CAP * LS :\r\n";
		else if (args[0] == 'R' && args[1] == 'E' && args[2] == 'Q')
			_response = ":monserv CAP * NAK :\r\n";
		else if (args[0] == 'E' && args[1] == 'N' && args[1] == 'D')
			_response = "";
		return;
	}
	else if(!unlocked)
	{
		if (cmd.compare("PASS") == 0)
			_response = ":monserv 464 * :Password incorrect\r\n";
		else
			_response = ":monserv 464 * :Password required\r\n";
		return;
	}
	else if (cmd.compare("PASS") == 0)
	{
		_response = "";
	}
	else if ((cmd.compare("NICK") == 0 || cmd.compare("USER") == 0) && connected)
	{
		_response = ":monserv 001 " + user + " :Welcome...\r\n"
		":monserv 002 " + user + " :Your host is monserv, running version 1.0\r\n"
		":monserv 003 " + user + " :This server was created...\r\n"
		":monserv 004 " + user + " monserv 1.0 o o\r\n"
		":monserv 375 " + user + " :- MOTD start -\r\n"
		":monserv 372 " + user + " :- Bienvenue !\r\n"
		":monserv 376 " + user + " :End of /MOTD command.\r\n";
	}
}

std::string Response::str()
{
	return (_response);
}

size_t Response::size()
{
	return (_response.size());
}

Response::~Response()
{

}
