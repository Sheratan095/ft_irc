#include "Server.hpp"

void	Server::modeCmd(Client *client, const IRCMessage &message)
{
	if (client->isRegistered() == false)
	{
		sendResponse(client, ERR_NOTREGISTERED, "MODE");
		return;
	}

	if (message.parameters.size() < 1)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "MODE");
		return;
	}

	const std::string	&channelName = toLower(message.parameters[0]);

	std::map<std::string, Channel*>::iterator	it = _channels.find(channelName);
	Channel	*targetChannel;

	if (it == _channels.end())
	{
		sendResponse(client, ERR_NOSUCHCHANNEL, channelName);
		return;
	}

	targetChannel = it->second;

	// check that the client is connected to the channel
	if (targetChannel->isClientInChannel(client->getSocketFd()) == false)
	{
		sendResponse(client, ERR_NOTONCHANNEL, channelName);
		return;
	}

	if (message.parameters.size() == 1)
	{
		sendResponse(client, RPL_CHANNELMODEIS, channelName + " " + targetChannel->getMode());
		return;
	}

	// Only channel operators may set modes
	if (!targetChannel->isClientOperator(client->getSocketFd()))
	{
		sendResponse(client, ERR_CHANOPRIVSNEEDED, channelName);
		return;
	}

	parseMode(client, targetChannel, message);
}

// /MODE     <channel> {[+|-]<modes>} {<mode params>}
//	command  params[0] params[1]      params[2], params[3], ...
void	Server::parseMode(Client *client, Channel *channel, const IRCMessage &message) const
{
	std::string						appliedModes; // To keep track of applied modes
	std::vector<std::string>		appliedParams; // To keep track of used parameters
	const std::string				&modeStr = message.parameters[1]; // e.g., +itk
	const std::vector<std::string>	&params = message.parameters; // just an alias

	bool	adding = true; // true if adding modes, false if removing, APPLIES UNTIL OVERRIDDEN

	size_t	paramIndex = 2; // Start from the first mode parameter

	for (size_t i = 0; i < modeStr.size(); ++i)
	{
		char	modeChar = modeStr[i];

		if (modeChar == '+')
		{
			adding = true;
		}
		else if (modeChar == '-')
		{
			adding = false;
		}
		else
		{
			switch (modeChar)
			{
				case 'i': // Invite-only
				{
					if (adding)
						channel->setInviteOnly();
					else
						channel->removeInviteOnly();
					appliedModes += (adding ? "+" : "-");
					appliedModes += 'i';
					break;
				}

				case 't': // Topic restriction
				{
					if (adding)
						channel->setTopicRestriction();
					else
						channel->removeTopicRestriction();
					appliedModes += (adding ? "+" : "-");
					appliedModes += 't';
					break;
				}

				case 'k': // Password protection
				{
					if (adding)
					{
						// The parameter is required ONLY when adding the password
						if (paramIndex >= params.size())
						{
							sendResponse(client, ERR_NEEDMOREPARAMS, "MODE " + channel->getName());
							continue;
						}
						appliedParams.push_back(params[paramIndex]);
						channel->setPassword(params[paramIndex++]);
						appliedModes += "+k";
					}
					else
					{
						channel->removePassword();
						appliedModes += "-k";
					}
					break;
				}

				case 'l': // User limit
				{
					if (adding)
					{
						// The parameter is required ONLY when adding the limit
						if (paramIndex >= params.size())
						{
							sendResponse(client, ERR_NEEDMOREPARAMS, "MODE " + channel->getName());
							continue;
						}

						int	limit = safeAtoi(params[paramIndex++]);
						if (limit < 0)
						{
							sendResponse(client, ERR_NEEDMOREPARAMS, "MODE " + channel->getName());
							continue;
						}
						appliedParams.push_back(params[paramIndex - 1]);
						appliedModes += "+l";
						channel->setUserLimit(limit);
					}
					else
					{
						appliedModes += "-l";
						channel->removeUserLimit();
					}
					break;
				}

				case 'o': // Operator status
				{
					if (paramIndex >= params.size())
					{
						sendResponse(client, ERR_NEEDMOREPARAMS, "MODE " + channel->getName());
						continue;
					}

					const std::string	&targetNick = params[paramIndex++];
					Client				*targetClient = findClientByName(targetNick);

					if (!targetClient)
					{
						sendResponse(client, ERR_NOSUCHNICK, targetNick);
						continue;
					}
					if (!channel->isClientInChannel(targetClient->getSocketFd()))
					{
						sendResponse(client, ERR_USERNOTINCHANNEL, targetNick + " " + channel->getName());
						continue;
					}
					if (adding)
					{
						appliedParams.push_back(targetNick);
						appliedModes += "+o";
						channel->addOperator(targetClient->getSocketFd());
					}
					else
					{
						appliedParams.push_back(targetNick);
						appliedModes += "-o";
						channel->removeOperator(targetClient->getSocketFd());
					}
					break;
				}

				default :
					sendResponse(client, ERR_UNKNOWNMODE, std::string(1, modeChar) + " " + channel->getName());
					continue;
			}
		}
	}

	if (!appliedModes.empty())
		channel->notifyMode(client, appliedModes, appliedParams);
}
