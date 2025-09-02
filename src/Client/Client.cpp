#include "Client.hpp"

Client::Client(SocketFd socket_fd, const std::string &ipAddress): _socketFd(socket_fd), _ipAddress(ipAddress)
{
	_username = "";
	_nickname = "";
	_realname = "";
	_isAuthenticated = false;
}

Client::~Client()
{
	close(_socketFd);
}

//--------------------------------------------------------------------------------

bool	Client::isRegistered() const
{
	if (_username.empty() || _nickname.empty() || _realname.empty() || !_isAuthenticated)
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

std::string	Client::getRealName() const
{
	return (_realname);
}

void	Client::setRealName(const std::string& realname)
{
	_realname = realname;
}

std::string	Client::getUsername() const
{
	return (_username);
}

std::string Client::getWho() const
{
	std::ostringstream	oss;

	oss << "* "
		<< getUsername() << " "
		<< getIpAddress() << " "
		<< SERVER_NAME << " "
		<< getNickname() << " "
		<< "H"
		<< " :0 "
		<< getRealName();

	return (oss.str());
}

void	Client::sendPrivMessage(Client *sender, const std::string &message) const
{
	std::stringstream	ss;

	ss << ":" << sender->getPrefix()
	<< " PRIVMSG " << this->getNickname() // Target nick
	<< " :" << message << "\r\n";

	sendMessage(this->getSocketFd(), ss.str());
}
