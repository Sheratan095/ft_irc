#include "Irc.hpp"

void	handleSigInt(int sig)
{
	if (sig	==	SIGINT)
	{
		std::cout << "\nInterrupt signal received. Shutting down server..." << std::endl;
		SERVER_RUNNING = false;
	}
}

int	parseInput(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "ERROR: Usage: " << argv[0] << " <port> <password>" << std::endl;
		exit(1);
	}

	std::string	port = argv[1];
	std::string	psw = argv[2];

	if (port.size() == 0 || psw.size() == 0)
	{
		std::cerr << "ERROR: Usage: " << argv[0] << " <port> <password>" << std::endl;
		exit(1);
	}

	// Find position of a character not in C string
	if (port.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cerr << "ERROR: Invalid port number" << std::endl;
		exit(1);
	}

	double	portnm = std::strtod(port.c_str(), NULL);
	if (portnm < 0 || portnm > 65535)
	{
		std::cerr << "ERROR:  Invalid port number" << std::endl;
		exit(1);
	}

	return (portnm);
}

std::vector<std::string>	split(const std::string& str, const std::string& delimiter)
{
		std::vector<std::string>	result;
		size_t						start = 0;
		size_t						end;

		while ((end = str.find(delimiter, start)) != std::string::npos)
		{
			result.push_back(str.substr(start, end - start));
			start = end + delimiter.length();
		}

		result.push_back(str.substr(start)); // Add remaining part

		return (result);
}

bool	insentiveStringCompare(const std::string &str1, const std::string &str2)
{
	if (str1.size() != str2.size())
		return (false);

	for (size_t i = 0; i < str1.size(); ++i)
	{
		if (std::tolower(str1[i]) != std::tolower(str2[i]))
			return (false);
	}

	return (true);
}

void	sendMessage(SocketFd socketFfd, const std::string &message)
{
	ssize_t	bytesSent = send(socketFfd, message.c_str(), message.size(), 0);

	if (bytesSent < 0)
		std::cerr << "Error sending message to socket fd: " << socketFfd << std::endl;
}

std::string toLower(const std::string &s)
{
	std::string	res = s;

	for (size_t i = 0; i < res.size(); ++i)
		res[i] = std::tolower(res[i]);

	return (res);
}