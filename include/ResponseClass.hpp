#ifndef RESPONSE_CLASS_HPP
#define RESPONSE_CLASS_HPP

#include <fstream>
#include <sstream>
#include "RequestClass.hpp"

class Response {

	public :
		Response(Request &request);
		~Response();

		std::string str();
		size_t size();
		void makeResponse(bool unlocked, bool connected, std::string user, std::string cmd, std::string args);


	private :
		Response();
		std::string _response;
		Request _request;

};

#endif
