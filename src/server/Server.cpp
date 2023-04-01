/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:36:54 by edos-san          #+#    #+#             */
/*   Updated: 2023/04/01 22:07:51 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Msg.hpp"

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

    //Client
    on("PRIVMSG", &Client::msgPrivate);
    on("USERHOST", &Client::userHost);
    on("NICK", &Client::nick);
    on("USER", &Client::user);

    //Channel
    on("JOIN", &Channel::join);
    on("PART", &Channel::leave);
    on("LIST", &Channel::list);
    on("MODE", &Channel::mode);
    on("KICK", &Channel::kick);

}



void Server::pass(Server *server, Client *client, std::string data)
{
    data = data.substr(1);

    if (server->getPassword() == data)
        client->setPassword(data);
    else
        server->send(client, ERR_PASSWDMISMATCH(client->getNickname()));
    if (client->isValid())
        acceptNewConnection(server, client);
}

/*
/leave              leave current channel
*/
void Server::leave(Server *server, Client *client, std::string data)
{

}

/*
/quit               quit irc
*/
void Server::quit(Server *server, Client *client, std::string data)
{
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
	/*if (data.empty())
	{
		return ;
	}
	
	if (data.find("#") == std::string::npos)
		data = "#" + data;

	Channel *channel = server->getChannels()[data];
	if (!channel)
		return ;

    std::vector<Client *> clients = channel->getClients();
    
    channel->who(server, client);*/
    
    //falta criar uma funcao que faz send para o client todos os users e respetivas salas onde estão dentro, ver MSG.hpp para ver implementação
    server->send(client, RPL_ENDOFWHO(client));
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
    int fd_client = socketAccept();
	if (fd_client < 0)
		return ;
	for (size_t i = 1; i < getMaxConnecting(); i++)
    {
        if (getSocket(i).fd == -1)
		{
			setEvent(i, fd_client, POLLIN | POLLHUP);
            addClient(i, new Client(getSocket(i).fd, i));
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



void Server::send(Client *client, std::vector<Client *> clients, std::string data)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i] != client)
            send(clients[i], data);
    }
}

void Server::send(Client *client, std::string data)
{
    if (!client)
        return ;
   emit(client->getIndexFd(), data + "\r\n");
}

Channel *Server::addChannel(std::string const channelName, const std::string channelpass)
{
    if (!_channels[channelName])
        _channels[channelName] = new Channel(channelName, channelpass);   
    return _channels[channelName];
}

std::string &Server::getPassword(){
	return _password;
}
std::map<std::string, Channel *> &Server::getChannels(){
    return _channels;
}
void Server::acceptNewConnection(Server *server, Client *client)
{
    server->send(client, RPL_WELCOME(client->getNickname()));
    server->addChannel("#public", "")->add(client, server);
};

Server::~Server()
{
    std::cout << "~Server\n";
}
