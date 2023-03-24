/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 12:46:22 by edos-san          #+#    #+#             */
/*   Updated: 2022/12/10 10:13:17 by edos-san         ###   ########.fr       */
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
    if (data.empty() || client->getChannel())
        server->send(client, MSG_COMMAND_INVALID);
    else
    {
        Channel *channel = server->getChannels()[data];
        if (!channel)
        {
            channel = new Channel(data);
            server->getChannels()[data] = channel;
        }
        channel->add(client);
        server->send(client, channel->getClients(), "\rUser: " + client->getNickname() + " in the room\n", YELLOW);
    }
}