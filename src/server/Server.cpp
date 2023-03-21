/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:36:54 by edos-san          #+#    #+#             */
/*   Updated: 2023/03/20 23:02:33 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){}

Server::Server(std::string hostname, int port, std::string password): _password(password)
{
    std::cout << "\x1B[2J\x1B[HServer has been created: " << port << " password: " << password << "\n";
    init(SERVER, hostname, port, 200);
    on("connect",  &Server::connect);
    //CAP
    on("PASS",  &Server::pass);
    _function_default =  &Server::msg;
    //on("help", &Server::help);
    //on("PRIVMSG", )
    on("/help", &Server::help);
    on("NICK", &Server::nick);
    on("USER", &Server::user);
    //on("/join", &Server::join);
    on("JOIN", &Server::join);
    on("/leave", &Server::leave);
    on("/quit", &Server::quit);
    on("/who", &Server::who);
    on("/msg", &Server::msg_private);
    on("/clear", &Server::clear);
}

void Server::pass(Client *client, String data)
{
    client->setPassword(data);
}

void Server::clear(Client *client, String data)
{
    send(client, "\x1B[2J\x1B[H");
}

/*nick [login]       change your login*/
void Server::nick(Client *client, String data)
{
   
    client->setNickname(data);
    std::cout << "Nick" << std::endl;
}

void Server::user(Client *client, String data)
{
    client->setUsername(data);
    if (client->getPassword().compare(this->getPassword()))
        send(client, PASSWORD_OK(client->getNickname()));
    else if (Client::isNickname(this->_clients,client->getNickname()))
        send(client, NICKNAME_ERROR(client->getNickname()));
    else
        send(client, PASSWORD_ERROR(client->getNickname()));
}

/*
/leave              leave current channel
*/
void Server::leave(Client *client, String data)
{
    if (client->getChannel())
    {    
        client->getChannel()->remove(client);
        send(client, client->getChannel()->getClients(), "\rUser: " + client->getNickname() + " remove room\n", RED);
        client->setChannel(NULL);
    }
}

/*join [channel]     join channel*/
void Server::join(Client *client, String data)
{
    if (data.empty() || client->getChannel())
        send(client, MSG_COMMAND_INVALID);
    else
    {
        Channel *channel = _channels[data];
        if (!channel)
        {
            channel = new Channel(data);
            _channels[data] = channel; 
        }
        channel->add(client);
        send(client, channel->getClients(), "\rUser: " + client->getNickname() + " in the room\n", YELLOW);
    }
}

/*
/quit               quit irc
*/
void Server::quit(Client *client, String data)
{
  if (client->getChannel())
    leave(client, "");
  send(client, "com^Dman^Dd\n", "");
  close(client->getFd());
  setEvent(client->getIndexFd(), -1, 0, 0);
  _clients.erase(client->getIndexFd());
  delete client;
  
}

/*
/who                list of users in channel
*/
void Server::who(Client *client, String data)
{
  if (client->getChannel())
  {
    std::vector<Client *> clients = client->getChannel()->getClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i] != client)
            send(client, std::to_string(i) + ": " + clients[i]->getNickname() + "\n", YELLOW);
    }
  }
  else
    send(client, MSG_COMMAND_INVALID);
}

/*
/msg [login] [msg]  submit msg at loginlogin
*/
void Server::msg_private(Client *client, String data)
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
                send(clients[i], data);
        
                return ;
            }
        }
    }
    send(client, MSG_MSG_INVALID);
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
void Server::help(Client *client, String data)
{
        std::cout << "help" << std::endl;
    send(client, MSH_HELP);
}

void Server::connect(Client *client, String data)
{
    int fd_client = socketAccept();
	if (fd_client < 0)
		return ;
	for (size_t i = 1; i < getMaxConnecting(); i++)
    {
        if (getSocket(i).fd == -1)
		{
			setEvent(i, fd_client, POLLIN | POLLHUP);
            _clients[i] = new Client(_fds[i].fd, i);
			break;
		}
    }
    setEvent(0, getFd(), POLLIN | POLLOUT | POLLHUP);
}

void Server::execute(Client *client, std::string event, String data)
{
	function fun = _events[event];
	if (!fun)
    {    
        fun = _function_default;
        data = event + " " + data;
    }
    (this->*fun)(client, data);
}

void Server::msg(Client *client, String data)
{
    Channel *channel = client->getChannel();
    if (channel)
    {    
        send(client, channel->getClients(), data);
    }
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
};

Server::~Server()
{
    std::cout << "~Server\n";
}

