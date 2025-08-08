#ifndef IRC_MESSAGE_HPP
#define IRC_MESSAGE_HPP

#include "Irc.hpp"

class	IRCMessage
{
	public:
		SocketFd					_senderSocketFd; // Socket file descriptor for the client	
		std::string					prefix;      // Optional, starts with ':'
		std::string					command;
		std::vector<std::string>	parameters;  // Command parameters
		std::string					trailing;    // Trailing parameter, if any

	public:
		IRCMessage();
		IRCMessage(SocketFd senderSocketFd, const std::string &line);

		~IRCMessage();

		std::string	toString() const;

	//-------------------------EXCEPTIONS-------------------------

	class InvalidIRCMessageException : public std::exception
	{
		virtual const char *what() const throw()
		{
			return "Error: invalid IRC message";
		}
	};
};

#endif