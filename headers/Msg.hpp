/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 15:48:35 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/24 16:51:32 by edos-san         ###   ########.fr       */
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
#define  RPL_ENDOFWHO(client)     std::string(":" + SERVER_NAME + " 315 " + std::string(client->getNickname()) + " :End of /WHO list.")
#define  RPL_TOPIC(client, data)  std::string(":" + SERVER_NAME + " 332 " + std::string(client->getNickname()) + " " + data.substr(0, data.find(' ')) + " :Welcome to " + data.substr(0, data.find(' ')))

//JOIN RPL_TOPIC REPLY: :irc.example.com 332 username #channel :Welcome to #channel

//352	RPL_WHOREPLY	RFC1459	<client> <channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real_name>	Reply to vanilla WHO (See RFC). This format can be very different if the 'WHOX' version of the command is used (see ircu).
//315	RPL_ENDOFWHO	RFC1459	<client> <name> :<info>	Used to terminate a list of RPL_WHOREPLY replies
/*
:irc.example.com 352 user1 #channel username hostname irc.example.com nick G*@ :0 Real Name
:irc.example.com 352 user1 #channel username2 hostname2 irc.example.com nick2 H@ :1 Another Real Name
:irc.example.com 315 user1 #channel :End of /WHO list.
*/
#endif