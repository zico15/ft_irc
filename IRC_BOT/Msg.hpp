/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 13:11:36 by rteles            #+#    #+#             */
/*   Updated: 2023/03/30 16:00:08 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
# define MSG_HPP

#define BOT_HELLO(nickname) std::string("Hello, "+nickname+"!")
#define	BOT_HELP() std::string("------------ SYSTEM ------------\n\
/nick [login]       change your login\n\
join [channel]     join channel\n\
leave              leave current channel\n\
quit               quit irc\n\
who                list of users in channel\n\
msg [login] [msg]  submit msg at login\n\
list               list of channel\n\
me                 defined an action\n\
------------- BOT -------------\n\
!help		       	show help list\n\
!help game           show game list\n\
!game [game] [yourChoise]   play a game \"Guess a Number\"\n\
!leaderboard        show the Leader Board\n")

#define	BOT_HELP_GAME() std::string("------------ GAMES ------------\n\
!game JanKenPo [rock/papper/scissors]   play a game \"JanKenPo\"\n\
!game GuessNumber [1-10]   play a game \"Guess a Number\"")


#define	BOT_GAME() std::string("Let's PLAY A GAME! Guess the number I'm thinking of from 1 to 10! You have 10 seconds to guess!")

#define BOT_GAME_RESULT(number, winners) std::string("Result: " + number + ", we have " + winners + " winners!")

#define BOT_LEAVE() std::string("BYE BYE!")

#define BOT_YOU_WIN(nick, exp) std::string(nick + " you win " + exp + " exp! Congrats!")
#define BOT_DRAW(nick, exp) std::string(nick + " it's draw, you win " + exp + " exp!")
#define BOT_YOU_LOSE(nick) std::string(nick + " you lose!")

#define BOT_ERRO(nick, choise) std::string(nick + " this isn't a good input! See !help command.")

#endif