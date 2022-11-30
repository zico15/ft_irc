/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:36:54 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/30 19:28:53 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"



Server::Server(){}

Server::Server(std::string hostname, int port)
{
    
    std::cout << "Server has been created: " << port << "\n";
    init(SERVER, hostname, port, 200);
    on("connect",  &Server::connect);
    on("msg",  &Server::msg);
    on("/help", &Server::help);
}


/*
/nick [login]       change your login
/join [channel]     join channel
/leave              leave current channel
/quit               quit irc
/who                list of users in channel
/msg [login] [msg]  submit msg at login
/list               list of channel
/me                 defined an action
[msg]               send msg in channel
*/
void Server::help(void *data)
{
    Client *client = (Client *) data;
    emit(client->getFd(), "/nick [login]       change your login\n/join [channel]     join channel\n");
}

void Server::connect(void *data)
{
    int fd_client = socketAccept();
	if (fd_client < 0)
		return ;
	for (size_t i = 1; i < getMaxConnecting(); i++)
    {
        if (getSocket(i).fd == -1)
		{
			setEvent(i, fd_client, POLLIN | POLLHUP);
            _clients[i] = new Client(i);
            emit(i, "Welcome: " + _clients[i]->getUsername());
			break;
		}
    }
    setEvent(0, getFd(), POLLIN | POLLOUT | POLLHUP);
}

void Server::execute(std::string event, void *d)
{
	function fun = _events[event];
	if (fun)
	  (this->*fun)(d);      
}

void Server::msg(void *data)
{
    std::cout << (char *) data << "\n";
    
}



Server::~Server()
{
    std::cout << "~Server\n";
}
