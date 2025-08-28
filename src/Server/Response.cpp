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

		case ERR_UNKNOWNCOMMAND:
			oss << " " << params << " :Unknown command";
			break;

		case ERR_PASSWDMISMATCH:
			oss << " :Password incorrect";
			break;

		case ERR_NEEDMOREPARAMS:
			oss << " " << params << " :Wrong number of parameters";
			break;

		case ERR_ALREADYREGISTERED:
			oss << " :You are already registered";
			break;

		case ERR_NOTREGISTERED:
			oss << " " << params << " :You have not registered";
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

		case RPL_NOTOPIC:
			// params = "<channel>"
			oss << " " << params << " :No topic is set";
			break;

		case RPL_TOPIC:
			// params = "<channel> :<topic text>"
			oss << " " << params;
			break;

		case RPL_WHOREPLY:
			// params = "<channel> :<names>"
			 oss << " " << params;
			break;

		case RPL_ENDOFWHO:
			// params = "<channel>"
			oss << " " << params << " :End of /WHO list";
			break;

		case RPL_CHANNELMODEIS:
			// params = "<channel> <modes>"
			oss << " " << params;
			break;

		case RPL_NAMREPLY:
			// params = "<channel> :<names>"
			oss << " " << params;
			break;

		case RPL_ENDOFNAMES:
			// params = "<channel>"
			oss << " " << params << " :End of NAMES list";
			break;

		case ERR_NOSUCHNICK:
			oss << " " << params << " :No such nick";
			break;

		case ERR_USERONCHANNEL:
			oss << " " << params << " :User is already on channel";
			break;

		case ERR_NOTONCHANNEL:
			oss << " " << params << " :You are not on that channel";
			break;

		case ERR_CHANOPRIVSNEEDED:
			oss << " " << params << " :You need to be a channel operator to perform this action";
			break;

		// params = "<nick> <channel>"
		case RPL_INVITE:
			oss << " " << params;
			break;

		default:
			oss << " :Unknown response code";
			break;
	}

	oss << "\r\n";

	return (oss.str());
}

// Special function because this message is sent when the Client class isn't still created
void	sendSrvFULLError(int client_fd)
{
	std::ostringstream	oss;

	// 501 ERR_SRVFULL
	oss << ":" << SERVER_NAME << " 501 * :Server is full\r\n";;

	sendMessage(client_fd, oss.str());
}
