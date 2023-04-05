/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 15:48:35 by edos-san          #+#    #+#             */
/*   Updated: 2023/04/05 22:02:30 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
# define MSG_HPP


#define SERVER_NAME  std::string("test")

#define  RPL_WELCOME(nickname)          std::string(":"+SERVER_NAME+" 001 "+nickname+" :Welcome to server, "+nickname)
#define  ERR_PASSWDMISMATCH(nickname)   std::string(":"+SERVER_NAME+" ERR_PASSWDMISMATCH " + nickname + " :Password incorrect")
#define  NICKNAME_ERROR(nickname)       std::string("433 *"+nickname+"* :Nickname incorreta.")
#define  ERR_UNKNOWNERROR(data)         std::string(":" + SERVER_NAME + " 421 " + std::string(data) + " :" + std::string(data))
#define  RPL_USERHOST(data)             std::string(":" + SERVER_NAME + " 302 " + std::string(data.substr(0, data.find(' '))) + " :" + data)
#define  RPL_ENDOFWHO(client)           std::string(":" + SERVER_NAME + " 315 " + std::string(client->getUsername()) + " :End of /WHO list.")

#define  RPL_JOIN(nick, user, host, channel)        std::string(":" + nick + "!" + user + "@" + host + " JOIN " + channel)
#define  RPL_NAMREPLY(client, server, channel)      std::string(":" + SERVER_NAME + " 353 " + client->getNickname() + " = " + channel->getName() + " :@" + channel->nicksOnChannel())
#define  RPL_ENDOFNAMES(nickname, channel)          std::string(":" + SERVER_NAME + " 366 " + nickname + " " + channel->getName() + " :End of NAMES list")
#define  ERR_BADCHANNELKEY(nickname, channelname)   std::string(":" + SERVER_NAME + " 475 " + nickname + " " + channelname + " :Cannot join channel (+k)")
#define  PRV_MSG(nick, user, host, dest, message) std::string(":" + nick + "!" + user + "@" + host + " PRIVMSG " + dest + " " + message)
#define  LEAVE_CHANNEL(canal, client) std::string(":" + client->getNickname() + "!" + client->getUsername() + "@host" + " PART " + canal + " :Leaving the channel")

#define LIST_START(nick, numberChannels) std::string(":" + SERVER_NAME + " 321 " + nick + " Channel " + numberChannels + " :")
#define LIST_MID(nick, channel, numberClients)   std::string(":" + SERVER_NAME + " 322 " + nick + " " + channel->getName() + " " + numberClients + " :" + channel->getTopic())
#define LIST_END(nick) std::string(":" + SERVER_NAME + " 323 " + nick + " :End of LIST")

#define RPL_WHOREPLY(nick, canal, nickA, hostA, status, nameA) std::string(":" + SERVER_NAME + " 352 " + nick + " " + canal + " " + nickA + " " + hostA + " " + SERVER_NAME + " " + nickA + "_irc" + " " + status + " :0 " + nameA)
#define RPL_SYNTAXERROR(message) std::string("ERROR Invalid command syntax. " + std::string(message))

#endif