#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <csignal>
#include "Server.hpp"

int		parseInput(int argc, char **argv);

void	handleSigInt(int sig);


#endif