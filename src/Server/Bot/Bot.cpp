#include "Bot.hpp"

Bot::Bot(const std::string &name) : _name(name)
{}

Bot::~Bot()
{}

std::string Bot::getName() const
{
	return (_name);
}

void Bot::reciveMessage(Client *client, const std::string &message) const
{
	(void)client;
	(void)message;

	// Process the received message
}