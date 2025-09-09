#include "Server.hpp"

void Server::capCmd(Client *client, const IRCMessage &message)
{
	client->setCapHandshaking(true);

	const std::string	serverName = SERVER_NAME;

	//multi-prefix - Shows multiple user prefixes (@ for ops, + for voice, etc.)
	//extended-join - Enhanced JOIN messages with account information
	//userhost-in-names - Include full user@host in NAMES replies
	std::vector<std::string>	supported;
	supported.push_back("extended-join");
	supported.push_back("multi-prefix");
	supported.push_back("userhost-in-names");

	if (message.parameters.empty())
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "CAP");
		return; // Malformed CAP command
	}

	const std::string &subCmd = message.parameters[0];

	if (subCmd == "LS")
	{
		// Client may send "LS" or "LS 302"

		// manually join the set
		std::string	capabilities = join(supported, " ");

		sendMessage(client->getSocketFd(), ":" + serverName + " CAP * LS :" + capabilities + "\r\n");
	}
	else if (subCmd == "REQ")
	{
		// Client requests one or more capabilities
		std::string	requested;

		// Extract requested capabilities from parameters or trailing
		if (!message.trailing.empty()) // CAP REQ :multi-prefix
			requested = message.trailing;
		else if (message.parameters.size() >= 2) // CAP REQ multi-prefix
			requested = message.parameters[1];

		// Split requested capabilities
		std::vector<std::string>	reqCaps = split(requested, " ");
		std::vector<std::string>	acked, nacked;
		size_t	supportedCount = supported.size();

		for (size_t i = 0; i < reqCaps.size(); ++i)
		{
			bool	found = false;
			for (size_t j = 0; j < supportedCount; ++j)
			{
				if (reqCaps[i] == supported[j])
				{
					found = true;
					break;
				}
			}
			if (found)
				acked.push_back(reqCaps[i]);
			else
				nacked.push_back(reqCaps[i]);
		}

		if (!acked.empty())
			sendMessage(client->getSocketFd(), ":" + serverName + " CAP * ACK :" + join(acked, " ") + "\r\n");
		if (!nacked.empty())
			sendMessage(client->getSocketFd(), ":" + serverName + " CAP * NAK :" + join(nacked, " ") + "\r\n");
	}
	else if (subCmd == "END")
	{
		// Acknowledge the end of capability negotiation DO NOT NEED TO SEND ACK
		// sendMessage(client->getSocketFd(), ":" + serverName + " CAP * ACK");
		client->setCapHandshaking(false);
	}
	else
	{
		// Unknown subcommand → NAK
		sendMessage(client->getSocketFd(), ":" + serverName + " CAP * NAK :" + subCmd + "\r\n");
	}

	if (client->isRegistered())
		sendWelcomeMessages(client);
}
