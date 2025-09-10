#include <ASocketClass.hpp>

ASocket::ASocket() : _connected(false), _fd(-1)
{
	std::memset(&_addr, 0, sizeof(_addr));
}

ASocket::~ASocket()
{
	close(_fd);
}

bool ASocket::isConnected()
{
	return (_connected);
}
