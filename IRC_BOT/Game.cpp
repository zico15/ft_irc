/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 01:00:29 by rteles            #+#    #+#             */
/*   Updated: 2023/03/30 15:37:47 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

Game::Game(void) {}

Game::Game(std::string room, int game, int time, int reward, int result): _room(room), _game(game),  _time(time), _reward(reward), _result(result)
{
    _start = clock();
    _vitory = "";
}

Game::~Game() {
    
}
