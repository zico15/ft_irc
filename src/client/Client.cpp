/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 22:40:38 by edos-san          #+#    #+#             */
/*   Updated: 2022/12/11 15:33:29 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

static	int		id_clinet = 1;	

Client::Client(): isConnect(false)
{
    return ;
}

Client::Client(int fd, int index): isConnect(false)
{
    _fd = fd;
    _index_fd = index;
    _username = "";
    _nickname = "";
    _password = "";
    std::cout << MSG_NEW_CLIENT(std::string("Tes"), std::to_string(fd));
}

Client::~Client() 
{
    std::cout << MSG_CLOSE_CLIENT(_username, std::to_string(_fd));
}

bool Client::isValid()
{
    return !(_username.empty() || _nickname.empty() || _password.empty());
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
        std::cout << "NICK:" << it->second->getNickname() << " | " << client->getNickname().compare(it->second->getNickname())<< "\n";
        if (client != it->second && client->getNickname().compare(it->second->getNickname()) == 0)
            return (true);
    }
    return (false);
}


