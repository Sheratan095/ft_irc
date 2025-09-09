#ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <sys/socket.h>

typedef int	SocketFd; // Define SocketFd as an alias for int, representing a socket file descriptor


class Bot
{
public:
	Bot(const std::string &server_ip, int server_port, int bot_port);
	void run();

private:
	std::string _server_ip;
	int _server_port;
	int _bot_port;
};

//----------------- Bot Utilities ----------------//

std::vector<std::string>	split(const std::string& str, const std::string& delimiter);

int							safeAtoi(const std::string &s);

std::string					intToString(int n);

std::string					join(const std::vector<std::string> &elements, const std::string &separator);

std::string					join(const std::set<std::string> &elements, const std::string &separator);

bool						startsWith(const std::string &str, const std::string &prefix);

std::string					toLower(const std::string &s);

void						sendMessage(SocketFd socketFfd, const std::string &message);

bool						insentiveStringCompare(const std::string &str1, const std::string &str2);

#endif // BOT_HPP