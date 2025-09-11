#include "RequestClass.hpp"

Request::Request()
{
	std::memset(_buffer, 0, 4096);
}

Request::~Request()
{
}

void	Request:: receive(int fd)
{
	size_t	next = 0;

	recv(fd, &_buffer, 4096, 0); // check value
	_str = _buffer;
	next = _str.find(' ');

	_cmd = _str.substr(0, next);
	_args = _str.substr(next + 1, _str.size() - next - 1);
	std::cout << "Buffer :";
	std::cout << _buffer << std::endl;
}

/* char buffer[1024];
int bytesReceived = recv(client_fd, buffer, sizeof(buffer), 0);

if (bytesReceived > 0) {
    buffer[bytesReceived] = '\0';  // Null-terminate to make it a valid string
    printf("Received: %s\n", buffer);
} else if (bytesReceived == 0) {
    printf("Client disconnected\n");
} else {
    perror("recv failed");
*/

void Request::show()
{
	std::cout << "----REQUEST----\n"
	<< "CMD : " << _cmd
	<< "\nARGS : " << _args
	<< std::endl;
}

