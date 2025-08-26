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
		Channel(const std::string &name, Client *creator);
		~Channel();

		const std::string					&getTopic() const;
		const std::string					&getName() const;
		const std::string					getUserList() const;
		const std::map<SocketFd, Client*>	&getMembers() const; // Added this line

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
		bool	isClientInvited(SocketFd client_fd) const;
		bool	isClientOperator(SocketFd client_fd) const;
		bool	isInviteOnly() const;
		bool	isChannelFull() const;
		bool	isPasswordProtected() const;
		bool	isPasswordCorrect(const std::string &password) const;

		void	broadcastMessage(const std::string &message) const;

		// return a vector of who's is connected to the channel
		std::vector<std::string>	getWho() const;
		std::string					getNames() const;
		std::string					getMode() const;
};

#endif