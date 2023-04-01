/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 12:46:22 by edos-san          #+#    #+#             */
/*   Updated: 2023/03/28 09:48:23 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(std::string	channel): _channel(channel), _pass("")
{
    std::cout << "\033[35mNew Channel: " << channel << "\033[0m" << std::endl;
}

Channel::Channel(std::string channel, std::string channelpass): _channel(channel), _pass(channelpass)
{
    std::cout << "\033[35mNew Channel: " << channel << "\033[0m" << std::endl;
}

Channel::~Channel()
{
    
}

void Channel::add(Client *client, Server *server) {
    std::vector<Client *>::iterator it;

    std::string nickname = client->getNickname();
    for (it = _clients.begin(); it != _clients.end(); it++) {
        if (*it == client) {
            std::cout << "ERRR o cliente ja foi adicionado ao channel!\n";
            return;
        }
    }
    _clients.push_back(client);
	std::cout << "\033[35mChannel: " << _channel  << " add client: " << nickname << "\033[0m" << std::endl;

    server->send(client, RPL_JOIN(nickname, client->getUsername(),server->getHostName(), this->_channel));
    server->send(client, RPL_NAMREPLY(client, server, this));
    server->send(client, RPL_ENDOFNAMES(nickname, this));
    if (!this->getClients().empty())
        this->sendMsgForAll(server, client, "JOIN " + nickname + " have joined the channel!\r\n");
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

std::string Channel::getpass()
{
    return _pass;
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

void Channel::mode(Server *server, Client *client, String data)
{
    server->send(client, ":teste MODE " + data + " " + client->getNickname());
}

void Channel::kick(Server *server, Client *client, String data)
{
    std::string final = data.substr(0, data.find(" :"));

    final = "KICK " + final + " :PORQUE EU QUERO!";
    
    std::cout << final << std::endl;

    server->getChannels()["#public"]->sendMsgForAll(server, NULL, final);

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
    std::string channelname;
    std::string channelpass = "";

    if (data.empty()){
        server->send(client, RPL_SYNTAXERROR("Missing arguments."));
        return ;
    }
    channelname = data.substr(0, data.find(' '));
    if (channelname[0] != '#') {
        server->send(client, RPL_SYNTAXERROR("Channels name must start with '#'."));
        return ;
    }
    std::cout << "Teste 1\n";

    if (data.find(' ') != data.npos)
        channelpass = data.substr(data.find(' '), data.size());

    std::cout << "Teste 2\n";

    Channel *svChannel = server->getChannels()[channelname];
    std::cout << "Teste 3\n";
    
    if (!svChannel)
        svChannel = server->addChannel(channelname, channelpass);
    std::cout << "Teste 4\n";
    
    if (svChannel->isInTheChannel(client))
        return ;
    std::cout << "Teste 5\n";

    if (server->getChannels()[channelname]->getpass().empty() || server->getChannels()[channelname]->getpass() == channelpass)
        svChannel->add(client, server);
    else
        server->send(client, ERR_BADCHANNELKEY(client->getNickname(), channelname));
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
