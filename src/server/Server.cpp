/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:36:54 by edos-san          #+#    #+#             */
/*   Updated: 2023/03/21 23:57:20 by rteles           ###   ########.fr       */
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
    on("/who", &Server::who);
    on("connect",  &Server::connect);
    //CAP
    on("PASS",  &Server::pass);
    _function_default =  &Server::errorcommand;
    on("/help", &Server::help);
    on("NICK", &Server::nick);
    on("USER", &Server::user);

    on("JOIN", &Channel::join);
    on("/leave", &Server::leave);
    on("/quit", &Server::quit);
    on("/who", &Server::who);
    on("/msg", &Server::msg_private);
    on("/clear", &Server::clear);
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

/*nick [login]       change your login*/
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
    if (client->getChannel())
    {    
        client->getChannel()->remove(client);
        server->send(client, client->getChannel()->getClients(), "\rUser: " + client->getNickname() + " remove room\n", RED);
        client->setChannel(NULL);
    }
}

/*
/quit               quit irc
*/
void Server::quit(Server *server, Client *client, String data)
{
  if (client->getChannel())
    server->leave(server, client, "");
  server->send(client, "com^Dman^Dd\n", "");
  close(client->getFd());
  server->setEvent(client->getIndexFd(), -1, 0, 0);
  server->removeClient(client);
  delete client;
}

/*
/who                list of users in channel
*/
void Server::who(Server *server, Client *client, String data)
{
  if (client->getChannel())
  {
    std::vector<Client *> clients = client->getChannel()->getClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i] != client)
            server->send(client, std::to_string(i) + ": " + clients[i]->getNickname() + "\n", YELLOW);
    }
  }
  else
    server->send(client, MSG_COMMAND_INVALID);
      /*
      Command: WHO
   Parameters: [<name> [<o>]]
   The WHO message is used by a client to generate a query which returns
   a list of information which 'matches' the <name> parameter given by
   the client.  In the absence of the <name> parameter, all visible
   (users who aren't invisible (user mode +i) and who don't have a
   common channel with the requesting client) are listed.  The same
   result can be achieved by using a <name> of "0" or any wildcard which
   will end up matching every entry possible.
   The <name> passed to WHO is matched against users' host, server, real
   name and nickname if the channel <name> cannot be found.
   If the "o" parameter is passed only operators are returned according
   to the name mask supplied.
   Numeric Replies:
           ERR_NOSUCHSERVER
           RPL_WHOREPLY                    RPL_ENDOFWHO
   Examples:
   WHO *.fi                        ; List all users who match against
                                   "*.fi".
   WHO jto* o                      ; List all users with a match against
                                   "jto*" if they are an operator.
    */
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
    
     
    }
}

/*+ " :Password incorrect\r\n"
/msg [login] [msg]  submit msg at loginlogin
*/
void Server::msg_private(Server *server, Client *client, String data)
{
    std::string user;

    user = data.substr(0, data.find_first_of(SPACES, 0));
	data = &data[user.size()];
	data = trim(data);
    if (client->getChannel())
    {
        std::vector<Client *> clients = client->getChannel()->getClients();
        for (size_t i = 0; i < clients.size(); i++)
        {
            if (clients[i]->getNickname() == user && client != clients[i])
            {    
                data = "\r" + std::string(BLUE) + "[private: " + client->getNickname()+"] " + COLOUR_END + data + "\n";
                server->send(clients[i], data);
        
                return ;
            }
        }
    }
    server->send(client, MSG_MSG_INVALID);
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
    {
        _function_default(this, client, event);
        std::cout << "event: " << event << " msg: " <<  ERR_UNKNOWNERROR(event) << "\n";
    }
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

