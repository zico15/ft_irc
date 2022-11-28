/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:36:54 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/28 00:08:54 by edos-san         ###   ########.fr       */
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
        if (socket->getSocket(i).fd == -1)
		{
            const std::string a = "as";
			socket->setEvent(i, fd_client, POLLIN);
            socket->emit(i, socket->console.getOut());            
			std::cout << "new client: " << socket->getSocket(i).fd << "\n"; 
			break;
		}
    }
    socket->setEvent(0, socket->getFd(), POLLIN | POLLOUT);
}

static void msg(void *data)
{
    std::cout << (char *) data << "\n";
    
}

Socket	*Server::getSocket(){
    return (_socket);
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
