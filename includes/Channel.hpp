#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Irc.hpp"

class	Channel
{
	private:
		std::string	_name;
		std::string	_topic;

	public:
		Channel(const std::string &name, const std::string &topic = "");
		~Channel();

		const std::string	&getName() const;
		const std::string	&getTopic() const;

		void	setTopic(const std::string &topic);
};

#endif