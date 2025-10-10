#include <ClientSocketClass.hpp>
#include "RoomClass.hpp"

ClientSocket::ClientSocket(int listenFd, std::string pwd, std::vector<Room*> *rooms) : _listenFd(listenFd), _unlocked(false), _connected(false), _nbRooms(0), _maxNbRooms(10),_response(""), _pwd(pwd), _nick(""), _username(""), _rooms(rooms)
{
	std::cout << "new client has been created" << std::endl;
	cmdsMap["PING"] = &ClientSocket::ping;
	cmdsMap["MODE"] = &ClientSocket::mode;
	cmdsMap["USER"] = &ClientSocket::user;
	cmdsMap["JOIN"] = &ClientSocket::join;
	cmdsMap["PRIVMSG"] = &ClientSocket::privmsg;
	cmdsMap["PART"] = &ClientSocket::part;
	cmdsMap["KICK"] = &ClientSocket::kick;
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
		"PING", "MODE", "USER", "JOIN", "PRIVMSG", "PART", "KICK"/*, "QUIT", "PASS", "NOTICE",
		"TOPIC", "NAMES", "LIST", "WHO", "WHOIS", "WHOWAS", "NICK", "INVITE",
		"OPER", "DIE", "RESTARD", "KILL", "SQUIT", "CONNECT" */ };

	for (int i = 0; i < 7; i++)
	{
		if (cmd == commands[i])
			return 1;
	}
	return (0);
}

std::vector<std::string> ClientSocket::split(std::string str)
{
	int							pos = 0;
	size_t 						i = 0;
	std::vector<std::string>	requests;

	for(i = 0; i < str.size(); i++)
	{
		if (str[i] == ' ')
		{
			requests.push_back(str.substr(pos, i - pos));
			pos = i + 1;
		}
	}
	if (!str[i])
		requests.push_back(str.substr(pos, i - pos));

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

void	ClientSocket::_interactMode(std::string modes, std::string modesArgs, Room *room)
{
	std::vector<std::string>	argsVec;
	std::string					modesChange;
	std::string					argsModesChange;
	std::string					lastArg;
	char						op;

	if(!modesArgs.empty())
		argsVec = split(modesArgs);
	for (size_t i = 0; i < modes.size(); i++)
	{
		if(i == 0 && modes[i] != '-' && modes[i] != '+')
			return(addResponse(":monserv 472 " + _nick + " " + modes[i] + " :is unknown mode char to me"));
		else if(modes[i] == '+' || modes[i] == '-')
		{
			op = modes[i];
		}
		else if(((modes[i] == 'k' || modes[i] == 'o' || modes[i] == 'l') && op == '+') || (modes[i] == 'o' && op == '-'))
		{
			if(argsVec.empty())
			{
				room->sendModesChange(modesChange, modesArgs, this);
				addResponse(":monserv 472 " + _nick + " " + modes[0] + " :is unknown mode char to me");
				return;
			}
			else if (op != '-' || modes[i] == 'o')
			{
				lastArg = getAndDel(argsVec);
				argsModesChange += lastArg;
				if(!argsVec.empty())
					argsModesChange += " ";
			}
		}
		if (modes[i] == 'k')
			room->k(lastArg, op);
		else if (modes[i] == 'i')
			room->i(op);
		else if (modes[i] == 't')
			room->t(op);
		else if (modes[i] == 'o')
			room->o(lastArg, op);
		else if (modes[i] == 'l')
			room->l(lastArg, op);
		modesChange += modes[i];
	}
	room->sendModesChange(modesChange, argsModesChange, this);
}

std::vector<std::string>	ClientSocket::_parsePartArgs(std::string args)
{
	std::vector<std::string>	roomsToPart;
	size_t i = 0;
	while (args.find(',', i) != std::string::npos && args.find(',', i) < args.find(' ', i))
	{
		roomsToPart.push_back(args.substr(i, args.find(',', i) - i));
		i = args.find(',', i) + 1;
	}
	if (args.find(' ', i) == std::string::npos)
	{
		roomsToPart.push_back(args.substr(i, args.size() - i + 1));
	}
	else
	{
		roomsToPart.push_back( args.substr(i, args.find(' ', i) - i));
		i = args.find(' ', i) + 1;
	}
	for (std::vector<std::string>::iterator roomToPartIt = roomsToPart.begin(); roomToPartIt != roomsToPart.end();)
	{
		std::string roomToPart = roomToPartIt->substr(1, roomToPartIt->size() - 1);
		if (!findOnVec(roomToPart, (*_rooms)))
		{
			addResponse(":monserv 403 " + _nick  + " #" + roomToPart + " :No such channel");
			roomToPartIt = roomsToPart.erase(roomToPartIt);
		}
		else if (!findOnVec(roomToPart, _roomsNames))
		{
			addResponse(":monserv 442 " + _nick  + " #" + roomToPart + " :You're not on that channel");
			roomToPartIt = roomsToPart.erase(roomToPartIt);
		}
		else
		{
			*roomToPartIt = roomToPart;
			++roomToPartIt;
		}
		for (std::vector<std::string>::iterator myRoomsIt = _roomsNames.begin(); myRoomsIt != _roomsNames.end();)
		{
			if (myRoomsIt->compare(roomToPart) == 0)
			{
				myRoomsIt = _roomsNames.erase(myRoomsIt);
			}
			else
				++myRoomsIt;
		}
	}
	return(roomsToPart);
}

int	ClientSocket::part(std::string args, Response &response)
{
	(void) response;
	std::vector<std::string>	roomNames = _parsePartArgs(args);
	std::string					message;
	size_t						find = args.find(':');
	if(find != std::string::npos)
		message = args.substr(find + 1, args.size() - find - 1);
	for (std::vector<std::string>::iterator it = roomNames.begin(); it != roomNames.end(); ++it)
	{
		Room	*room = findOnRoom(*it, *_rooms);
		room->part(this, message);
		_nbRooms--;
	}
	return(0);
}


int	ClientSocket::mode(std::string args, Response &response)
{
	(void)		response;
	Room		*room;
	std::string	roomName;
	std::string	modes;
	std::string	modesArgs;

	if (args.empty() == 1)
		return(addResponse(":monserv 461 " + _nick + " MODE :Not enough parameters"), 1);
	if (args.find(' ') == std::string::npos)
		roomName = args.substr(0, args.size());
	else
		roomName = args.substr(0, args.find(' '));
	if (args[0] != '#')
		return(addResponse(":monserv 476 " + _nick  + " " + roomName + " :Bad Channel Mask"), 1);
	roomName.erase(roomName.begin());
	if (!findOnVec(roomName, (*_rooms)))
		return(addResponse(":monserv 403 " + _nick  + " #" + roomName + " :No such channel"), 1);
	else if (!findOnVec(roomName, _roomsNames))
		return(addResponse(":monserv 442 " + _nick  + " #" + roomName + " :You're not on that channel"), 1);
	room = findOnRoom(roomName, *_rooms);
	if (args.find(' ') == std::string::npos)
		return(addResponse(":monserv 324 " + _nick  + " #" + roomName + " " + room->getModes()), 1);
	else
	{
		size_t space = args.find(' ');
		size_t space2 = args.find(' ', space + 1);
		modes = args.substr(space + 1, space2 - space - 1);
		if (args.size() > space2 + 1)
		modesArgs = args.substr(space2 + 1, args.size() - space2 + 1);
		if (room->isOp(_nick))
			_interactMode(modes, modesArgs, room);
		else
			return(addResponse(":monserv 482 " + _nick  + " #" + roomName + " :You're not channel operator"), 1);
	}
	return(0);
}

int	ClientSocket::user(std::string args, Response &response)
{
	(void)response;
	std::vector<std::string> argsVec = split(args);
	_username = argsVec[0];
	_hostname = argsVec[1];
	_servername = argsVec[2]; //verif invalid  read
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

void	ClientSocket::addRoom(std::string roomName)
{
	_roomsNames.push_back(roomName);
	_nbRooms++;
}

std::vector< std::pair<std::string, std::string> >	ClientSocket::_parseJoinArgs(std::string args)
{
	size_t i = 0;
	std::vector< std::pair<std::string, std::string> > argsVec;
	while (args.find(',', i) != std::string::npos && args.find(',', i) < args.find(' ', i))
	{
		std::pair<std::string, std::string> pair;
		pair.first = args.substr(i, args.find(',', i) - i);
		argsVec.push_back(pair);
		i = args.find(',', i) + 1;
	}

	std::pair<std::string, std::string> pair;

	if (args.find(' ', i) == std::string::npos)
	{
		pair.first = args.substr(i, args.size() - i + 1);
		argsVec.push_back(pair);
		for(size_t l = 0; l < argsVec.size(); l++)
			argsVec[l].second = 'x';
		return(argsVec);
	}
	else
	{
		pair.first = args.substr(i, args.find(' ', i) - i);
		argsVec.push_back(pair);
		i = args.find(' ', i) + 1;
	}
	size_t j = 0;
	while(args.find(',', i) != std::string::npos)
	{
		argsVec[j].second = args.substr(i, args.find(',', i) - i);
		j++;
		i = args.find(',', i) + 1;
	}
	argsVec[j].second = args.substr(i, args.size() - i);
	return (argsVec);
}

int	ClientSocket::join(std::string args, Response &response)
{
	(void) response;
	bool	isCreated = false;
	std::string roomName;
	std::vector< std::pair<std::string, std::string> > argsVec;
	argsVec = _parseJoinArgs(args);
	for(size_t i = 0; i < argsVec.size(); i++)
	{
		if (argsVec[i].first.empty())
		{
			addResponse(":monserv 461 " + _nick + " JOIN " + ":Not enough parameters");
			continue;
		}
		roomName = argsVec[i].first.substr(1, argsVec[i].first.size() - 1);
		if(argsVec[i].first[0] != '#')
		{
			addResponse(":monserv 403 " + _nick + " " + argsVec[i].first + " :No such chanel");
			continue;
		}
		else if (_nbRooms == _maxNbRooms)
		{
			addResponse(":monserv 405 " + _nick + " #" + roomName + " :You have joined too many channels\r\n");
			continue;
		}
		for (size_t j = 0; j < (*_rooms).size(); j++)
		{
			if ((*_rooms)[j]->getName() == roomName)
			{
				(*_rooms)[j]->join(this, argsVec[i].second);
				isCreated = true;
			}
		}
		if (!isCreated)
		{(
			*_rooms).push_back(new Room(roomName, this));
			addResponse(":monserv MODE #" + roomName + " +o " + _nick);
		}
	}
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

int	ClientSocket::kick_user_check(std::string user, Room *salon)
{
	if (user == _nick)
		return (addResponse(":monserv 444: " + salon->getName() + " " + _nick + " You can't kick yourself"), 1);
	if (salon->isOp(user))
		return (addResponse(":monserv 445: " + salon->getName() + " " + user + " Can't kick an operator"), 1);
	if (!salon->is_in_room(user))
		return (addResponse(":monserv 446: " + salon->getName() + " " + user + " :is not on channel"), 1);
	return (0);
}

std::vector<std::string> ClientSocket::kick_split(std::string str)
{
	size_t							pos = 0;
	size_t 						i = 0;
	std::vector<std::string>	requests;

	for(i = 0; i < str.size(); i++)
	{
		if (str[i] == ',')
		{
			requests.push_back(str.substr(pos, i - pos));
			pos = i + 1;
			i++;
		}
	}
	if (pos < str.size())
		requests.push_back(str.substr(pos, i - pos));

	// std::cout << "request = " << std::endl;

	// for (size_t j = 0; j < requests.size(); j++)
	// 	std::cout << "[" << requests[j] << "] ";

	// std::cout << std::endl;

	return (requests);
}
int	ClientSocket::kick_user(std::string user, std::string comment, Room *salon)
{
	(void)user;
	(void)comment;
	(void)salon;
	ClientSocket *client = salon->user_on_room(user);

/* 	std::string response;

	if (comment.empty())
	{
		response = this->getnick() + " KICK " + "#" + salon->getName() + " " + client->getnick();
	}
	else
	{
		response = this->getnick() + " KICK " + "#" + salon->getName() + " " + client->getnick()
		+ ":" + comment;
	} */
	salon->kick(this, client, comment);
	//deconnect le client de la room

	salon->Kickmsg(comment, this, client); //envoie le message a la room
	//send msgs a la room avec le commentaire

	return (0);
}

int	ClientSocket::kick(std::string args, Response &response)
{
	(void)response;
	int	i = -1;
	int	b = 0;
	std::string	comment;
	std::vector<std::string> args_tab;
	args_tab = split(args);
	std::cout << "Commande KICK" << std::endl;
	std::cout << " a " << std::endl;
	if (args_tab.size() > 3) //verif
		return (addResponse("monserv 463 " + _nick + "KICK :too mutch parameters"), 1);
	for (std::vector<std::string>::iterator it = args_tab.begin(); it != args_tab.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
	std::cout << " b " << std::endl;
	if (args_tab[0][0] != '#')
		return (addResponse(":monserv 461 " + _nick + "KICK :Not enough parameters"), 1);
	for (size_t j = 0; j < (*_rooms).size(); j++)
	{
		if ((*_rooms)[j]->getName() == &args_tab[0][1])
		{
			i = j;
			break;
		}
	}
	std::cout << " c " << std::endl;
	if (i == -1)
		return (addResponse(":monserv 403 " + _nick + " " + args_tab[0] + " :No such channel"), 1);
	for (std::vector<std::string>::iterator it = _roomsNames.begin(); it != _roomsNames.end(); ++it)
	{
		if (*it == &args_tab[0][1]) // sans le #
			b = 1;
	}
	if (b == 0)
		return (addResponse(":monserv 442 " + _nick + " " + args_tab[0] + " :You're not on that channel"), 1);
	if (!(*_rooms)[i]->isOp(_nick))
		return (addResponse(":monserv 482 " + _nick + " " + args_tab[0] + " :You're not channel operator"), 1);
	comment = args_tab[2];
	if (args_tab.size() == 3)
	{
		std::cout << "JUMPING THAT" << std::endl;
		std::cout << args_tab.size() << std::endl;
		args_tab = kick_split(args_tab[1]);

		std::cout << "tab size : " << args_tab.size() << "   <<--  if no nbr it s wrong" << std::endl;

		for (std::vector<std::string>::iterator it = args_tab.begin(); it < args_tab.end(); ++it)
		{
			std::cout << "args_tab_string = " << *it << std::endl;
		}

		for (size_t w = 0; w < args_tab.size(); w++)
		{
			std::cout << w << " that is it" << std::endl;
			if (kick_user_check(args_tab[w], (*_rooms)[i]) == 1)
				continue;
			kick_user(args_tab[w], comment, (*_rooms)[i]);
		}
	}
	return (0);
}
