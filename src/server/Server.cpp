/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:36:54 by edos-san          #+#    #+#             */
/*   Updated: 2023/03/27 22:06:58 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){}

Server::Server(std::string hostname, int port, std::string password): _password(password)
{
    std::cout << "\x1B[2J\x1B[HServer has been created: " << port << " password: " << password << "\n";
    init(SERVER, hostname, port, 200);

    //testing bellow
    on("PING", &Server::ping);
    on("CAP", &Server::cap);
    on("WHO", &Server::who);
    //on("who", &Server::who);
    on("connect",  &Server::connect);
    on("USERHOST", &Server::userhost);
    //CAP
    on("PASS",  &Server::pass);
    _function_default =  &Server::errorcommand;
    on("/help", &Server::help);
    on("NICK", &Server::nick);
    on("USER", &Server::user);

    on("JOIN", &Channel::join);
    on("PRIVMSG", &Server::msg_private);
    on("PART", &Channel::leave);

    on("LIST", &Server::list);

    /*---- QUIT the Channel ----
    event: QUIT
    value: :Konversation terminated!
    */
    on("QUIT", &Server::quit);

    on("/leave", &Server::leave);
    on("/quit", &Server::quit);
    on("/clear", &Server::clear);

    addChannel("#public");
}

void Server::userhost(Server *server, Client *client, String data)
{
    server->send(client, RPL_USERHOST(data));
}


void Server::pass(Server *server, Client *client, String data)
{
    data = data.substr(1);


    if (server->getPassword() == data)
        client->setPassword(data);
    else
        server->send(client, ERR_PASSWDMISMATCH(client->getNickname()));
    if (client->isValid())
        server->send(client, RPL_WELCOME(client->getNickname()));
}

void Server::clear(Server *server, Client *client, String data)
{
    server->send(client, "\x1B[2J\x1B[H");
}

void Server::nick(Server *server, Client *client, String data)
{
    client->setNickname(data);
    if (Client::isNickname(server->getClients(), client))
    {   
        server->send(client, NICKNAME_ERROR(client->getNickname()));
        client->setNickname("");
    }
    else if (client->isValid())
        server->send(client, RPL_WELCOME(client->getNickname()));
}

void Server::user(Server *server, Client *client, String data)
{
    client->setUsername(data.substr(0, data.find(' ')));
    client->setRealname(data.substr(data.find(':') + 1));
    if (client->isValid())
        server->send(client, RPL_WELCOME(client->getNickname()));
}

/*
/leave              leave current channel
*/
void Server::leave(Server *server, Client *client, String data)
{
    /*if (client->getChannel())
    {    
        client->getChannel()->remove(client);
        server->send(client, client->getChannel()->getClients(), "\rUser: " + client->getNickname() + " remove room\n", RED);
        client->setChannel(NULL);
    }*/
}

/*
/quit               quit irc
*/
void Server::quit(Server *server, Client *client, String data)
{
    /*if (client->getChannel())
      server->leave(server, client, "");*/
    server->send(client, "com^Dman^Dd\r\n", "");
    close(client->getFd());
    server->setEvent(client->getIndexFd(), -1, 0, 0);
    server->removeClient(client);
    delete client;
}

/*
/who [channel]        list of users in channel
*/
void Server::who(Server *server, Client *client, String data)
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
void Server::ping(Server *server, Client *client, String data)
{
    std::string reply = ("PONG :" + data );

    //std::cout << YELLOW "the following message will be sent to the client: ->\t\t" RED << reply + RESET << std::endl;
    server->send(client, reply);
}

//https://ircv3.net/specs/core/capability-negotiation-3.1.html#available-capabilities
void Server::cap(Server *server, Client *client, String data)
{
    if (data == "LS 302")
        server->send(client, "CAP * LS :message-tags multi-prefix");
    else if (data == "REQ :multi-prefix")
        server->send(client, "CAP * ACK :multi-prefix");
    else if (data == "END")
    {
        client->setConnect(true);
        if (client->isValid())
        {
            server->send(client, RPL_WELCOME(client->getNickname()));
            //Channel *public_channel = this->_channels["public"];
            //client->setChannel(_channels["public"]);
            //client->addChannel(_channels["public"])
        }
    }
}

/*+ " :Password incorrect\r\n"
/msg [login] [msg]  submit msg at loginlogin
*/
void Server::msg_private(Server *server, Client *client, String data)
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
        
        Channel *channel = server->getChannels()[dest];
        
        if (channel)
        {
            std::vector<Client *> clients = channel->getClients();
            std::vector<Client *>::iterator it;
        
            if (channel->isInTheChannel(client) == false) //Client is not in the Server
                return ;

            channel->sendMsgForAll(server, client, message);
        }
    }
}

/* /list               list of channel */
void Server::list(Server *server, Client *client, String data)
{
    /*
    :servername 321 nick Channel (2):Users  Name
    :servername 322 nick #teste1 (1) :Example topic
    :servername 322 nick #teste2 (1):Another topic
    :servername 323 nick :End of /LIST
    */

   	if (!data.empty()) //if NOT empty
		return ;
	
    std::string nick = client->getNickname();
    
    std::map<String, Channel *> channels = server->getChannels();
    
    std::ostringstream stream;
    stream << channels.size() - 1;
    std::string channelsNmbr = stream.str();

    server->send(client, LIST_START(nick, channelsNmbr));
    
    std::map<String, Channel *>::iterator it;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it).first != "public")
			(*it).second->list(server, client);
	}
    
	server->send(client, LIST_END(nick));
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
void Server::help(Server *server, Client *client, String data)
{
    if (data.size() == 0)
        server->send(client, MSH_HELP);
    else
        server->send(client, "HELP LIST COMMAND!\r\n");
}

void Server::connect(Server *server, Client *client, String data)
{
    int fd_client = server->socketAccept();
	if (fd_client < 0)
		return ;
	for (size_t i = 1; i < server->getMaxConnecting(); i++)
    {
        if (server->getSocket(i).fd == -1)
		{
			server->setEvent(i, fd_client, POLLIN | POLLHUP);
            server->addClient(i, new Client(server->getSocket(i).fd, i));
			break;
		}
    }
    server->setEvent(0, server->getFd(), POLLIN | POLLOUT | POLLHUP);
}

void Server::execute(Client *client, std::string event, String data)
{
	function fun = _events[event];
	if (!fun)  
        _function_default(this, client, event);
    else
        (fun)(this, client, data);
}

void Server::errorcommand(Server *server, Client *client, String data)
{
    server->send(client, ERR_UNKNOWNERROR(data));
}

void Server::send(Client *client, std::vector<Client *> clients, std::string data, std::string color)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i] != client)
             send(clients[i], data , color);
    }
}

void Server::send(Client *client, std::string data, std::string color)
{
    if (!client)
        return ;
   emit(client->getIndexFd(), data + "\r\n");
}

Channel *Server::addChannel(std::string const channelName)
{
    if (!_channels[channelName])
    {
        std::cout << "\033[35m" << "Server:\nAdicionado o Channel: " << channelName << "\033[0m" << std::endl;
        _channels[channelName] = new Channel(channelName);   
    }
    
    return _channels[channelName];
}

std::string &Server::getPassword(){
	return _password;
}
std::map<String, Channel *> &Server::getChannels(){
    return _channels;
};

Server::~Server()
{
    std::cout << "~Server\n";
}
