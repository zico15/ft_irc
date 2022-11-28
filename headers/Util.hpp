#ifndef UTILS_HPP
#define UTILS_HPP

#include <fcntl.h>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <sys/stat.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <csignal>

#define SPACES "\n\r\t\v\f "

int                         exit_error(std::string msg);
std::vector<std::string>	split(std::string str, std::string set);

#endif
