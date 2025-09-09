#ifndef UTILS_HPP
#define UTILS_HPP

int							parseInput(int argc, char **argv);

void						handleSigInt(int sig);

std::vector<std::string>	split(const std::string& str, const std::string& delimiter);

bool						insensitiveStringCompare(const std::string &str1, const std::string &str2);

std::string					toLower(const std::string &s);

bool						startsWith(const std::string &str, const std::string &prefix);

int							safeAtoi(const std::string &str);

std::string					intToString(int n);

std::string					join(const std::set<std::string> &elements, const std::string &separator);

std::string					join(const std::vector<std::string> &elements, const std::string &separator);

#endif