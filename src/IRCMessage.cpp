#include "IRCMessage.hpp"

static std::string	composeResponse(ResponseCode code, const std::string &targetName, const std::string &params);

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

IRCMessage::IRCMessage(Client *client, ResponseCode code, const std::string &message)
{
	_senderSocketFd = client->getSocketFd();
	prefix = client->getPrefix();
	command = message;
	parameters.push_back(client.getNickname());
	trailing = composeResponse(code, client.getNickname(), message);
	_server = client.getServer(); // Assuming Client has a method to get the server instance

	if (_server == nullptr)
		throw (InvalidIRCMessageException());
}

IRCMessage::IRCMessage(SocketFd senderSocketFd, const std::string &prefixStr, const std::string &cmdStr, const std::vector<std::string> &params, const std::string &trailingStr)
: _senderSocketFd(senderSocketFd), prefix(prefixStr), command(cmdStr), parameters(params), trailing(trailingStr)
{
	if (command.empty())
		throw (InvalidIRCMessageException());
}

IRCMessage::~IRCMessage()
{}

std::string	IRCMessage::toString() const
{
	std::ostringstream	oss;

	if (!prefix.empty())
		oss << ":" << prefix << " ";

	oss << command;

	for (std::vector<std::string>::const_iterator it = parameters.begin(); it != parameters.end(); ++it)
		oss << " " << *it;

	if (!trailing.empty())
		oss << " :" << trailing;

	return (oss.str());
}

static std::string	getResponseByCode(ResponseCode code)
{
	switch (code)
	{
		case RPL_WELCOME:
			return("Welcome to the IRC Network");

		case ERR_SRVFULL:
			return("Server is full");

		case ERR_UNKNOWNCOMMAND:
			return("Unknown command");

		case ERR_PASSWDMISMATCH:
			return("Password incorrect");

		// TO DO create a usage for each command?
		case ERR_NEEDMOREPARAMS:
			return("Wrong number of parameters");

		case ERR_ALREADYREGISTERED:
			return("You are already registered");
		
		case ERR_NICKNAMEINUSE:
			return("Nickname is already in use");

		case ERR_ERRONEUSNICKNAME:
			return("Erroneous nickname");

		default:
			return("Unknown response code");
	}

	return ("Unknown response code");
}