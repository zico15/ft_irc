/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 13:11:36 by rteles            #+#    #+#             */
/*   Updated: 2023/04/06 15:11:17 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
# define MSG_HPP

#define BOT_HELLO_CHANNEL(channel, nick)    std::string("Hello, "+channel+"! I'm, " + nick + " a bot, if you want to know me more, use the command !help.")
#define BOT_HELLO(nickname)                 std::string("Hello, "+nickname+"!")
#define BOT_INVITE(channel)                 std::string("JOIN " + channel)
#define	BOT_GAME(game, seconds)             std::string("GAME START! " + game + " You have " + seconds + " seconds to play!")
#define BOT_GAME_RESULT(number, winners)    std::string("GAME END! Result: " + number + ", we have " + winners + " winners!")
#define BOT_LEAVE()                         std::string("BYE BYE!")
#define BOT_YOU_WIN(nick, exp)              std::string(nick + " you win " + exp + " exp! Congrats!")
#define BOT_DRAW(nick, exp)                 std::string(nick + " it's draw, you win " + exp + " exp!")
#define BOT_YOU_LOSE(nick)                  std::string(nick + " you lose!")
#define BOT_ERRO(nick, choise)              std::string(nick + " this isn't a good input! See !help command.")
#define	BOT_HELP()                          std::string("------------ SYSTEM ------------\n\
/nick [login]       change your login\n\
join [channel]     join channel\n\
leave              leave current channel\n\
quit               quit irc\n\
who                list of users in channel\n\
msg [login] [msg]  submit msg at login\n\
list               list of channel\n\
------------- BOT -------------\n\
!help		       	show help list\n\
!invite [channel] [?password]       invite me for your channel\n\
!helpgame           show game list\n\
!game [game] [yourChoise]   play a game \n\
!leaderboard        show the Leader Board\n")
#define	BOT_HELP_GAME()                     std::string("------------ GAMES ------------\n\
!game jankenpo [rock/paper/scissor]        play a game \"JanKenPo\"\n\
!game guess [1-10]                play a game \"Guess a Number\"")

#endif