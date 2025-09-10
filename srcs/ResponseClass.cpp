#include "ResponseClass.hpp"

Response::Response(Request &request) : _request(request)
{

}

void Response::_makeResponse()
{
	_response = "ok";
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
