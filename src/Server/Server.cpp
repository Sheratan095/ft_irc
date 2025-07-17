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

	std::cout << "Server created with port: " << port << " and password: '" << password << "'" << std::endl;
}

Server::~Server()
{
	close(_socketFd);
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
	// Create and configure the server address structure
	struct sockaddr_in	addr = {};

	// Configure the address structure:
	addr.sin_family = AF_INET;						// IPv4 address family
	//htonl  converts a 32-bit (long) integer from your machine’s native byte order (host order) to network byte order (big-endian)
	// INADDR_ANY is 0x00000000
	addr.sin_addr.s_addr = htonl(INADDR_ANY);		// Accept connections from any IP
	addr.sin_port = htons(_port);					// Convert port to network byte order

	// Bind the socket to the specified address and port
	// This associates the socket with a specific network interface and port
	// After binding, the socket can listen for incoming connections on this address
	if (bind(_socketFd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		// Binding failed - port might be already in use or insufficient permissions
		close(_socketFd);
		return (false);
	}

	return (true);
}

bool	Server::startSocketListening()
{
	if (listen(_socketFd, SOMAXCONN) == -1)
	{
		// FAILED TO LISTEN ON SOCKET
		close(_socketFd);
		return (false);
	}
	return (true);
}