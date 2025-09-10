#include "Bot.hpp"

bool	Bot::createSocket()
{
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd == -1)
	{
		std::cerr << "Error creating socket" << std::endl;
		return false;
	}
	return true;
}

void	Bot::connectToServer(const std::string &password)
{
	struct sockaddr_in serverAddr = {};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(_server_port);
	inet_pton(AF_INET, _server_ip.c_str(), &serverAddr.sin_addr);

	if (connect(_socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
	{
		std::cerr << "Error connecting to server at " << _server_ip << ":" << _server_port << std::endl;
		close(_socketFd);
		return;
	}

	std::cout << "Connected to IRC server at " << _server_ip << ":" << _server_port << std::endl;

	// IRC handshake
	if (!password.empty())
		sendMessage(_socketFd, "PASS " + password + "\r\n");
	sendMessage(_socketFd, "NICK " + _nick + "\r\n");
	sendMessage(_socketFd, "USER " + _nick + " 0 * :" + _nick + "\r\n");
	sendMessage(_socketFd, "IMBOT\r\n");

	// main loop
	run();
}