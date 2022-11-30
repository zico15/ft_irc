/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 22:40:38 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/30 19:16:15 by edos-san         ###   ########.fr       */
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
    std::cout << "new client: " << _username << " fd: " << fd << "\n"; 
}

Client::~Client() 
{
    
     
}


