#ifndef UTILS_HPP
#define UTILS_HPP

#include <fcntl.h>
//#include <fstream>
#include <cstring>
//#include <stdlib.h>
//#include <sys/stat.h>
#include <vector>
#include <iostream>
//#include <string>
#include <sstream>
//#include <unistd.h>
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

#define MSH_HELP "help /me [msg]\r\nhelp /ame [msg]\r\nhelp /join <channel> [pwd]\r\nhelp /j <channel> [pwd]\r\nhelp /leave <channel> [reason]\r\nhelp /quit\r\nhelp /notice <nick/channel> [msg]\r\nhelp /query <nick>\r\nhelp /msg <nick/channel> [msg]\r\nhelp /smsg <nick/channel> [msg]\r\nhelp /amsg <nick/channel> [msg]\r\nhelp /op <nick>\r\nhelp /op <nick>\r\nhelp /deop <nick>\r\nhelp /oper <nick>\r\nhelp /voice <nick>\r\nhelp /unvoice <nick>\r\nhelp /topic <channel> [msg]\r\nhelp /away [reason]\r\nhelp /aaway [reason]\r\nhelp /invite <nick> [channel]\r\nhelp /exec <script> [parameters]\r\nhelp /notify <nick>\r\nhelp /kick <nick> [reason]\r\nhelp /ban <channel> [user|mask]\r\nhelp /unban <channel> [pattern]\r\nhelp /ignore <nick>\r\nhelp /quote <cmd>\r\nhelp /say [msg]\r\nhelp /raw [open/close]\r\nhelp /konsole\r\nhelp /server [url:port] [pwd]\r\nhelp /ddc SEND <nick> [filename]\r\nhelp /dcc\r\nhelp /ctcp <nick/channel> [msg]\r\n"
#define MSG_COMMAND_INVALID "Command Invalid!\n"
#define MSG_INVALID_CHANNEL "You are in the Channel!\r\n"
#define MSG_MSG_INVALID "\033[31mNickname Invalid!\n\033[0m"
#define MSG_NEW_CLIENT(username, fd) "\033[33mNew client: " + username + " fd: " + fd + "\n\033[0m";
#define MSG_CLOSE_CLIENT(username, fd) "\033[31mClose client: " + username + " fd: " + fd + "\n\033[0m";
int                         exit_error(std::string msg);
std::vector<std::string>	split(std::string str, std::string set);
std::string                 trim(std::string str);
/*std::string                 hhhelp_msg[] = {"/me msg\nSends msg to the channel preceded by your nickname. For example, if your nickname were Tux, typing /me brews a cup a tea. displays the message Tux brews a cup of tea. Unless they've filtered against these types of messages, all users see it. This kind of message is typically used to convey non-verbal information, such as your current emotional state, or what you are physically doing. If you are leaving your workstation for a short time, use the /away command instead.",
                                            "/ame msg\nLike the /me command above except that the message is sent to all the channels and queries you have open on all the servers you have connected in Konversation.",
                                            "/join channel [pwd]\nJoins channel. If the server permits it, and if the channel does not already exist, it is created and you are the channel operator. If the channel requires a password in order to join, specify as pwd. Most channel names begin with #.",
                                            "/j channel [pwd]\nSame as /join above.",
                                            "/part [channel] [reason]\nLeaves channel with reason and closes the corresponding channel tab. Within a Konversation channel or query tab, you can just enter /part which closes that channel or query with your default reason as given in the Settings.",
                                            "/leave [channel] [reason]\nSame as /part above.",
                                            "/quit\nWhen entered in any channel, query, or DCC tab, disconnects from the corresponding server and closes all channel, query, or DCC tabs open on that server.",
                                            "/notice nick|channel msg\nSends msg to the user with nickname nick or to everyone in channel. The following is an example of what appears to you in the channel tab\n[17:14] [Notice] Sending notice \"I think I understand\" to #konversation\nAnd this is what the recipients see\n[17:15] [Notice] -Eisfuchs- I think I understand",
                                            "/query nick\nCreates a new tab with the user's nick for private conversation, commonly known on IRC as a Query. Everything written here is only visible to you and your query partner. You may enter more than one nickname separated by spaces and a query tab will be opened for each nickname. Each such query tab will be private between you and the query partner.",
                                            "/msg nick|channel message\nSends a private message to the user with nickname nick or to all users in channel. Unlike /query, does not open a new tab. If message begins with /me , the message is formatted in the same manner as a /me command.\n\nExamples\n\n/msg Eisfuchs I need some help with the msg command\n\n/msg #konversation New version of Konvi is online!\n\n/msg Eisfuchs /me is compiling the Handbook right now.",
                                            "/smsg nick|channel message\nSame as /msg except the message is not displayed on your screen. This is useful for preventing passwords and other sensitive information from remaining exposed on your screen or being logged.",
                                            "/amsg nick|channel message\nLike the /msg command above except that the message is sent to all the channels and queries you have open on all the servers you have connected in Konversation.",
                                            "/op nick\nWhen entered into a channel tab, attempts to give nick operator privilege for the channel. May not work if you lack sufficient privilege in the channel (you must be a channel operator yourself). You may enter more than one nickname separated by spaces.",
                                            "/deop nick\nWhen entered into a channel tab, takes away channel operator privilege from nick. You may enter more than one nickname separated by spaces.",
                                            "/oper [nick]\nGrants the user with nickname nick IRC-Op (Administrator) privilege. This only works if the nickname is listed in the server's O: line. IRC-Ops have the privilege to /kill nicknames, /connect servers to the IRC network, and more. If nick is not entered, attempts to grant yourself IRC-Op privilege. You will be prompted for your IRC-Op user name and password in either case.",
                                            "/voice nick\nIn certain channels (with +m flag), users cannot send messages unless they have voice privilege. This command attempts to give nick voice privilege. You must have channel operator privilege to execute this command. This command is mostly used when you are an operator for a moderated channel.",
                                            "/unvoice nick\nTakes away voice privilege from nick in the channel.",
                                            "/topic [channel] [message]\nAttempts to set the topic of channel to message. When entered in a channel tab, channel may be omitted and defaults to the channel active in the tab. If message is omitted, displays the current topic of the channel. Changing a channel topic may fail if you lack sufficient privilege.",
                                            "/away reason\nIf anyone tries to send you a private message or invite you to a channel, they will be told that you are away from your workstation for reason. To change your state back to normal, enter /away without a reason.",
                                            "/aaway reason\nLike the /away command above except that the command is sent to all the servers you have connected in Konversation.",
                                            "/invite nick [channel]\nInvites the user with nickname nick to join channel. This command is especially useful if the channel is invite-only. When entered in a channel tab, channel may be omitted.",
                                            "/exec script [parameters]\nExecutes the Konversation-compatible shell script named script, passing it parameters. See Scripting for more information on using and writing scripts.\n\nExamples:\n\n/sayversion\nSends your system's Konversation, Qt™, KF5 and Plasma™ version numbers to the channel.",
                                            "/exec cmd uname -a\nSends your Linux® version string (the output of “uname -a”) to the channel.",
                                            "/notify [nick]\nAdds or removes nick from your Watched Nicks list. If the nickname is already in the list, it is removed. If it is not in the list, it is added. If nick is omitted, displays the current list of watched nicknames.",
                                            "/kick nick reason\nWhen entered in a channel tab, attempts to kick the user with nickname nick from the channel with reason. If reason is not entered, defaults to the reason in the Settings. This command may only be used in a channel tab.",
                                            "/ban [-host|-domain| -userhost|-userdomain] [channel] user|mask\nAttempts to ban the user or mask from channel. When entered in a channel tab, channel may be omitted. For more information on hosts, domains, and masks, see the documents at irchelp.org. To get the current ban list for a channel, enter /mode channel b.",
                                            "/unban [channel] pattern\nReverts a previous banning from a channel. When entered in a channel tab, channel may be omitted.",
                                            "/ignore [-all] nick\nAdds nick to your Ignore list. You may specify more than one nickname separated by spaces. Channel messages from users on your Ignore list are not displayed. In addition, if you enter -all, all message types (queries, notices, CTCP, DCC, and Exceptions) from the user are ignored as well as channel messages.",
                                            "/quote cmd\nSends cmd to the server.",
                                            "/say msg\nWhen entered in a channel tab, sends msg to the channel. This is the same as entering msg in the channel Input Line. Useful for sending IRC commands when you don't want to execute the command. Two command characters (//) in a row does the same thing as /say. Also used by scripts to send messages.",
                                            "/raw [open|close]\nPlaces the server log in raw mode (open) or stops raw mode (close). If neither open or close is entered, defaults to open. In raw mode, the log displays every message received from the server in native IRC format.",
                                            "/konsole\nOpens a new tab running a Konsole.",
                                            "/server [url] [pwd]\nConnects to the server at address url. If url is omitted, connects to the server associated with the tab where you enter the command. This is useful for re-connecting to a server when disconnected. If the server requires a password in order to connect to it, enter it for pwd. If the server requires a port number other than the default (6667), enter the url like this: irc.kde.org:6665.",
                                            "/dcc SEND nick [filename]\nSends filename to the user with nickname nick. If filename is omitted, a dialog appears for you to select a file.",
                                            "/dcc\nOpens the DCC Status tab.",
                                            "/ctcp nick|channel ping\nSends a PING message to nick or channel. This is useful for determining if someone you are talking with has a high network lag, or (when sent to a channel) if your system has a high lag.",
                                            "/ctcp nick|channel msg\nSends a message to nick or all users in channel. This is similar to the /msg command above, except that the message is sent using the CTCP protocol. The response on the other end depends on the IRC client program the user is running. For example, when conversing with another Konversation user, /ctcp Eisfuchs TIME will return the system time on Eisfuch's computer. /ctcp Eisfuchs VERSION will return the Konversation version number from Eisfuch's computer."};*/
#endif
