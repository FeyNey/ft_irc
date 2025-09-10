#include <ClientSocketClass.hpp>

ClientSocket::ClientSocket(int listenFd) : _listenFd(listenFd)
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

void ClientSocket::interact()
{
	Request		request;
	request.receive(_fd);
	request.show();
	Response	response(request);
	std::cout << response.str() << std::endl;
	send(_fd, response.str().c_str(), response.str().size(), 0);
}

ClientSocket::~ClientSocket()
{
	close(_fd);
}
