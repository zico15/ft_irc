/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:36:54 by edos-san          #+#    #+#             */
/*   Updated: 2022/12/09 22:57:24 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){}

Server::Server(std::string hostname, int port)
{
    std::cout << "Server has been created: " << port << "\n";
    init(SERVER, hostname, port, 200);
    on("connect",  &Server::connect);
    _function_default =  &Server::msg;
    on("/help", &Server::help);
    on("/nick", &Server::nick);
    on("/join", &Server::join);
    on("/leave", &Server::leave);
}

/*nick [login]       change your login*/
void Server::nick(Client *client, String data)
{
    if (data.empty())
    {   
        std::cout << "Nick invalido\n";
        send(client, "Nick invalido\n");
    }
    else
        client->setNickname(data);
    response(client);
}

void Server::leave(Client *client, String data)
{
    if (client->getChannel())
        client->getChannel()->remove(client);
    client->setChannel(NULL);
    response(client);
}

/*join [channel]     join channel*/
void Server::join(Client *client, String data)
{
    if (data.empty())
        send(client, "Jon invalido\n");
    else
    {
        Channel *channel = _channels[data];
        if (!channel)
        {
            channel = new Channel(data);
            _channels[data] = channel; 
        }
        channel->add(client);
        send(client, "Channel: " + channel->getChannel() + "\n");
        std::cout << channel;
    }
    response(client);
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
void Server::help(Client *client, String data)
{
    send(client, "/nick [login]       change your login\n/join [channel]     join channel\n/leave              leave current channel\n");
    response(client);
}

void Server::connect(Client *client, String data)
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
            send(_clients[i], "Welcome: " + _clients[i]->getUsername() + "\n");
            response(_clients[i]);
			break;
		}
    }
    setEvent(0, getFd(), POLLIN | POLLOUT | POLLHUP);
}

void Server::execute(Client *client, std::string event, String data)
{
	function fun = _events[event];
	if (!fun)
    {    
        fun = _function_default;
        data = event + " " + data;
    }
    (this->*fun)(client, data);
}

void Server::msg(Client *client, String data)
{
    Channel *channel = client->getChannel();
   if (channel)
   {
        std::vector<Client *> clients = channel->getClients();
        for (size_t i = 0; i < clients.size(); i++)
        {
           if (clients[i] != client)
           {
                send(clients[i], "\r[public: "+client->getNickname()+"] ", MAGENTA);
                send(clients[i], data + "\n");
                response(clients[i]);
           }
        }
   }
   response(client);
}

void Server::send(Client *client, std::string data, std::string color)
{
    if (!client)
        return ;
   emit(client->getFd(), color + data + "\033[0m");
}

void Server::response(Client *client){
    if (client->getChannel())
        send(client, "[" + client->getNickname() + "]  ", GREEN);
    else
        send(client, "[" + client->getUsername() + "]  ", GREEN);  
}

Server::~Server()
{
    std::cout << "~Server\n";
}
