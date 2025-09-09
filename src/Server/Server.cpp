#include "Server.hpp"

bool	SERVER_RUNNING = true;

Server::Server(const int port, const std::string &password): _port(port), _password(password)
{
	if (password.empty())
		throw (std::invalid_argument("Password cannot be empty."));

	// 0 - 1024 are well-known ports and are used for standard services.
	// 1024 - 49151 are registered ports and can be used by user applications
	// 49152 - 65535 are dynamic or private ports and can be used for ephemeral(effimera) connections.
	if (port < 1024 || port > 65535)
		throw std::out_of_range("Port must be between 1024 and 65535.");

	_bot = new Bot("ChatBot");

	std::cout << "Server created with password: '" << password << "'" << std::endl;
}

Server::~Server()
{
	close(_socketFd);

	for (size_t i = 0; i < _pollFds.size(); ++i)
	{
		if (_pollFds[i].fd != -1)
			close(_pollFds[i].fd);
	}

	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i] != NULL)
		{
			delete _clients[i];
			_clients[i] = NULL;
		}
	}

	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		delete (it->second); // Delete each channel

	_channels.clear();

	delete (_bot);

	std::cout << "Server destroyed." << std::endl;
}

//----------------------------------------------------------

void	Server::startServer()
{
	signal(SIGINT, handleSigInt);

	if (!createSocket())
		throw (SocketException());

	if (!bindSocket())
		throw (BindException());

	pollfd	serverFd = {};
	serverFd.fd = _socketFd;      // set your listening socket
	serverFd.events = POLLIN;     // watch for readability (incoming connections)
	_pollFds.push_back(serverFd); // add to the poll list

	if (!startSocketListening())
		throw (ListenException());

	run();
	
	close(_socketFd);
}

bool	Server::createSocket()
{
	// Create a TCP socket using IPv4
	// AF_INET: Address family for IPv4
	// SOCK_STREAM: TCP socket type (reliable, connection-oriented)
	// 0: Protocol (0 = default protocol for the socket type, which is TCP)
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd == -1)
	{
		// Socket creation failed - could be due to system limits or permissions
		return (false);
	}

	// Set socket option SO_REUSEADDR to allow immediate reuse of the port
	// This prevents "Address already in use" error when restarting the server
	// SOL_SOCKET: Socket level option (not protocol specific)
	int	opt = 1;
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		// Failed to set socket options - close the socket to prevent resource leak
		close(_socketFd);
		return (false);
	}

	return (true);
}

bool	Server::bindSocket()
{
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);

	// Resolve hostname → IP
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == -1)
	{
		std::cerr << "Error: gethostname failed" << std::endl;
		close(_socketFd);
		return false;
	}

	struct hostent *host = gethostbyname(hostname);
	if (host == NULL)
	{
		std::cerr << "Error: gethostbyname failed" << std::endl;
		close(_socketFd);
		return false;
	}

	struct in_addr **addr_list = reinterpret_cast<struct in_addr **>(host->h_addr_list);
	if (addr_list[0] == NULL)
	{
		std::cerr << "Error: no IP addresses found for host" << std::endl;
		close(_socketFd);
		return false;
	}

	// Use the first IP associated with this hostname
	addr.sin_addr = *addr_list[0];

	if (bind(_socketFd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) == -1)
	{
		std::cerr << "Error: socket binding failed" << std::endl;
		close(_socketFd);
		return false;
	}

	return true;
}


bool	Server::startSocketListening()
{
	if (listen(_socketFd, SOMAXCONN) == -1)
	{
		// FAILED TO LISTEN ON SOCKET
		close(_socketFd);
		return (false);
	}

	 // After listen() succeeds, retrieve the bound address
	struct sockaddr_in	addr;
	socklen_t			len = sizeof(addr);

	if (getsockname(_socketFd, reinterpret_cast<struct sockaddr*>(&addr), &len) == 0)
	{
		char	ipStr[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &addr.sin_addr, ipStr, sizeof(ipStr)) != NULL)
			_ip = ipStr;
	}

	return (true);
}