#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <csignal>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <fcntl.h> 
#include <list>
#include <vector>
#include <sstream>
#include <poll.h>
#include <algorithm> // For std::find
#include "Utils.hpp"
#include "Response.hpp"

#define SERVER_NAME "ircServer"
#define MAX_NICKNAME_LENGTH 9 // Maximum length of a nickname

#define	DEFAULT_USER_LIMIT_IN_CHANNEL 128 

#define	MAX_CLIENTS 2

typedef int	SocketFd; // Define SocketFd as an alias for int, representing a socket file descriptor


class	Channel;
class	Client;
class	Server;

struct	IRCMessage
{
	std::string					prefix;      // Optional, starts with ':'
	std::string					command;
	std::vector<std::string>	parameters;  // Command parameters
};

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

char	*readMessageFromClient(int client_fd);

void	sendMessage(SocketFd socketFfd, const std::string &message);

#endif