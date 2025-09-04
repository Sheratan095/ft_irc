#ifndef SERVER_HPP
#define SERVER_HPP

#include "Irc.hpp"

// Forward declarations
class	Channel;
class	Client;

extern bool SERVER_RUNNING;

class	Server
{
	private:
		const int						_port;
		std::string						_ip;
		const std::string				_password;
		SocketFd						_socketFd;

		std::map<SocketFd, Client*>		_clients;
		std::vector<pollfd>				_pollFds;

		// channels's name are normalized to lowercase
		std::map<std::string, Channel*>			_channels;
		//Needed to send QUIT and NICK also in private conversations and not just in the shared channels
		std::map<SocketFd, std::set<SocketFd> >	_privateConversations;


		bool	createSocket();
		bool	bindSocket();
		bool	startSocketListening();

		void	run();
		bool	checkPoll(int poolResult) const;

		void	handleConnectionRequest(struct sockaddr_in	client_addr, socklen_t client_len);
		void	handleDisconnection(int client_fd);
		void	handleRequest(int client_fd);

		bool	switchCommand(const IRCMessage &message, Client *client);


		std::string				readMessageFromClient(int client_fd) const;
		std::vector<IRCMessage>	parseMessage(const std::string &message) const;
		void					printRawMessage(const std::vector<IRCMessage> &messages) const;
		Client*					findClientByName(const std::string &nickname) const;


		bool	addClient(pollfd clientPollFd, const std::string &ip_str); // first connection
		void	removeClient(Client *client);

		// commands
		void	quitCmd(Client *client, const IRCMessage &message);
		void	quitCmd(Client *client, const std::string &reason); // used for like end task or in general when the client process isn't disconnected in normal way
		void	userCmd(Client *client, const IRCMessage &message);
		void	passCmd(Client *client, const IRCMessage &message);
		void	nickCmd(Client *client, const IRCMessage &message);
		void	joinCmd(Client *client, const IRCMessage &message);
		void	modeCmd(Client *client, const IRCMessage &message);
		void	whoCmd(Client *client, const IRCMessage &message);
		void	topicCmd(Client *client, const IRCMessage &message);
		void	inviteCmd(Client *client, const IRCMessage &message);
		void	kickCmd(Client *client, const IRCMessage &message);
		void	privmsgCmd(Client *client, const IRCMessage &message);
		void	whoisCmd(Client *client, const IRCMessage *message);

		// used during modeCmd
		void	parseMode(Client *client, Channel *channel, const IRCMessage &message) const;


		// Used jus by whoisCmd
		std::string	formatWhoisUser(Client *targetClient) const;
		std::string	formatWhoisChannels(Client *targetClient, const std::map<std::string, Channel*> &channels) const;
		std::string	formatWhoisServer(Client *targetClient) const;

		// Global notifications
		void	dispatchNotifications(Client *sender, const std::string &message, bool includeSender) const;
		void	notifyNickChange(Client *sender, const std::string &oldNickname) const;
		void	notifyQuit(Client *sender, const std::string &reason) const;

		// Send a message to a client
		void	sendInviteMessage(Client *inviter, Client *invited, Channel *channel) const;


	public:
		Server(const int port, const std::string &password);
		~Server();

		void	startServer();

//-------------------------EXCEPTIONS-------------------------

		class SocketException : public std::exception
		{
			virtual const char *what() const throw()
			{
				return "Error: socket creation failed";
			}
		};

		class BindException : public std::exception
		{
			virtual const char *what() const throw()
			{
				return "Error: bind failed";
			}
		};

		class ListenException : public std::exception
		{
			virtual const char *what() const throw()
			{
				return "Error: listen failed";
			}
		};

};

#endif