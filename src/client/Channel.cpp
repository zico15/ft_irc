/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 12:46:22 by edos-san          #+#    #+#             */
/*   Updated: 2023/03/27 22:56:00 by rteles           ###   ########.fr       */
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

void Channel::add(Client *client, Server *server){

    _clients.push_back(client);
    //client->setChannel(this);
	std::cout << "Channel: " << _channel  << " add client: " << client->getNickname() << std::endl;

    server->send(client, RPL_JOIN(client->getNickname(), client->getUsername(),server->getHostName(), this->_channel));
    server->send(client, RPL_NAMREPLY(client, server, this->_channel));
    server->send(client, RPL_ENDOFNAMES(client->getNickname(), this->_channel));
    //TODO
    if (!this->getClients().empty())
        this->sendMsgForAll(server, client, "The " + client->getNickname() + " have joined the channel!\r\n");
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

//Returns a string list of nicks inside 
std::string Channel::nicksOnChannel(void)
{
    std::string nameslist;

    for (int i = 0; i < _clients.size(); i++) {
        nameslist += _clients[i]->getNickname();
        if (i + 1 < _clients.size())
            nameslist += " ";
    }
    return nameslist;
}
//TODO NOT WORKING
void Channel::who(Server *server, Client *client)
{
    std::vector<Client *>::iterator it;
    std::string                     message;
    std::string                     status;

    for (it = _clients.begin(); it < _clients.end(); ++it)
    {
        status = (*it)->isConnect() ? "H" : "*";
        
        message = RPL_WHOREPLY(client->getNickname(), this->_channel, (*it)->getNickname(), server->getHostName(), status, (*it)->getRealname());
        std::cout << message << std::endl;
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
    std::string channel = data.substr(0, data.find(' '));
    //std::string channelpass = data.substr(data.find(' '), data.size());

    Channel *svChannel = server->getChannels()[channel];
    
    if (!svChannel)
        svChannel = server->addChannel(channel);
    
    if (svChannel->isInTheChannel(client))
        return ;

    svChannel->add(client, server);
    //client->addChannel(channel, channel);
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
