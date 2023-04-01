/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:39:47 by rteles            #+#    #+#             */
/*   Updated: 2023/03/30 16:11:49 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot(void)
{
    std::cout << "Bot Default Constructor!" << std::endl;
}

Bot::Bot(std::string host, char * port, std::string password, std::string name) : 
_name(name), _hostname(host), _port(port), _password(password) 
{
    struct sockaddr_in   target;
    
    _socket = socket(AF_INET, SOCK_STREAM, 0);//IPV 4, TCP

	if (_socket < 0)
		throw std::runtime_error("Error: While opening socket.");

	bzero((char *) &target, sizeof(target));

    target.sin_family = AF_INET;
    target.sin_port = htons(atof(port));
    target.sin_addr.s_addr = INADDR_ANY;

	int	host_connect = connect(_socket, (struct sockaddr *)&target, sizeof(target));
    if (host_connect < 0)
		throw std::runtime_error("Erro: Bot can't connect the Server!");
	
	this->pollEvents = new pollfd[1];

	pollEvents[0].fd = _socket;
  	pollEvents[0].events = POLLIN;
	//pollEvents[1].fd = host_connect;
  	//pollEvents[1].events = POLLIN;
}

Bot::Bot( Bot const & src)
{
    *this = src;
}

Bot::~Bot() {
	delete pollEvents;
	std::cout << "\033[31mDisconnected!\033[0m" << std::endl;
}

Bot & Bot::operator=(Bot const & rhs)
{
    if (this != &rhs)
    {
        this->_name = rhs._name;
		this->_hostname = rhs._hostname;
		this->_port = rhs._port;
		this->_password = rhs._password;
		this->_socket = rhs._socket;
    }

    return *this;
}

void	Bot::authenticate(void)
{
	if (!_password.empty())
		sendMessage("PASS :", this->_password);
	usleep(100);
	sendMessage("NICK ", this->_name);
	usleep(100);
	sendMessage("USER ", this->_name + " 0 * :" + this->_name);
	usleep(100);
	sendMessage("CAP ", "END");
}

void Bot::sendMessage(std::string const command, std::string const message)
{
	std::string	message_for_send = command + message + "\r\n";

	send(this->_socket, message_for_send.c_str(), message_for_send.size(), 0);
}

void Bot::debug(std::string message, std::string callBack, std::string user, std::string channel)
{
	if (callBack.empty())
	{
		std::cout << "GIVE A VALOR TO CALLBACK!" << std::endl;
		return ;
	}

	if (channel.empty())
		sendMessage("PRIVMSG " + user + " :", callBack);
	else
		sendMessage("PRIVMSG " + channel + " :", callBack);

	if (!message.empty())
		std::cout << "\033[32m[" << user << "] \033[0m" << message;

	std::cout << "\033[38;2;255;165;0m[" << this->_name << "] \033[0m" << callBack << std::endl;
}

void Bot::run(void)
{
	this->authenticate();

	int	timeout = 1000;// 1 second
	int	poll_response;
    while (true)
    {
		poll_response = poll(pollEvents, 1, timeout);

		if (poll_response < 0)
		{
			delete this;
			throw std::runtime_error("Erro: Waiting for Events!");
		}
		else if (poll_response == 0 && _games.size())
			gameTime();
		else
		{
			if (pollEvents[0].revents & POLLHUP) //When disconnect
			{
				std::cout << this->_name << " disconnect..." << std::endl;
        		break ;
			}
			if (pollEvents[0].revents & POLLIN) //Recive a command
			{
				if (this->recive() == 1)
					break ;
				pollEvents[0].events = POLLIN | POLLHUP;
				pollEvents[0].revents = 0;
			}	
		}
	}
	this->quit();
}

void Bot::quit(void)
{
	sendMessage("QUIT :", BOT_LEAVE());
	close(_socket);
}

int	Bot::recive(void)
{
	int			size;
	char		buffer[BUFFER_SIZE];
	std::string message;
	
	size = recv(_socket, buffer, BUFFER_SIZE - 1, 0);
	while (size)
	{
		if (size < 0)
		{
			std::cerr << "Erro: Recive!" << std::endl;
			return 1;
		}
		buffer[size] = '\0';
		message += buffer;
		if (size != BUFFER_SIZE)
			break ;
		size = recv(_socket, buffer, BUFFER_SIZE - 1, 0);
	}

	if (message.empty())
		return 0;

	return this->response(message);
}

/*
Prefixo: :rteles!rteles@localhost
Comando: PRIVMSG
Parâmetro 1: #public
Parâmetro 2: Oi, como você está?
//Envio:

PRIVMSG #public :Oi, estou bem! E você?
		ou
PRIVMSG rteles :Oi, estou bem! E você?
*/
void	Bot::privateMessage(std::string message)
{
/*
rteles!rteles@localhost PRIVMSG #public :Hello
rteles!rteles@localhost PRIVMSG meu_bot :Hello, como você está?
rteles!rteles@localhost PRIVMSG meu_bot :!help
rteles!rteles@localhost PRIVMSG #general meu_bot :!help
rteles!rteles@localhost PRIVMSG meu_bot :!game
rteles!rteles@localhost PRIVMSG meu_bot :sad
rteles!rteles@localhost PRIVMSG #general meu_bot :!game
*/
	std::string		user = "";
	std::string		channel = "";
	std::string		callBack = "";
    std::size_t     found;

	found = message.find(":");
	if (found == 0)
		message = message.substr(1, message.size()).c_str();
	found = message.find(":");
	if (found == std::string::npos)
		return ;
	user = message.substr(0, message.find("!")).c_str();
	if (message.find("#") != std::string::npos)
	{
		channel = message.substr(message.find("#"), message.find(":")).c_str();
		channel = channel.substr(channel.find("#"), channel.find(" ")).c_str();
	}

	callBack = message.substr(message.find(":")+1, message.size()).c_str();

	if (callBack.find("Hello") != std::string::npos ||
		callBack.find("hello") != std::string::npos)
		callBack = BOT_HELLO(user);
	else if (callBack.find("!help game") != std::string::npos ||
			callBack.find("!Help Game") != std::string::npos ||
			callBack.find("!HELP GAME") != std::string::npos)
		callBack = BOT_HELP_GAME();
	else if (callBack.find("!help") != std::string::npos ||
			callBack.find("!Help") != std::string::npos ||
			callBack.find("!HELP") != std::string::npos)
		callBack = BOT_HELP();
	else if (callBack.find("!game") != std::string::npos ||
			callBack.find("!Game") != std::string::npos ||
			callBack.find("!GAME") != std::string::npos)
	{
		this->gamePlay(user, channel, message, callBack.substr(callBack.find("!game")+5, callBack.size()).c_str());
		return ;
	}
	else if (callBack.find("!leaderboard") != std::string::npos)
	{
		callBack = showLeaderBoard();
		channel = "";
	}
	else
		return ;
	
	debug(message, callBack,user, channel);

}

int	Bot::response(std::string message)
{
	std::string		line;
    std::size_t     found;
	std::string		command = "";
	std::string		value;
	std::string		callBack = "";

	if (message.find("PING") == 0)
	{
		value = message.substr(4, message.length()).c_str();
		callBack = "PONG" + value;
		sendMessage("", callBack);
	}
	else
	{
		found = message.find(" PRIVMSG ");
		if (found != std::string::npos)
			this->privateMessage(message);
		else
		{
			if (message == "com^Dman^Dd") //TODO
			{
				return 1;
			}
			if (message.find(":Password incorrect") != std::string::npos && message.find("464") != std::string::npos)
			{
				std::cerr << "Erro: Password incorrect!" << std::endl;
				return 1;
			}
			if (message.find(":Nickname incorreta.") != std::string::npos && message.find("433") != std::string::npos)
			{
				std::cerr << "Erro: Nickname incorrect!" << std::endl;
				return 1;
			}
			if (message.find(":Welcome to server, "+this->_name) != std::string::npos && message.find("001") != std::string::npos)
			{
				std::cout << "\033[32mConnected!\033[0m" << std::endl;
				return 0;
			}
			std::cout << message << std::endl;
		}
	}
	return 0;
}

std::map<std::string, int> &Bot::addPlayer(std::string nick)
{	
	if (!_players[nick].empty())
		return _players[nick];

    std::map<std::string, int> player;

	player["EXP"] = 0;
	player["WIN"] = 0;
	player["LEVEL"] = 1;
	
	_players[nick] = player;
	
	return _players[nick];
}

void	Bot::setPlayer(std::string nick, bool isWin, int exp)
{
	if (nick == _name)
		return ;

	std::map<std::string, int> player = this->addPlayer(nick);

	player["EXP"] += exp;
	player["WIN"] += isWin ? 1 : 0;

	if (player["EXP"] >= player["LEVEL"] * 100)
	{
		player["EXP"] -= player["LEVEL"] * 100;
		player["LEVEL"] += 1;
		std::cout << nick << " up for Level " << player["LEVEL"] << "!" << std::endl;

		debug("LEVEL UP!\n", nick + " up for Level " + convertToString(player["LEVEL"]) + "!", nick, "");
	}

	_players[nick] = player;
}

std::string	Bot::showLeaderBoard(void)
{
	std::map<std::string, std::map<std::string, int> >::iterator it;
   	std::vector<int> 	board;
	std::string		leaderBoard = "------ LEADER BOARD ------\n";
	
    for (it = this->_players.begin(); it != this->_players.end(); ++it) 
    {
		if (std::find(board.begin(), board.end(), (it->second["LEVEL"] * 100) + it->second["EXP"]) == board.end())
			board.push_back((it->second["LEVEL"] * 100) + it->second["EXP"]);
	}
	
	std::sort(board.begin(), board.end());
	std::reverse(board.begin(),board.end());

	std::vector<int>::iterator vector_it;
	int i = 1;
	for (vector_it = board.begin(); vector_it != board.end(); ++vector_it) 
    {
		for (it = this->_players.begin(); it != this->_players.end(); ++it) 
    	{
    		std::string pos = convertToString(i);
			if (*vector_it == (it->second["LEVEL"] * 100) + it->second["EXP"])
			{
 				std::string aux = pos + "º " + it->first + " (LVL: " + convertToString(it->second["LEVEL"]) + ", EXP: " + convertToString(it->second["EXP"]) + "/" +  convertToString(it->second["LEVEL"] * 100) + ")";
				std::cout << i << "º - " << it->first << " (LVL: " << it->second["LEVEL"] << ", EXP: " << it->second["EXP"] << "/" <<  it->second["LEVEL"] * 100 << ")" << std::endl;	
				leaderBoard += aux + "\n";
			}
		}
		i++;
	}
	return leaderBoard;
}
