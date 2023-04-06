/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot_Game.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:39:47 by rteles            #+#    #+#             */
/*   Updated: 2023/04/06 15:09:21 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This code creates a game that has 2 modes: a game of rock-paper-scissors and a guessing game.
The rock-paper-scissors game has the computer choose a random number from 1 to 3.
If the number the player chooses is the same as the computer's number, the player draws.
If the number the player chooses is 1 greater than the computer's number or 2 less than the computer's number, the player wins.
If the number the player chooses is 1 less than the computer's number or 2 greater than the computer's number, the player loses.
The guessing game has the computer choose a random number from 1 to 10.
If the number the player chooses is the same as the computer's number, the player wins.
If the number the player chooses is not the same as the computer's number, the player loses.
*/
#include "Bot.hpp"

std::string convertInput(int input)
{
	if (input == 1)
		return "Rock";
	else if (input == 2)
		return "Paper";
	else if (input == 3)
		return "Scissor";
	else
		return "";
}

void	Bot::rockPapperScissors(std::string nick, std::string choise, Game *game)
{
	std::string result;
	int			input = 0;
	int random = game->getResult();
	
	if (choise.find("rock") != std::string::npos)
		input = 1;
	else if (choise.find("paper") != std::string::npos)
		input = 2;
	else if (choise.find("scissor") != std::string::npos)
		input = 3;
	else
		return ;
	
	game->setResult(random);
	
	if (input == random)
		game->setWinnerOrLosser(nick, 1);
	else if (input - random == 1 || input - random == -2)
		game->setWinnerOrLosser(nick, 2);
	else
		game->setWinnerOrLosser(nick, 0);
}
/*   WIN	     LOSE		 DRAW
	2-1 = 1		2-3 = -1	2-2 = 0
	3-2 = 1		3-1 = 2		3-3 = 0
	1-3 = -2	1-2 = -1	1-1 = 0
*/

void Bot::guessNumber(std::string nick, std::string choise, Game *game)
{
	std::string result = "";
	int random = game->getResult();
    std::istringstream iss(choise);
	int input;

	game->setResult(random);

	if (iss >> input)
		input = input;
	else
		return ;

	if (input == random)
		game->setWinnerOrLosser(nick, 2);
	else
		game->setWinnerOrLosser(nick, 0);
}

void Bot::gamePlay(std::string user, std::string channel, std::string game)
{
	std::string callBack = "";
	Game		*room_game;

	addPlayer(user);
	if (game.find("jankenpo") != std::string::npos)
	{
		if (!channel.empty())
			room_game = addGame(channel, JANKENPO, 10, 200, 1 + (std::rand() % 3));
		else
			room_game = addGame(user, JANKENPO, 0, 200, 1 + (std::rand() % 3));
		
		rockPapperScissors(user, game.substr(game.find("jankenpo")+9, game.size()).c_str(), room_game);
		
		room_game->setVitory("You default a " + convertInput(room_game->getResult()) + "!");
	}
	else if (game.find("guess") != std::string::npos)
	{
		if (!channel.empty())
			room_game = addGame(channel, GUESS, 10, 1000, 1 + (std::rand() % 10));
		else
			room_game = addGame(user, GUESS, 0, 1000, 1 + (std::rand() % 10));

		guessNumber(user, game.substr(game.find("guess")+6, game.size()).c_str(), room_game);
		
		room_game->setVitory("The Number is: " + convertToString(room_game->getResult()));
	}
}

Game	*Bot::addGame(std::string room, int game, int time, int reward, int result)
{
    if (!_games[room])
	{
		std::string gameMessage = "";
		_games[room] = new Game(room, game, time, reward, result);
		std::cout << "\033[35mNEW GAME ROOM: " << room << "\033[0m" << std::endl;
		if (game == GUESS)
			gameMessage = "Guess the Number! I'm thinking a number from 1 to 10! Using !game guess 1-10";
		else if (game == JANKENPO)
			gameMessage = "JanKenPo! Try too beat me! Using !game jankenpo rock/paper/scissor!";
		debug("", BOT_GAME(gameMessage, convertToString(_games[room]->getTimeLimit())), "", room);
	}

	return _games[room];
}

std::string	Bot::winners(std::string room)
{
	if (!_games[room])
		return "";

	std::string garbage = room;

	Game *game = _games[room];
	
	std::vector<std::string> winners = game->getWinners();
	std::vector<std::string> draws = game->getDraws();
	std::vector<std::string> losers = game->getLosers();
	std::vector<std::string>::iterator user;
	int reward = game->getReward();
	for (user = winners.begin(); user != winners.end(); ++user)
    {
		std::string callBack = BOT_YOU_WIN(*user, convertToString(reward)) + " " + game->getVictory();
		debug("", callBack, *user, "");
		setPlayer(*user, true, reward);
		usleep(100);
	}
	for (user = draws.begin(); user != draws.end(); ++user)
    {
		std::string callBack = BOT_DRAW(*user, convertToString(50));
		debug("", callBack, *user, "");
		setPlayer(*user, true, 50);
		usleep(100);
	}
	for (user = losers.begin(); user != losers.end(); ++user)
    {
		std::string callBack = BOT_YOU_LOSE(*user);
		debug("", callBack, *user, "");
		usleep(100);
	}

	if (game->getGame() == GUESS)
		debug("", BOT_GAME_RESULT(convertToString(game->getResult()), convertToString(winners.size())), "", room);
	else
		debug("", BOT_GAME_RESULT(convertInput(game->getResult()), convertToString(winners.size())), "", room);

	delete _games[room];

	std::cout << "\033[35mDELETE GAME ROOM: " << garbage << "\033[0m" << std::endl;

	return garbage;
}

void	Bot::gameTime(void)
{
    std::map<std::string, Game *>::iterator it;
	std::vector<std::string> finish_games;
	
	for (it = this->_games.begin(); it != this->_games.end(); ++it)
    {
		(*it).second->setTime(1);
		if ((*it).second->getTimeLimit() <= 0)
			finish_games.push_back(this->winners((*it).second->getRoom()));
	}

    std::vector<std::string>::iterator v_it;
	for (v_it = finish_games.begin(); v_it != finish_games.end(); ++v_it)
	{
		if (!(*v_it).empty())
			_games.erase(*v_it);
	}
}
