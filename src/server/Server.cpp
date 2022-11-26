/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:36:54 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/25 23:50:15 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){}

Server::Server(std::string hostname, int port)
{
    std::cout << "Server has been created: " << port << "\n";
    _socket = new Socket(SERVER, hostname, port, 200);
}

void Server::run()
{
    _socket->run();
}

Server::~Server()
{
    std::cout << "~Server\n";
    delete _socket;
}
