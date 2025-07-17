#include "Client.hpp"

Client::Client(int socket_fd): _socketFd(socket_fd)
{}

Client::~Client()
{}

//--------------------------------------------------------------------------------

bool	Client::isRegistered() const
{
	return (!_username.empty() && !_nickname.empty() && _isAuthenticated);
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