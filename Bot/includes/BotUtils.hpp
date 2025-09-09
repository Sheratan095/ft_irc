#ifndef BOTUTILS_HPP
#define BOTUTILS_HPP

#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <csignal>
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
#include <set>
#include <algorithm> // For std::find
#include <netdb.h> // For gethostbyname

typedef int	SocketFd; // Define SocketFd as an alias for int, representing a socket file descriptor

struct	IRCMessage
{
	std::string					prefix;      // Optional, starts with ':'
	std::string					command;
	std::vector<std::string>	parameters;  // Command parameters
	std::string					trailing;    // Trailing parameter, if any
};


//----------------- Bot Utilities ----------------//

void						handleSigInt(int sig);

std::vector<std::string>	split(const std::string& str, const std::string& delimiter);

int							safeAtoi(const std::string &s);

std::string					intToString(int n);

std::string					join(const std::vector<std::string> &elements, const std::string &separator);

std::string					join(const std::set<std::string> &elements, const std::string &separator);

bool						startsWith(const std::string &str, const std::string &prefix);

std::string					toLower(const std::string &s);

void						sendMessage(SocketFd socketFfd, const std::string &message);

bool						insentiveStringCompare(const std::string &str1, const std::string &str2);

void						replaceAll(std::string &str, const std::string &from, const std::string &to);

std::string					getNickByPrefix(const std::string &prefix);

void						printRawMessage(const std::vector<IRCMessage> &messages);


#endif