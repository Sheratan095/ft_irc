#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Irc.hpp"

class	Client
{
	private:
		int			_socket_fd;

	public:
		Client(int socket_fd);
		~Client();

};

#endif
