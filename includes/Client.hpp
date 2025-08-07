#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Irc.hpp"

class	Client
{
	private:
		int			_socketFd;

		std::string				_username;
		std::string				_nickname;
		bool					_isAuthenticated;
		std::list<Channel *>	_joinedChannels;

	public:
		Client(int socket_fd);
		~Client();

		bool		isRegistered() const;
		int			getSocketFd() const;
		std::string	getNickname() const;
		void		joinChannel(Channel *channel);

		void	setUsername(const std::string& username);
		void	setNickname(const std::string& nickname);
		void	setAuthenticated();


};

#endif
