#include "Response.hpp"

void	sendResponse(Client *client, ResponseCode code, const std::string &params)
{
	std::string	response = composeResponse(code, client->getNickname(), params);

	sendMessage(client->getSocketFd(), response);
}

std::string	composeResponse(ResponseCode code, const std::string &targetName, const std::string &params)
{
	std::ostringstream	oss;
	oss << ":" << SERVER_NAME << " " << code << " " << (targetName.empty() ? "*" : targetName);

	switch (code)
	{
		case RPL_WELCOME:
			oss << " :Welcome to the IRC Network";
			break;

		case ERR_SRVFULL:
			oss << " :Server is full";
			break;

		case ERR_UNKNOWNCOMMAND:
			oss << " " << params << " :Unknown command";
			break;

		case ERR_PASSWDMISMATCH:
			oss << " :Password incorrect";
			break;

		// TO DO create a usage for each command?
		case ERR_NEEDMOREPARAMS:
			oss << " " << params << " :Wrong number of parameters";
			break;

		case ERR_ALREADYREGISTERED:
			oss << " :You are already registered";
			break;
		
		case ERR_NICKNAMEINUSE:
			oss << " " << params << " :Nickname is already in use";
			break;

		case ERR_ERRONEUSNICKNAME:
			oss << " " << params << " :Erroneous nickname";
			break;

		case ERR_NOSUCHCHANNEL:
			oss << " " << params << " :No such channel";
			break;

		case ERR_INVITEONLYCHAN:
			oss << " " << params << " :Cannot join channel (+i)";
			break;

		case ERR_BADCHANNELKEY:
			oss << " " << params << " :Password mismatch";
			break;

		default:
			oss << " :Unknown response code";
			break;
	}

	oss << "\r\n";

	return (oss.str());
}
