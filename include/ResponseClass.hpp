#ifndef RESPONSE_CLASS_HPP
#define RESPONSE_CLASS_HPP

#include <fstream>
#include <sstream>
#include "RequestClass.hpp"
#include "poll.h"


class ClientSocket;

class Response {

	public :
		Response(Request &request);
		~Response();

		std::string str();
		size_t size();
		void makeResponse(bool unlocked, bool connected, std::string user, std::string cmd, std::string args);
		int addResponse(std::string str);
		void clear();

	private :
		Response();
		Request _request;
		std::string _response;

};

#endif
