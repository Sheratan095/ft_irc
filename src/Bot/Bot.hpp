#ifndef BOT_HPP
#define BOT_HPP

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

extern bool BOT_RUNNING;

typedef int	SocketFd; // Define SocketFd as an alias for int, representing a socket file descriptor

struct	IRCMessage
{
	std::string					prefix;      // Optional, starts with ':'
	std::string					command;
	std::vector<std::string>	parameters;  // Command parameters
	std::string					trailing;    // Trailing parameter, if any
};


class Bot
{
	private:
		std::string	_server_ip;
		int			_server_port;
		SocketFd	_socketFd;

		std::string	_nick;

		std::string	_ip;

		std::vector<std::string>	_responses;


	public:
		Bot(const std::string &server_ip, int server_port);

		bool	createSocket();
		void	run();
		void	connectToServer(const std::string &password);

		std::vector<IRCMessage>	parseMessage(const std::string &message) const;

		void	privmsgCmd(const IRCMessage &message) const;


	//-------------------------EXCEPTIONS-------------------------

		class SocketException : public std::exception
		{
			virtual const char *what() const throw()
			{
				return "Error: socket creation failed";
			}
		};

		class BindException : public std::exception
		{
			virtual const char *what() const throw()
			{
				return "Error: bind failed";
			}
		};

		class ListenException : public std::exception
		{
			virtual const char *what() const throw()
			{
				return "Error: listen failed";
			}
		};


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

#endif // BOT_HPP