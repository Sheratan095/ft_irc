#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Irc.hpp"

class	Channel
{
	private:
		std::string					_name;
		std::string					_topic;


		bool						_isInviteOnly; // (i)

		bool						_istopicRestrictedToOps; // (t) specifies if only channel operators can change the topic

		bool						_isPasswordProtected; // (k)
		std::string					_password; // (k) - optional password for private channels

		size_t						_userLimit; // (l) - maximum number of members allowed in the channel


		std::map<SocketFd, Client*>		_members; // Map of client file descriptors to Client objects
		std::list<SocketFd>				_operators; // List of operator file descriptors
		std::list<SocketFd>				_invited; // List of invited clients

	public:
		Channel(const std::string &name);
		~Channel();

		const std::string	&getTopic() const;

		void	setTopic(const std::string &topic);
		void	setInviteOnly(bool isInviteOnly);
		void	setTopicRestrictedToOps(bool istopicRestrictedToOps);
		void	setPasswordProtected(bool _isPasswordProtected, const std::string &password = "");
		void	setUserLimit(size_t _userLimit);

		bool	addClient(SocketFd client_fd, Client *client);
		bool	removeClient(SocketFd client_fd);
		bool	inviteClient(SocketFd client_fd);
		bool	addOperator(SocketFd client_fd);
		bool	removeOperator(SocketFd client_fd);

		bool	isClientInChannel(SocketFd client_fd) const;

		void	broadcastMessage(const std::string &message) const;
};

#endif