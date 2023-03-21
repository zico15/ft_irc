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

Client::Client()
{
    return ;
}

Client::Client(int fd, int index)
{
    _fd = fd;
    _index_fd = index;
    _username = "Guest_" + std::to_string(id_clinet++);
    _nickname = _username;
    std::cout << MSG_NEW_CLIENT(_username, std::to_string(fd));
}

Client::~Client() 
{
    std::cout << MSG_CLOSE_CLIENT(_username, std::to_string(_fd));
}

void Client::setNickname(std::string nickname){
	_nickname = trim(nickname);
	//std::cout << "Client: " << _username << " set nickname: " << _nickname << std::endl;
}


