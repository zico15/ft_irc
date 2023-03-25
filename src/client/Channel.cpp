/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 12:46:22 by edos-san          #+#    #+#             */
/*   Updated: 2023/03/25 19:50:56 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(std::string	channel): _channel(channel)
{
    std::cout << "New Channel: " << channel << std::endl;
}

Channel::~Channel()
{
    
}

void Channel::add(Client *client){

    _clients.push_back(client);
    client->setChannel(this);
	std::cout << "Channel: " << _channel  << " add client: " << client->getNickname() << std::endl;
}

void Channel::remove(Client *client){
    std::vector<Client *>::iterator it;
    
    // Printing the Vector
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (client == *it)
        {
            _clients.erase(it);
            std::cout << "Channel: " << _channel  << " remove client: " << client->getNickname() << std::endl;
            return ;
        }
    }
}

std::string Channel::getName()
{
  return (_channel);
}

std::vector<Client *> Channel::getClients()
{
    return _clients;
}

size_t Channel::getSize()
{
    return (_clients.size());
}

std::ostream& operator<<(std::ostream& os, Channel *channel)
{
	/*std::vector<Client *> clients = channel->getClients();
	for (size_t i = 0; i < clients.size(); i++)
	{*/
	//}
    return os;
}


/*join [channel]     join channel*/
void Channel::join(Server *server, Client *client, String data)
{
    //JOIN #canal
    //
    //transformar
    //
    //:nick!user@host JOIN #canal

    std::string canal = data;
    std::string nick = client->getNickname();
    std::string user = client->getUsername();
    

    if (data.empty() || client->getChannel())
        server->send(client, MSG_COMMAND_INVALID);
    else
    {
        std::cout << "Nome do channel: " << data << std::endl;
        Channel *channel = server->getChannels()[data];
        if (!channel)
            channel = server->addChannel(data);
        channel->add(client);
        client->addChannel(data, channel);

        //:nick!user@host JOIN #canal
        std::string message = ":" + nick + "!" + user + "@" + server->getHostName() + " JOIN " + canal;
        server->send(client, message, YELLOW);
        //send(client->getFd(), message.c_str(), message.size(), 0);

        /* Lista de Usuarios no Server
        //:nome_servidor 353 nick = #canal :@nick1 +nick2
        message = ":test 353 " + nick + " = #" + canal + " :@nick1 +nick2\r\n";
        send(client->getFd(), message.c_str(), message.size(), 0);
        
        //:nome_servidor 366 nick
        message = ":test 366 " + nick + " #" + canal + " :End of /NAMES list\r\n";
        send(client->getFd(), message.c_str(), message.size(), 0);*/
        
        //server->send(client, channel->getClients(), "\rUser: " + client->getNickname() + " in the room\n", YELLOW);
    }
}
