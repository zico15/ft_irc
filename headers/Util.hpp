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
#define COLOUR_RED "\033[31m"
#define COLOUR_YELLO "\033[33m"

#define COLOUR_GREEN "\033[7;1;32m"
#define COLOUR_END "\033[0m"
#define COLOUR_YELLOW "\033[7;1;33m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define MSH_HELP "/nick [login]       change your login\n/join [channel]     join channel\n/leave              leave current channel\n/quit               quit irc\n/who                list of users in channel\n/msg [login] [msg]  submit msg at login\n/list               list of channel\n/me                 defined an action\n[msg]               send msg in channel\n"
#define MSG_COMMAND_INVALID "\033[31mCommand Invalid!\n\033[0m"
#define MSG_MSG_INVALID "\033[31mNickname Invalid!\n\033[0m"
#define MSG_NEW_CLIENT(username, fd) "\033[33mNew client: " + username + " fd: " + fd + "\n\033[0m";
#define MSG_CLOSE_CLIENT(username, fd) "\033[31mClose client: " + username + " fd: " + fd + "\n\033[0m";
int                         exit_error(std::string msg);
std::vector<std::string>	split(std::string str, std::string set);
std::string                 trim(std::string str);

#endif
