/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 22:40:38 by edos-san          #+#    #+#             */
/*   Updated: 2023/04/01 21:58:18 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Msg.hpp"
#include "Util.hpp"

static	int		id_clinet = 1;	

Client::Client(): _isConnect(false)
{
    return ;
}

Client::Client(int fd, int index): _isConnect(false)
{
    _fd = fd;
    _index_fd = index;
    _username = "";
    _nickname = "";
    _password = "";
    _capend = false;
    _isConnect = false;
    std::cout << MSG_NEW_CLIENT(std::string(""), std::to_string(fd));
}

Client::~Client() 
{
    std::cout << MSG_CLOSE_CLIENT(_username, std::to_string(_fd));
    std::map<std::string, Channel *>::iterator it;
    
    for (it = _channels.begin(); it != _channels.end(); it++)
    {
        it->second->remove(this);
    }
    
}

bool Client::isValid()
{
    this->_isConnect = !(_username.empty() || _nickname.empty() || _password.empty() || !_capend);

    return _isConnect;
}

void Client::setNickname(const std::string& nickname){
	_nickname = trim(nickname);
	//std::cout << "Client: " << _username << " set nickname: " << _nickname << std::endl;
}

void Client::nick(Server *server, Client *client, std::string data)
{
    client->setNickname (data);
    if (Client::isNickname(server->getClients(), client))
    {   
        server->send(client, NICKNAME_ERROR(client->getNickname()));
        client->setNickname("");
    }
    else if (client->isValid())
        server->acceptNewConnection(server, client);
}

void Client::user(Server *server, Client *client, std::string data)
{
    client->setUsername(data.substr(0, data.find(' ')));
    client->setRealname(data.substr(data.find(':') + 1));
    if (client->isValid())
        server->acceptNewConnection(server, client);
}

bool Client::isNickname(std::map<int, Client *> clients, Client *client)
{
    std::map<int, Client *>::iterator it;

    for (it = clients.begin(); it != clients.end(); it++)
    {
        if (client != it->second && client->getNickname().compare(it->second->getNickname()) == 0)
            return (true);
    }
    return (false);
}

void Client::addChannel(Channel *channel)
{
    _channels[channel->getName()] = channel;
    std::cout << "\033[34m" << "Cliente: " << this->_nickname << "\nAdicionou o Channel: " << channel->getName() << "\033[0m" << std::endl;
}

void Client::removeChannel(Channel *channel)
{
    //TODO
    _channels.erase(channel->getName());
    std::cout << "\033[34m" << "Cliente: " << this->_nickname << "\nRemoveu o Channel: " << channel->getName() << "\033[0m" << std::endl;
};


void Client::msgPrivate(Server *server, Client *client, std::string data)
{
    //PRIVMSG <nick> :<mensagem>
    //:nick!user@host PRIVMSG (dest)nick
    //:nick!user@host PRIVMSG #channel

    std::string user = client->getUsername();
    std::string nick = client->getNickname();
    std::string host = server->getHostName();
    std::string dest;
    Client  *client_dest;

    dest = data.substr(0, data.find(' '));

    if (dest.empty())
        return ;

    client_dest = server->getClient(dest);
    std::string message = data.substr(data.find(":"), data.size());

    if (client_dest) //Dest is a Client
    {
        message = PRV_MSG(nick, user, host, client_dest->getNickname(), message);
        
        server->send(client_dest, message); 
    }
    else if (dest.find("#") == 0) //Dest is a Channel
    {
        message = PRV_MSG(nick, user, host, dest, message);
        
        Channel *channel = server->getChannel(dest);
        
        if (channel)
        {
            std::vector<Client *> clients = channel->getClients();
            std::vector<Client *>::iterator it;
        
            if (channel->isInTheChannel(client) == false) //Client is not in the Server
                return ;
            channel->send(server, client, message);
        }
    }
}

void Client::userHost(Server *server, Client *client, std::string data)
{
    server->send(client, RPL_USERHOST(data));
}