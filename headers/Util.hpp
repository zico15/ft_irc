#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <iostream>


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

#define MSH_HELP "help /me [msg]\r\nhelp /ame [msg]\r\nhelp /join <channel> [pwd]\r\nhelp /j <channel> [pwd]\r\nhelp /leave <channel> [reason]\r\nhelp /quit\r\nhelp /notice <nick/channel> [msg]\r\nhelp /query <nick>\r\nhelp /msg <nick/channel> [msg]\r\nhelp /smsg <nick/channel> [msg]\r\nhelp /amsg <nick/channel> [msg]\r\nhelp /op <nick>\r\nhelp /op <nick>\r\nhelp /deop <nick>\r\nhelp /oper <nick>\r\nhelp /voice <nick>\r\nhelp /unvoice <nick>\r\nhelp /topic <channel> [msg]\r\nhelp /away [reason]\r\nhelp /aaway [reason]\r\nhelp /invite <nick> [channel]\r\nhelp /exec <script> [parameters]\r\nhelp /notify <nick>\r\nhelp /kick <nick> [reason]\r\nhelp /ban <channel> [user|mask]\r\nhelp /unban <channel> [pattern]\r\nhelp /ignore <nick>\r\nhelp /quote <cmd>\r\nhelp /say [msg]\r\nhelp /raw [open/close]\r\nhelp /konsole\r\nhelp /server [url:port] [pwd]\r\nhelp /ddc SEND <nick> [filename]\r\nhelp /dcc\r\nhelp /ctcp <nick/channel> [msg]\r\n"
#define MSG_COMMAND_INVALID "Command Invalid!\n"
#define MSG_INVALID_CHANNEL "You are in the Channel!\r\n"
#define MSG_MSG_INVALID "\033[31mNickname Invalid!\n\033[0m"
#define MSG_NEW_CLIENT(username, fd) "\033[33mNew client: " + username + " fd: " + fd + "\n\033[0m";
#define MSG_CLOSE_CLIENT(username, fd) "\033[31mClose client: " + username + " fd: " + fd + "\n\033[0m";
int                         exit_error(std::string msg);
std::vector<std::string>	split(std::string str, std::string set);
std::string                 trim(std::string str);
std::string                 uppcase(std::string str);
bool                        isNumber(std::string str);
std::string                 ft_itoa(int n);
#endif
