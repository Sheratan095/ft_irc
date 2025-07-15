#ifndef UTILS_HPP
#define UTILS_HPP

int		parseInput(int argc, char **argv);

void	handleSigInt(int sig);

void	printRawMessage(int bytes_received, char *buffer);

#endif