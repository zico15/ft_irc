/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:39:44 by rteles            #+#    #+#             */
/*   Updated: 2023/04/04 23:32:13 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BOT_HPP
# define BOT_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/poll.h>
#include <sstream>
#include <stdlib.h>
#include <netdb.h>
#include <map>
#include <algorithm>
#include <vector>
#include "Msg.hpp"
#include "Game.hpp"

enum Type_Game{
  JANKENPO,
  GUESS,  
};

#define BUFFER_SIZE 1024

class Bot
{
    private:
        std::string _name;
        std::string _hostname;
		std::string	_port;
		std::string	_password;
        int			_socket;
		pollfd		*pollEvents;
        std::map<std::string, std::map<std::string, int> > _players;
        std::map<std::string, Game *>                      _games;

        Bot(void);

    public:
		Bot(std::string host, char * port, std::string password, std::string name);
        Bot(std::string const & type);
        Bot( Bot const & src);
        
        ~Bot(void);

        Bot & operator=(Bot const & rhs);

        void        authenticate(void);
		void	    run(void);
		int 	    recive(void);
		int 	    response(std::string message);
		void	    sendMessage(std::string const command, std::string const message);
		void	    privateMessage(std::string message);
        void        debug(std::string message, std::string callBack, std::string user, std::string channel);
        void        quit(void);
        void	    welcomeChannel(std::string message);
        void    	invite(std::string message);

        //Games
        void        gamePlay(std::string user, std::string channel, std::string game);
        void	    gameTime(void);
        void	    rockPapperScissors(std::string nick, std::string choise, Game *game);
        void        guessNumber(std::string nick, std::string choise, Game *game);
        Game	    *addGame(std::string room, int game, int time, int reward, int result);
        std::string winners(std::string room);

        //Player [EXP]
        std::map<std::string, int>  &addPlayer(std::string nick);
        void                        setPlayer(std::string nick, bool isWin, int exp);
        std::string                 showLeaderBoard(void);

        std::string convertToString(int input)
        {
            std::ostringstream stream;
            
        	stream << input;
        
            std::string nbr = stream.str();
        
        	return nbr;
        }
};


#endif