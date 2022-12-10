/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 22:40:38 by edos-san          #+#    #+#             */
/*   Updated: 2022/12/09 18:33:03 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

static	int		id_clinet = 1;	

Client::Client()
{
    return ;
}

Client::Client(int fd)
{
    _fd = fd;
    _username = "Guest_" + std::to_string(id_clinet++);
    _nickname = _username;
    std::cout << "new client: " << _username << " fd: " << fd << "\n";
}

Client::~Client() 
{
    
     
}

void Client::setNickname(std::string nickname){
	_nickname = trim(nickname);
	std::cout << "Client: " << _username << " set nickname: " << _nickname << std::endl;
}


