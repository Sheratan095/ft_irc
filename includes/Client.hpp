#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Irc.hpp"

class	Client
{
	private:
		SocketFd				_socketFd;

		std::string				_username;
		std::string				_nickname;
		std::string				_realname;
		bool					_isAuthenticated;

		std::string				_ipAddress;

	public:
		Client(SocketFd socket_fd, const std::string &ipAddress);
		~Client();

		bool		isRegistered() const;
		int			getSocketFd() const;
		std::string	getNickname() const;
		std::string	getIpAddress() const;
		std::string	getPrefix() const;
		std::string	getRealName() const;
		std::string	getUsername() const;

		void	setUsername(const std::string& username);
		void	setNickname(const std::string& nickname);
		void	setAuthenticated();
		void	setRealName(const std::string& realname);

};

#endif
