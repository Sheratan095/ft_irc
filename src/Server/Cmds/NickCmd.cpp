#include "Server.hpp"

static bool isValidNickname(const std::string &nickname);

// NICK RULES:
// Can contains:
// - letters (a-z, A-Z)
// - digits (0-9)
// - special characters: '_'
// must start with a letter
// must not be longer than MAX_NICKNAME_LENGTH
void	Server::nickCmd(Client *client, const IRCMessage &message)
{
	// used to send welcome or notify other users about the new nickname
	bool	firstConnection = (client->getNickname().empty());

	if (message.parameters.size() != 1)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "NICK");
		return;
	}

	const std::string	&new_nickname = message.parameters[0];

	if (isValidNickname(new_nickname) == false)
	{
		sendResponse(client, ERR_ERRONEUSNICKNAME, new_nickname);
		return;
	}

	if (client->getNickname() == new_nickname)
		return; // no change

	if (findClientByName(new_nickname) != NULL)
	{
		sendResponse(client, ERR_NICKNAMEINUSE, new_nickname);
		return;
	}

	std::string	oldNickname = client->getNickname();
	client->setNickname(new_nickname);

	// if the client was not registered before the message
	if (firstConnection)
	{
		// if now the registration is complete
		if (client->isRegistered())
		{
			client->setAuthenticated();
			sendWelcomeMessages(client);
		}
	}
	else
	{
		notifyNickChange(client, oldNickname);
	}

}

static bool isValidNickname(const std::string &nickname)
{
	if (nickname.empty() || nickname.length() > MAX_NICKNAME_LENGTH)
		return (false);

	if (!std::isalpha(nickname[0])) // First character must be a letter
		return (false);

	// start from the second char because the first one is already checked
	for (size_t i = 1; i < nickname.length(); ++i)
	{
		char	c = nickname[i];
		if (std::isdigit(c) || std::isalpha(c) || c == '_')
			continue;
		
		// If character is not allowed, return false
		return (false);
	}

	return (true);
}

