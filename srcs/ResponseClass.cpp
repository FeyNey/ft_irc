#include "ResponseClass.hpp"
#include "ClientSocketClass.hpp"

Response::Response(Request &request) : _request(request)
{
	_response = "";
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
		else if (args[0] == 'E' && args[1] == 'N' && args[2] == 'D')
			_response = ":monserv CAP * END:\r\n";
		return;
	}
	else if(!unlocked)
	{
		if (cmd.compare("PASS") == 0)
		{
			_response = ":monserv 464 * :Password incorrect\r\n";
		}
		else
			_response = ":monserv 464 * :Password required\r\n";
		return;
	}
	else if (cmd.compare("PASS") == 0)
	{
		_response = ":monserv 464 * :Password is valid\r\n"; // Faire une diff si c est la deuxieme fois ??
	}
//
	// if (cmd.compare("NICK") == 0 && !connected)
	// {
		// _response = ":monserv 001 ";
	// }
//
//

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

bool	Response::isacmd(std::string cmd)
{
	static const char* commands[] = {
		"NICK", "USER", "PASS", "PING", "JOIN", "PART", "QUIT", "PRIVMSG", "NOTICE",
		"TOPIC", "NAMES", "LIST", "WHO", "WHOIS", "WHOWAS", "MODE", "KICK", "INVITE",
		"OPER", "DIE", "RESTARD", "KILL", "SQUIT", "CONNECT" };

	for (int i = 0; i < 24; i++)
	{
		if (cmd == commands[i])
		{
			std::cout << "cmd, foud it" << std::endl;
			return 1;
		}
	}
	std::cout << "cmd not found" << std::endl;
	return (0);
}

std::string	Response::ping(std::string args, std::string username)
{
	if (args.empty() == 1)
		return (":myserver 409 " + username + "No origin specified");
	else if (args.find(" ") != std::string::npos) //found
	{
		std::string arg;

		for (int i = 0; (args.c_str())[i] != ' ' && args.c_str(); i++)
		{
			arg = arg + (args.c_str())[i];
		}
		std::cout << "arg" << arg << std::endl; //debug a verif la suite de la commande a ete supp;
		return("myserv PONG server :" + arg);
	}
	else //not found
	{
		return (":myserv PONG server :" + args);
	}
}

void	Response::interactcmd(ClientSocket *client, std::string cmd, std::string args)
{
	_response = " ";

	if (!isacmd(cmd))
	{
		_response = ":myserver 421 " + client->getusername() + " " + cmd + ":Unknown command\r\n";
	}
	if (cmd.compare("PING") == 0)
		_response = ping(args, client->getusername());
	std::cout << "okay then" << std::endl;

}
