#ifndef RESPONSE_CLASS_HPP
#define RESPONSE_CLASS_HPP

#include <fstream>
#include <sstream>
#include "RequestClass.hpp"

class ClientSocket;

class Response {

	public :
		Response(Request &request);
		~Response();

		std::string str();
		size_t size();
		void makeResponse(bool unlocked, bool connected, std::string user);
		void	interactcmd(ClientSocket *client, std::string cmd, std::string args);
		static bool	isacmd(std::string cmd);
		std::string	ping(std::string args, std::string username);

	private :
		Response();
		std::string _response;
		Request _request;

};

#endif