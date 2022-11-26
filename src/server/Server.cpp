/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:36:54 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/26 07:07:02 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static void connect(void *data);
static void msg(void *data);

Server::Server(){}

Server::Server(std::string hostname, int port)
{
    std::cout << "Server has been created: " << port << "\n";
    _socket = new Socket(SERVER, hostname, port, 200);
    _socket->on("connect",  connect);
    _socket->on("msg",  msg);
}

static void connect(void *data)
{
    Socket *socket = ((Socket *)data);

    int fd_client = socket->socketAccept();
	if (fd_client < 0)
		return ;
	for (size_t i = 1; i < socket->getMaxConnecting(); i++)
    {
        if (socket->getSocket(i).fd == 0)
		{
            
            
			socket->setEvent(i, fd_client, POLLIN | POLLOUT);
            socket->emit(i, "conect: ok");            
			std::cout << "new client: " << socket->getSocket(i).fd << "\n";
            sleep(1);
             socket->emit(i, "asdasdsda: ok");   
			break;
		}
    }
}

static void msg(void *data)
{
    std::cout << (char *) data << "\n";
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
