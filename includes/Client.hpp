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
		bool					_capHandshaking; // if the client requests CAP, the welcome message is delayed until CAP END
		bool					_isAuthenticated;

		std::string				_ipAddress;

		bool					_imBot; // if the client is a bot for instant messaging

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

		std::string	getWho() const;

		void	setUsername(const std::string& username);
		void	setNickname(const std::string& nickname);
		void	setAuthenticated();
		void	setRealName(const std::string& realname);
		void	setCapHandshaking(bool status);
		void	setImBot(bool status);


		void	sendPrivMessage(Client *sender, const std::string &message) const;
};

#endif
