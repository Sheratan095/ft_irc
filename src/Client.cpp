#include "Client.hpp"

Client::Client(SocketFd socket_fd, const std::string &ipAddress): _socketFd(socket_fd), _ipAddress(ipAddress)
{}

Client::~Client()
{
	close(_socketFd);
}

//--------------------------------------------------------------------------------

bool	Client::isRegistered() const
{
	if (_username.empty() || _nickname.empty() || !_isAuthenticated)
		return (false);

	return (true);
}

void	Client::setUsername(const std::string& username)
{
	_username = username;
}

void	Client::setNickname(const std::string& nickname)
{
	_nickname = nickname;
}

void	Client::setAuthenticated()
{
	_isAuthenticated = true;
}

int	Client::getSocketFd() const
{
	return (_socketFd);
}

std::string	Client::getNickname() const
{
	return (_nickname);
}

std::string	Client::getIpAddress() const
{
	return (_ipAddress);
}

std::string	Client::getPrefix() const
{
	return (_nickname + "!" + _username + "@" + _ipAddress);
}
