#ifndef UTILS_HPP
#define UTILS_HPP

int							parseInput(int argc, char **argv);

void						handleSigInt(int sig);

std::vector<std::string>	split(const std::string& str, const std::string& delimiter);

#endif