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
#include <list>
#include <vector>
#include <poll.h>
#include <algorithm> // For std::find
#include "Utils.hpp"
#include "ReponseCode.hpp"

#define	DEFAULT_USER_LIMIT_IN_CHANNEL 128 

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

#endif