/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:36:54 by edos-san          #+#    #+#             */
/*   Updated: 2023/04/05 22:13:51 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/poll.h>
#include "Server.hpp"
#include "Client.hpp"
#include "Msg.hpp"
#include "Util.hpp"

Server::Server(){
    
}

Server::Server(std::string hostname, int port, std::string password): _password(password)
{
    std::cout << "\x1B[2J\x1B[HServer has been created: " << port << " password: " << password << "\n";
    init(SERVER, hostname, port, 200);

    //Server
    on("PING", &Server::ping);
    on("CAP", &Server::cap);
    on("WHO", &Server::who);
    on("PASS",  &Server::pass);
    on("HELP", &Server::help);
    on("QUIT", &Server::quit);
    on("NOTICE", &Server::notice);

    //Client
    on("PRIVMSG", &Client::msgPrivate);
    on("USERHOST", &Client::userHost);
    on("NICK", &Client::nick);
    on("USER", &Client::user);

    //Channel
    on("JOIN", &Channel::join);
    on("PART", &Channel::leave);
    on("LIST", &Channel::list);
   // on("MODE", &Channel::mode);
    on("KICK", &Channel::kick);
    on("TOPIC", &Channel::topic);




}

void Server::pass(Server *server, Client *client, std::string data)
{
    data = data.substr(1);

    if (server->getPassword() == data)
        client->setPassword(data);
    else
    {
        server->send(client, ERR_PASSWDMISMATCH(client->getNickname()));
        Server::quit(server, client, "");
    }
    if (client->isValid())
        acceptNewConnection(server, client);
}

/*
/quit               quit irc
*/
void Server::quit(Server *server, Client *client, std::string data)
{
    (void)data;
    server->removeClient(client);
    close(client->getFd());
    server->setEvent(client->getIndexFd(), -1, 0, 0);
    delete client;
}

/*
/who [channel]        list of users in channel
*/
void Server::who(Server *server, Client *client, std::string data)
{
    if (!data.empty() && data[0] != '#')
    {
        //:irc.example.com 352 yournick myusername example.com irc.example.com myusername + :0 Real Name
        //:irc.example.com 315 yournick myusername :End of /WHO list.
        server->send(client, ":Teste 352 " + client->getNickname() + " " + client->getUsername() + " " + server->getHostName() + " Teste " + client->getNickname() + " + :0 " + client->getRealname());
        server->send(client, ":Teste 315 " + client->getNickname() + " " + client->getUsername() + " :End of /WHO list.");
    }
    else if (!data.empty() && data[0] == '#')
    {
        Channel *channel = server->getChannel(data);

        if (!channel)
            return ;

        std::vector<Client *>::iterator it;
        
        for (it = channel->getClients().begin(); it != channel->getClients().end(); ++it)
            server->send((*it), ":Teste 352 " + (*it)->getNickname() + " " + data + " " + (*it)->getUsername() + " " + "example.com" + " " + "Teste" + " " + (*it)->getUsername() + " " + "+" + " :0 " + (*it)->getRealname());

        server->send(client, ":Teste 315 " + client->getNickname() + " " + data + " :" + "End of /WHO list.");
    }
}

//The function in bellow will send the message: "PONG :data" read for information here: 4.6.3 Pong message
void Server::ping(Server *server, Client *client, std::string data)
{
    std::string reply = ("PONG :" + data );

    //std::cout << YELLOW "the following message will be sent to the client: ->\t\t" RED << reply + RESET << std::endl;
    server->send(client, reply);
}

//https://ircv3.net/specs/core/capability-negotiation-3.1.html#available-capabilities
void Server::cap(Server *server, Client *client, std::string data)
{
    if (data == "LS 302")
        server->send(client, "CAP * LS :message-tags multi-prefix");
    else if (data == "REQ :multi-prefix")
        server->send(client, "CAP * ACK :multi-prefix");
    else if (data == "END")
    {
        client->setcapend(true);
        if (client->isValid())
            acceptNewConnection(server, client);
    }
}

void Server::notice(Server *server, Client *client, std::string data)
{
    (void)client;

    std::string target = data.substr(0, (data.find(" ")));
    std::string message = ":test NOTICE " + target;
    message += data.substr(data.find(" "), data.size());
    std::cout << "O valor-------->" << message << "\n";
    if (server->getChannels()[target])
        server->getChannels()[target]->send(server, NULL, message);
    else if (server->getClient(target))
        server->send(server->getClient(target), message);
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
void Server::help(Server *server, Client *client, std::string data)
{
    if (data.size() == 0)
        server->send(client, MSH_HELP);
    else
        server->send(client, "HELP LIST COMMAND!\r\n");
}

void Server::connect()
{
    struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	
	int fd_client = accept(_fd,  (struct sockaddr *) &clientAddr, &clientAddrLen);
    if (fd_client < 0)
		return ;
	std::string hostname = inet_ntoa(clientAddr.sin_addr);
    
	std::cout << "IP: " << hostname << std::endl;
	
	for (int i = 1; i < getMaxConnecting(); i++)
    {
        if (getSocket(i).fd == -1)
		{
			setEvent(i, fd_client, POLLIN | POLLHUP);
            addClient(i, new Client(getSocket(i).fd, i, hostname));
			break;
		}
    }
    setEvent(0, getFd(), POLLIN | POLLOUT | POLLHUP);
}

void Server::execute(Client *client, std::string event, std::string data)
{
	function fun = _events[event];
	if (!fun)  
        send(client, ERR_UNKNOWNERROR(data));
    else
        (fun)(this, client, data);
}

void Server::sendAll(Client *client,  std::string data)
{
    std::map<int, Client *>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); it++)
    {
        if ((*it).second != client)
            send((*it).second, data);
    }
}

void Server::send(Client *client, std::string data)
{
    if (!client)
        return ;
   emit(client->getIndexFd(), data + "\r\n");
}

std::string &Server::getPassword(){
	return _password;
}

Channel *Server::addChannel(std::string const channelName, const std::string channelpass)
{
    Channel *channel = getChannel(channelName);
    if (!channel)
        channel = new Channel(channelName, channelpass);
    _channels[channelName] = channel;
    return channel;
}

std::map<std::string, Channel *> &Server::getChannels(){
    return _channels;
}

Channel *Server::getChannel(std::string name){

    std::map<std::string, Channel *>::iterator it;

    it = _channels.begin();
    for (it = _channels.begin(); it != _channels.end(); it++)
    {
        if(it->first == name)
            return (it->second);
    }
    return NULL;
}

void Server::acceptNewConnection(Server *server, Client *client)
{
    server->send(client, RPL_WELCOME(client->getNickname()));
    Channel::join(server, client, "#public");
};

Server::~Server()
{
    std::cout << "~Server\n";
}
