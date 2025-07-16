#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Irc.hpp"

class	Client
{
	private:
		int			_socketFd;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_hostname;
		std::string	_password;
		bool		_isRegistered;

	public:
		Client(int socket_fd);
		~Client();

};

#endif
