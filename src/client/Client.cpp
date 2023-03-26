/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 22:40:38 by edos-san          #+#    #+#             */
/*   Updated: 2023/03/26 23:56:55 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

static	int		id_clinet = 1;	

Client::Client(): _isConnect(false)
{
    return ;
}

Client::Client(int fd, int index): _isConnect(false)
{
    _fd = fd;
    _index_fd = index;
    _username = "";
    _nickname = "";
    _password = "";
    _capend = false;
    std::cout << MSG_NEW_CLIENT(std::string(""), std::to_string(fd));
}

Client::~Client() 
{
    std::cout << MSG_CLOSE_CLIENT(_username, std::to_string(_fd));
}

bool Client::isValid()
{
    return !(_username.empty() || _nickname.empty() || _password.empty() || !_capend);
}
void Client::setNickname(const std::string& nickname){
	_nickname = trim(nickname);
	//std::cout << "Client: " << _username << " set nickname: " << _nickname << std::endl;
}

bool Client::isNickname(std::map<int, Client *> clients, Client *client)
{
    std::map<int, Client *>::iterator it;

    it = clients.begin();
    for (it; it != clients.end(); it++)
    {
        if (client != it->second && client->getNickname().compare(it->second->getNickname()) == 0)
            return (true);
    }
    return (false);
}

void Client::addChannel(std::string name, Channel *channel)
{
    _channels[channel->getName()] = channel;
    std::cout << "\033[34m" << "Cliente: " << this->_nickname << "\nAdicionou o Channel: " << name << "\033[0m" << std::endl;
}

void Client::removeChannel(std::string name, Channel *channel)
{
    //TODO
    _channels[channel->getName()] = channel;
    std::cout << "\033[34m" << "Cliente: " << this->_nickname << "\nRemoveu o Channel: " << name << "\033[0m" << std::endl;
};