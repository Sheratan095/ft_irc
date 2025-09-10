#include "Bot.hpp"

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

bool	startsWith(const std::string &str, const std::string &prefix)
{
	if (str.size() < prefix.size())
		return (false);

	return (str.compare(0, prefix.size(), prefix) == 0);
}

int safeAtoi(const std::string &s)
{
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (!std::isdigit(s[i]))
			return (-1); // invalid number
	}
	return (std::atoi(s.c_str()));
}

std::string	intToString(int n)
{
	std::ostringstream	oss;

	oss << n;

	return (oss.str());
}

// Join vector<string> with a separator
std::string	join(const std::vector<std::string> &elements, const std::string &separator)
{
	std::string	result;

	for (size_t i = 0; i < elements.size(); ++i)
	{
		result += elements[i];
		if (i != elements.size() - 1)
			result += separator;
	}
	return (result);
}

// Join set<string> with a separator
std::string	join(const std::set<std::string> &elements, const std::string &separator)
{
	std::string	result;

	for (std::set<std::string>::const_iterator it = elements.begin(); it != elements.end(); ++it)
	{
		if (!result.empty())
			result += separator;
		result += *it;
	}
	return (result);
}

void	handleSigInt(int sig)
{
	if (sig	==	SIGINT)
	{
		std::cout << "\nInterrupt signal received" << std::endl;
		BOT_RUNNING = false;
	}
}

std::vector<IRCMessage> Bot::parseMessage(const std::string &message) const
{
	std::vector<IRCMessage> messages;

	// 1. Normalize line endings (\r\n → \n, \r → \n)
	std::string normalized = message;
	replaceAll(normalized, "\r\n", "\n");
	replaceAll(normalized, "\r", "\n");

	// 2. Split into lines
	std::vector<std::string> lines = split(normalized, "\n");

	for (size_t i = 0; i < lines.size(); ++i)
	{
		const std::string &line = lines[i];
		if (line.empty())
			continue;

		IRCMessage msg;
		size_t pos = 0;
		std::string rest = line;

		// 3. Prefix (optional)
		if (rest[0] == ':')
		{
			pos = rest.find(' ');
			if (pos == std::string::npos)
				continue; // Invalid line
			msg.prefix = rest.substr(1, pos - 1);
			rest = rest.substr(pos + 1);
		}

		// 4. Command
		pos = rest.find(' ');
		if (pos == std::string::npos)
		{
			msg.command = rest;
			// Convert command to uppercase
			std::transform(msg.command.begin(), msg.command.end(), msg.command.begin(), ::toupper);
			messages.push_back(msg);
			continue;
		}
		msg.command = rest.substr(0, pos);
		// Convert command to uppercase
		std::transform(msg.command.begin(), msg.command.end(), msg.command.begin(), ::toupper);
		rest = rest.substr(pos + 1);

		// 5. Parameters
		std::vector<std::string> params;
		while (!rest.empty())
		{
			if (rest[0] == ' ')
			{
				rest = rest.substr(1);
				continue;
			}
			if (rest[0] == ':')
			{
				// Trailing param — keep raw (important for CTCP/DCC)
				msg.trailing = rest.substr(1);
				break;
			}
			pos = rest.find(' ');
			if (pos == std::string::npos)
			{
				params.push_back(rest);
				break;
			}
			params.push_back(rest.substr(0, pos));
			rest = rest.substr(pos + 1);
		}

		msg.parameters = params;
		messages.push_back(msg);
	}

	return messages;
}

void	replaceAll(std::string &str, const std::string &from, const std::string &to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

std::string	getNickByPrefix(const std::string &prefix)
{
	size_t	excl_pos = prefix.find('!');

	if (excl_pos == std::string::npos)
		return (prefix); // No '!', return whole prefix as nick

	return (prefix.substr(0, excl_pos));
}

void	printRawMessage(const std::vector<IRCMessage> &messages)
{
	for (size_t i = 0; i < messages.size(); ++i)
	{
		const IRCMessage	&msg = messages[i];

		if (msg.prefix.empty())
			std::cout << "No prefix, " << std::endl;
		else
			std::cout << "Prefix: " << msg.prefix << ", " << std::endl;

		if (msg.command.empty())
			std::cout << "No command " << std::endl;
		else
			std::cout << "Command: " << msg.command << std::endl;

		if (msg.parameters.empty())
			std::cout << "No parameters " << std::endl;
		else
		{
			std::cout << "Parameters: ";
			for (size_t i = 0; i < msg.parameters.size(); ++i)
				std::cout << msg.parameters[i] << " " ;

			std::cout << std::endl;
		}

		if (!msg.trailing.empty())
			std::cout << "Trailing: " << msg.trailing << std::endl;

		std::cout << std::endl;

	}

}
