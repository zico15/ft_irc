/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 15:48:35 by edos-san          #+#    #+#             */
/*   Updated: 2023/04/01 21:07:14 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
# define MSG_HPP


#define SERVER_NAME  std::string("test")

#define  RPL_WELCOME(nickname) std::string(":"+SERVER_NAME+" 001 "+nickname+" :Welcome to server, "+nickname)
#define  ERR_PASSWDMISMATCH(nickname) std::string(":"+SERVER_NAME+" 464 " + nickname + " :Password incorrect\r\n")
#define  NICKNAME_ERROR(nickname) std::string("433 *"+nickname+"* :Nickname incorreta.")
#define  ERR_UNKNOWNERROR(data)   std::string(":" + SERVER_NAME + " 421 " + std::string(data) + " :" + std::string(data))
#define  RPL_USERHOST(data)       std::string(":" + SERVER_NAME + " 302 " + std::string(data.substr(0, data.find(' '))) + " :" + data)
#define  RPL_ENDOFWHO(client)     std::string(":" + SERVER_NAME + " 315 " + std::string(client->getUsername()) + " :End of /WHO list.")

#define  RPL_JOIN(nick, user, host, channel) std::string(":" + nick + "!" + user + "@" + host + " JOIN " + channel)
#define  RPL_NAMREPLY(client, server, channel)  std::string(":" + SERVER_NAME + " 353 " + client->getNickname() + " = " + channel->getName() + " :@" + channel->nicksOnChannel())
#define  RPL_ENDOFNAMES(nickname, channel)      std::string(":" + SERVER_NAME + " 366 " + nickname + " " + channel->getName() + " :End of NAMES list")
#define  ERR_BADCHANNELKEY(nickname, channelname)   std::string(":" + SERVER_NAME + " 475 " + nickname + " " + channelname + " :Cannot join channel (+k)")
#define  PRV_MSG(nick, user, host, dest, message) std::string(":" + nick + "!" + user + "@" + host + " PRIVMSG " + dest + " " + message)
#define  LEAVE_CHANNEL(canal, client) std::string(":" + client->getNickname() + "!" + client->getUsername() + "@host" + " PART " + canal + " :Leaving the channel")

#define LIST_START(nick, numberChannels) std::string(":" + SERVER_NAME + " 321 " + nick + " Channel " + numberChannels + " :")
#define LIST_MID(nick, channel, numberClients)   std::string(":" + SERVER_NAME + " 322 " + nick + " " + channel->getName() + " " + numberClients + " :*no topic*")
#define LIST_END(nick) std::string(":" + SERVER_NAME + " 323 " + nick + " :End of LIST")

#define RPL_WHOREPLY(nick, canal, nickA, hostA, status, nameA) std::string(":" + SERVER_NAME + " 352 " + nick + " " + canal + " " + nickA + " " + hostA + " " + SERVER_NAME + " " + nickA + "_irc" + " " + status + " :0 " + nameA)
#define RPL_SYNTAXERROR(message) std::string("ERROR Invalid command syntax. " + std::string(message))
//: <servidor> 352 <seu-nick> <nome-do-canal> <nome-do-usuario> <endereco-ip> <servidor-origem> <nome-real-do-usuario> <status no canal>


//352	RPL_WHOREPLY	RFC1459	<client> <channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real_name>	Reply to vanilla WHO (See RFC). This format can be very different if the 'WHOX' version of the command is used (see ircu).
//315	RPL_ENDOFWHO	RFC1459	<client> <name> :<info>	Used to terminate a list of RPL_WHOREPLY replies
/*
:irc.example.com 352 user1 #channel username hostname irc.example.com nick G*@ :0 Real Name
:irc.example.com 352 user1 #channel username2 hostname2 irc.example.com nick2 H@ :1 Another Real Name
:irc.example.com 315 user1 #channel :End of /WHO list.
*/
#endif