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
		Server*						_server;     // Pointer to the server instance

	public:
		IRCMessage();
		IRCMessage(Client *client, ResponseCode code, const std::string &message);
		IRCMessage(SocketFd senderSocketFd, const std::string &line);
		IRCMessage(Server *server, ResponseCode code, const std::string &message);
		IRCMessage(SocketFd senderSocketFd, const std::string &prefix, const std::string &command, const std::vector<std::string> &parameters, const std::string &trailing = "");

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