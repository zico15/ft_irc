/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 12:46:22 by edos-san          #+#    #+#             */
/*   Updated: 2023/03/27 02:03:38 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(std::string	channel): _channel(channel)
{
    std::cout << "\033[35mNew Channel: " << channel << "\033[0m" << std::endl;
}

Channel::~Channel()
{
    
}

void Channel::add(Client *client){

    _clients.push_back(client);
    //client->setChannel(this);
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

bool Channel::isInTheChannel(Client *client)
{
    std::vector<Client *>::iterator it;
    
    for (it = _clients.begin(); it < _clients.end(); ++it)
    {
        if (client == *it)
            return true;
    }
    return false;
}

void Channel::sendMsgForAll(Server *server, Client *client, std::string message)
{
    std::vector<Client *>::iterator it;
    
    for (it = _clients.begin(); it < _clients.end(); ++it)
    {
        if (client != *it)
            server->send((*it), message);
    }
}

//:irc.server.com 322 client_nick #channel :*no topic
void Channel::list(Server *server, Client *client)
{
    std::ostringstream stream;
    stream << this->_clients.size();
    std::string clientsNmbr = stream.str();

    server->send(client, LIST_MID(client->getNickname(), this->getName(), clientsNmbr));
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
    

    if (data.empty())
        server->send(client, MSG_COMMAND_INVALID);
    else
    {
        Channel *channel = server->getChannels()[data];
        if (!channel)
            channel = server->addChannel(data);
        
        std::vector<Client *> clientss = channel->getClients();

        if (!clientss.empty())
        {
            std::string message = "The " + client->getNickname() + " have joined the channel!\r\n";

            //TODO
            //std::cout << "\n\nENVIO DE MENSAGEM\n\n" << message << std::endl;
            
            channel->sendMsgForAll(server, client, message);
        }

        channel->add(client);
        //client->addChannel(data, channel);
        
        server->send(client, JOIN_CHANNEL(nick,user,server->getHostName(),canal), YELLOW);

        // --- Notes ---
        //:nick!user@host JOIN #canal
        //std::string message = ":" + nick + "!" + user + "@" + server->getHostName() + " JOIN " + canal;
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

/*leave [channel]     leave channel*/
void Channel::leave(Server *server, Client *client, String data)
{
    /*
        data: #chanel1 :chanel1 or #chanel1 :Konversation terminated!
                transform
        PART #test\r\n
    */

    std::cout << "\033[35mLEAVE: " << client->getNickname() << "\033[0m" << std::endl;

    if (data.empty())
    {
        server->send(client, MSG_COMMAND_INVALID);
        return ;
    }
    
    std::string canal = data.substr(0, data.find(":")-1);

    Channel *channel = server->getChannels()[canal];
    
    if (!channel || !channel->isInTheChannel(client)) //Is not in the channel or the channel dont exist
        return ;
    
    channel->remove(client);
    //Don't need client -> channel

    server->send(client, LEAVE_CHANNEL(canal));
}

std::ostream& operator<<(std::ostream& os, Channel *channel)
{
	/*std::vector<Client *> clients = channel->getClients();
	for (size_t i = 0; i < clients.size(); i++)
	{*/
	//}
    return os;
}
