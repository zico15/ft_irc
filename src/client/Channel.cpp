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

Channel::Channel(std::string	channel): _channelName(channel)
{
    std::cout << "New Channel: " << channel << std::endl;
}

Channel::~Channel()
{
    
}

void Channel::add(Client *client){

    _clients.push_back(client);
    client->setChannel(this);
    std::cout << "Adicionado: " << client->getNickname() << std::endl;
}

void Channel::remove(Client *client){
    std::vector<Client *>::iterator it;
    
    // Printing the Vector
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (client == *it)
        {
            _clients.erase(it);
            std::cout << "Channel: " << _channelName  << " remove client: " << client->getNickname() << std::endl;
            return ;
        }
    }
}

std::string Channel::getName()
{
  return (_channelName);
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
/*void Channel::join(Server *server, Client *client, String data)
{
    //JOIN #canal
    //
    //transformar
    //
    //:nick!user@host JOIN #canal
    //:nome_servidor 353 nick = #canal :@nick1 +nick2
    std::string canal = data;
    std::string nick = client->getNickname();


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


        //:nick!user@host JOIN #canal
//        std::string message = ":test JOIN #" + canal + "\r\n";
        std::string message = ":" + nick + "!user@" + server->getHostName() + " JOIN " + data + "\r\n";
        send(client->getFd(), message.c_str(), message.size(), 0);

        //:nome_servidor 353 nick = #canal :@nick1 +nick2
        // message = ":test 353 " + nick + " = #" + canal + " :@" + nick + "\r\n";
        // send(client->getFd(), message.c_str(), message.size(), 0);

        // //:nome_servidor 366 nick
        // message = ":test 366 " + nick + " #" + canal + " :End of /NAMES list\r\n";
        // send(client->getFd(), message.c_str(), message.size(), 0);

        //server->send(client, channel->getClients(), "\rUser: " + client->getNickname() + " in the room\n", YELLOW);
    }
}*/

/*join [channel]     join channel*/
void Channel::join(Server *server, Client *client, String data)
{
    std::string channelname = data.substr(0, data.find(' '));
    //std::string channelpass = data.substr(data.find(' '), data.size());

    if (server->isChannel(data.substr(0, data.find(' ')))) {
        //if data is a channel, just add client in channel and advice all the new user inside channel
        server->addClientToChannel(server, client, channelname);
        
        server->send(client, ":" + client->getNickname() + "!"+client->getUsername()+"@" + SERVER_NAME + " JOIN " + data.substr(0, data.find(' ')));
        server->send(client, ":Teste 353 " + client->getNickname() + " =" + " :@" + client->getNickname());
        server->send(client, ":Teste 366 " + client->getNickname() + " " + data.substr(0, data.find(' ')) + " :End of /NAMES list");
        return ;
    }
    else {
        //Create channel here
        server->addChannel(channelname);
        server->addClientToChannel(server, client, channelname);
        
        server->send(client, ":" + client->getNickname() + "!"+client->getUsername()+"@" + SERVER_NAME + " JOIN " + data.substr(0, data.find(' ')));
        server->send(client, ":Teste 353 " + client->getNickname() + " =" + " :@" + client->getNickname());
        server->send(client, ":Teste 366 " + client->getNickname() + " " + data.substr(0, data.find(' ')) + " :End of /NAMES list");
        return ;
    }
}

void Channel::part(Server *server, Client *client, String data)
{
    
}

void Channel::sendmessage(Server *server, Client *client, String data)
{
    std::vector<Client*>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); it++) {
        server->send((*it), data);
        std::cout << "Madou menssagem para: " << client->getNickname() << std::endl;
    }
}
