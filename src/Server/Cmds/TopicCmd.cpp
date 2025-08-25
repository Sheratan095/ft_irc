#include "Server.hpp"

void	Server::topicCmd(Client *client, const IRCMessage &message)
{
	if (message.parameters.empty() || message.parameters.size() > 2)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "TOPIC");
		return;
	}

	const std::string	&channelName = message.parameters[0];
	std::map<std::string, Channel*>::iterator	channelIt = _channels.find(channelName);

	if (channelIt == _channels.end())
	{
		sendResponse(client, ERR_NOSUCHCHANNEL, channelName);
		return;
	}

	Channel	*channel = channelIt->second;

	if (message.trailing.empty())
	{
		std::string	topic = channel->getTopic();

		if (topic.empty())
			sendResponse(client, RPL_NOTOPIC, channelName);
		else
			sendResponse(client, RPL_TOPIC, channelName + " :" + topic);
	}
	else
	{
		// Set the new topic
		const std::string &newTopic = message.trailing;
		channel->setTopic(newTopic);

		sendResponse(client, RPL_TOPIC, channelName + " :" + newTopic);

		NotifyTopicChange(client, channel);
	}
}