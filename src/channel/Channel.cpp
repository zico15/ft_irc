/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 12:46:22 by edos-san          #+#    #+#             */
/*   Updated: 2023/04/05 22:09:25 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "Channel.hpp"
#include "Server.hpp"
#include "Msg.hpp"
#include "Client.hpp"
#include "Util.hpp"

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

    std::string nickname = client->getNickname();
    _clients.push_back(client);
	std::cout << "\033[35mChannel: " << _channel  << " add client: " << nickname << "\033[0m" << std::endl;
    
    //TODO 
    //server->send(client, client->getPrefix() + " JOIN " + this->_channel);
    
    server->send(client, RPL_JOIN(nickname, client->getUsername(),server->getHostName(), this->_channel));
    
    for (long unsigned int i = 0; i != this->getClients().size(); i++)
    {
        server->send(getClients()[i], RPL_NAMREPLY(getClients()[i], server, this));
        server->send(getClients()[i], RPL_ENDOFNAMES(getClients()[i]->getNickname(), this));
    }
    client->addChannel(this);
    if (!this->getClients().empty())
        this->send(server, client, "JOIN " + nickname + " have joined the channel!\r\n");
}

/*join [channel]     join channel*/
void Channel::join(Server *server, Client *client, std::string data)
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
    
    if (data.find(' ') != data.npos)
        channelpass = data.substr(data.find(' '), data.size());

    Channel *svChannel = server->getChannel(channelname);
    
    if (!svChannel) {
        svChannel = server->addChannel(channelname, channelpass);
        svChannel->_op.push_back(client->getNickname());
    }
    
    if (svChannel->isInTheChannel(client))
        return ;

    if (server->getChannel(channelname)->getpass().empty() || server->getChannel(channelname)->getpass() == channelpass)
        svChannel->add(client, server);
    else
        server->send(client, ERR_BADCHANNELKEY(client->getNickname(), channelname));
}

void Channel::remove(Client *client){
    std::vector<Client *>::iterator it;
    
    std::cout << " TES: " << " client: " << client->getNickname() << "\n";
    // Printing the Vector
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (client == *it)
        {

            _clients.erase(it);
            rmOp(*it);
            std::cout << "Channel: " << _channel  << " remove client: " << client->getNickname() << std::endl;
            return ;
        }
    }
}

std::string Channel::getName()
{
  return (_channel);
}

std::vector<Client *> &Channel::getClients()
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

std::string Channel::getTopic() const
{
    return this->_topic;
}

void Channel::setTopic(std::string topic)
{
    this->_topic = topic;
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

bool Channel::isOp(std::string nickname)
{
    std::vector<std::string>::iterator it;

    for (it = _op.begin(); it != _op.end(); it++) {
        if (nickname == (*it)) {
            return true;
        }
    }
    return false;
}

void Channel::send(Server *server, Client *client, std::string message)
{
    std::vector<Client *>::iterator it;
    
    for (it = _clients.begin(); it < _clients.end(); ++it)
    {
        if (client != *it)
            server->send((*it), message);
    }
}

void Channel::rmOp(Client *client) {
    std::vector<std::string>::iterator  it;

    for (it = _op.begin(); it != _op.end(); it++) {
        if ((*it) == client->getNickname()) {
            (*it).erase();
            return ;
        }
    }
}

//Returns a string list of nicks inside 
std::string Channel::nicksOnChannel(void)
{
    std::string nameslist;

    for (long unsigned int i = 0; i < _clients.size(); i++)
    {
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

void Channel::mode(Server *server, Client *client, std::string data)
{
    //server->send(client, ":teste MODE " + data + " " + client->getNickname());
    (void)server;
    (void)client;
    (void)data;
    if (data.empty())
        return ;
    if (data.find("+o")) {
        std::cout << "Channel operator added: " + data.substr(data.find("+") + 3) + "\n";
        server->getChannel(data.substr(0, data.find(' ') - 1))->_op.push_back(data.substr(data.find("+") + 3));
    }
    return ;
}

void Channel::kick(Server *server, Client *client, std::string data)
{
    //value:       #public Nickname_edu :User terminated!
    //:Nickname_op KICK #public Nickname_edu :User terminated!
    
    std::string     channelName = data.substr(0, data.find(' '));
       
    Channel *channel = server->getChannel(channelName);

    if (!channel || !channel->getClients().size())
        return ;

    if (!channel->isOp(client->getNickname()))
    {
        //:yourserver 482 clientnickname #public :You're not channel operator
        server->send(client, ":TESTE 482 " + client->getNickname() + " " + channelName + " :You're not channel operator");
        return ;
    }

    std::string     nickban = data.substr(data.find(' ') + 1, data.find(':') - data.find(' ') - 1);
    nickban = nickban.substr(0, nickban.find_last_not_of(' ') + 1);
    std::string     reasons = data.substr(data.find(":"), data.size());
        
    std::vector<Client *>::iterator it;
    Client * banClient = NULL;
    
    if (channel->isOp(nickban)) {
        server->send(client, ":TESTE 482 " + client->getNickname() + " " + channelName + " :You cannot kick a channel Operator.");
        return ;
    }

    for (it = channel->getClients().begin(); it != channel->getClients().end(); ++it)
    {
        server->send((*it), ":" + (*it)->getNickname() + " KICK " + channelName + " " + nickban + " " + reasons);
        
        if (nickban == (*it)->getNickname())
            banClient = *it;
    }
    
    if (banClient)
        channel->remove(banClient);
}

void Channel::topic(Server *server, Client *client, std::string data)
{
    std::string     channelname = data.substr(0, data.find(' '));
        std::cout << "Vai ser enviado um topic\n";

    Channel *channel = server->getChannel(channelname);

    if (!channel)
        return ;

    if (client->getNickname() != channel->getClients()[0]->getNickname())
    {
        server->send(client, ":TESTE 482 " + client->getNickname() + " " + channelname + " :You're not channel operator");
        return ;
    }
        std::cout << "Vai ser enviado um topic\n";

    if (data.find(":") == data.npos) {
        server->send(client, RPL_SYNTAXERROR("/TOPIC you have to add a channel topic as a parameter!"));
        return ;
    }
    std::string topic = data.substr(data.find(":") + 1);
    server->getChannels()[channelname]->setTopic(topic);
}

//:irc.server.com 322 client_nick #channel :*no topic
void Channel::list(Server *server, Client *client, std::string data)
{
    (void)data;
    std::ostringstream stream;
    stream << server->getChannels().size();
    server->send(client, LIST_START(client->getNickname(), stream.str()));
    std::map<std::string, Channel *>::iterator it;
	for (it = server->getChannels().begin(); it != server->getChannels().end(); ++it)
	{
        if (!it->second)
            continue;
        std::ostringstream stream;
        stream << it->second->getClients().size();
        server->send(client, LIST_MID(client->getNickname(), it->second, stream.str()));
	}
	server->send(client, LIST_END(client->getNickname()));
}

/*leave [channel]     leave channel*/
void Channel::leave(Server *server, Client *client, std::string data)
{
    std::cout << "\033[35mLEAVE: " << client->getNickname() << "\033[0m" << std::endl;

    if (data.empty())
    {
        server->send(client, MSG_COMMAND_INVALID);
        return ;
    }
    
    std::string canal = data.substr(0, data.find(":")-1);

    Channel *channel = server->getChannel(canal);
    
    if (!channel || !channel->isInTheChannel(client)) //Is not in the channel or the channel dont exist
        return ;

    channel->send(server, NULL, LEAVE_CHANNEL(canal, client));
    
    channel->remove(client);
    client->removeChannel(channel);
}
