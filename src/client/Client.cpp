/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 22:40:38 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/25 22:30:29 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
    return ;
}

Client::Client(std::string hostname, int port)
{
    _socket = new Socket(CLIENT, hostname, port, 2);
}

Client::~Client() 
{
    
     
}


bool Client::run()
{
    std::string line;

    std::cout << "line:, " << line << "\n";
    std::getline (std::cin, line);    
    if (line.size() > 0)
    {    
        line += "\r";
        send(_socket->getFd(), line.c_str(), line.size(), 0);
        std::cout << "line:, " << line << "\n";
    }
    
    return (true);
}
