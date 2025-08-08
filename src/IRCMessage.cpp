#include "IRCMessage.hpp"

IRCMessage::IRCMessage(SocketFd senderSocketFd, const std::string &line): _senderSocketFd(senderSocketFd)
{
	std::string	input = line;
	size_t		pos = 0;

	if (line.empty())
		throw (InvalidIRCMessageException());

	// parse optional prefix
	if (input[0] == ':')
	{
		pos = input.find(' ');
		if (pos == std::string::npos)
			throw (InvalidIRCMessageException());
		prefix = input.substr(1, pos - 1);
		input.erase(0, pos + 1);
	}

	// parse command
	pos = input.find(' ');
	if (pos == std::string::npos)
	{
		command = input;
		return;
	}
	command = input.substr(0, pos);
	input.erase(0, pos + 1);

	// parse parameters and trailing
	while (!input.empty())
	{
		if (input[0] == ':') {
			trailing = input.substr(1);
			break;
		}
		pos = input.find(' ');
		if (pos == std::string::npos) {
			parameters.push_back(input);
			break;
		}
		parameters.push_back(input.substr(0, pos));
		input.erase(0, pos + 1);
	}
}

IRCMessage::~IRCMessage()
{}
