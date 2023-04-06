/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:39:47 by rteles            #+#    #+#             */
/*   Updated: 2023/04/06 15:22:38 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This code is a bot that interacts with a user on an IRC channel.
The code starts by declaring the Bot class.
The code then defines a Bot constructor that takes no parameters.
The constructor prints "Bot Default Constructor!".
The code then defines a Bot constructor that takes 4 parameters.
The constructor sets the bot's name, hostname, port, and password.
The code then defines a Bot copy constructor.
The code then defines a Bot destructor.
The destructor deletes the pollEvents variable and prints "Disconnected!".
The code then defines a Bot assignment operator.
The code then defines an authenticate function.
The function sends a password message to the server if the password is not empty.
The function then sends the bot's name to the server.
The code then defines a sendMessage function that takes a command and a message.
The function sends a message to the server.
The code then defines a debug function that takes a message and a callback.
The function sends the callback to the user and prints the callback.
The code then defines a run function.
The function authenticate the user and enters a loop that runs until the user disconnects.
The code then defines a quit function.
The function sends a quit message to the server and closes the socket.
The code then defines a recive function.
The function receives a message and responds accordingly.
The code then defines a privateMessage function.
The function checks if the message is a command. If it is a command, the function returns. Otherwise, the function responds accordingly.
The code then defines a response function.
The function checks the type of message and responds accordingly.
*/
#include "Bot.hpp"

Bot::Bot(void)
{
    std::cout << "Bot Default Constructor!" << std::endl;
}

Bot::Bot(std::string host, char * port, std::string password, std::string name) : 
_name(name), _hostname(host), _port(port), _password(password) 
{
	struct hostent *server = gethostbyname(host.c_str());
	if (server == NULL)
	    throw std::runtime_error("Erro: Isn't possible to resolve Hostname");

    struct sockaddr_in   target;
    
    _socket = socket(AF_INET, SOCK_STREAM, 0);//IPV 4, TCP

	if (_socket < 0)
		throw std::runtime_error("Error: While opening socket.");

	bzero((char *) &target, sizeof(target));

    target.sin_family = AF_INET;
    target.sin_port = htons(atof(port));
	bcopy((char *)server->h_addr, (char *)&target.sin_addr.s_addr, server->h_length);

	int	host_connect = connect(_socket, (struct sockaddr *)&target, sizeof(target));
    if (host_connect < 0)
		throw std::runtime_error("Erro: Bot can't connect the Server!");
	
	this->pollEvents = new pollfd[1];

	pollEvents[0].fd = _socket;
  	pollEvents[0].events = POLLIN;
}

Bot::Bot( Bot const & src)
{
    *this = src;
}

Bot::~Bot() {
	delete[] pollEvents;
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

	pollEvents[0].revents = 0;
	pollEvents[0].events = POLLIN | POLLOUT | POLLHUP;
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

	pollEvents[0].events = POLLIN | POLLHUP;
	pollEvents[0].revents = 0;

	return this->response(message);
}

void	Bot::privateMessage(std::string message)
{
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
	
	message = message.substr(message.find(":")+1, message.size()).c_str();


	if (message.find("Hello") != std::string::npos ||
		message.find("hello") != std::string::npos)
		callBack = BOT_HELLO(user);
	else if (message.find("!helpgame") != std::string::npos ||
			message.find("!HelpGame") != std::string::npos ||
			message.find("!HELPGAME") != std::string::npos)
		callBack = BOT_HELP_GAME();
	else if (message.find("!help") != std::string::npos ||
			message.find("!Help") != std::string::npos ||
			message.find("!HELP") != std::string::npos)
		callBack = BOT_HELP();
	else if (message.find("!game") != std::string::npos ||
			message.find("!Game") != std::string::npos ||
			message.find("!GAME") != std::string::npos)
	{
		this->gamePlay(user, channel, message.substr(message.find("!game")+5, message.size()).c_str());
		return ;
	}
	else if (message.find("!leaderboard") != std::string::npos)
	{
		callBack = showLeaderBoard();
		channel = "";
	}
	else if (message.find("!invite") != std::string::npos && !message.empty())
	{
		invite(message.substr(message.find("!invite")+8));
		return ;
	}
	else
		return ;
	
	debug(message, callBack, user, channel);

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
			if (message.find("com^Dman^Dd") == 0)
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
			if (message.find(":"+this->_name+"!"+this->_name+"@"+this->_hostname+" JOIN #") != std::string::npos)
			{
				welcomeChannel(message);
				return 0;
			}
			//std::cout << message << std::endl;
		}
	}
	return 0;
}

/*
The function "addPlayer" takes in a string and returns a map.
The code starts by checking if the map at the index of the given string is empty.
If it is not, the code will return the map.
If it is, the code will define a player variable and set it to an empty map.
The code will then set the EXP key in the player variable to 0, the WIN key in the player variable to 0, and the LEVEL key in the player variable to 1.
The code will then set the map at the index of the given string to the player variable.
The code will then return the map at the index of the given string.
*/
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

/*
This function takes in a string, a boolean, and an integer.
The code starts by checking if the given string is equal to the name of the bot.
If it is, the code will return.
If it is not, the code will define a player variable and set it to the result of the "addPlayer" function.
The code will then increment the EXP key of the player variable by the given integer.
The code will then increment the WIN key of the player variable by the given boolean.
The code will then check if the value of the EXP key of the player variable is greater than or equal to the value of the LEVEL key of the player variable multiplied by 100.
If the value of the EXP key of the player variable is greater than or equal to the value of the LEVEL key of the player variable multiplied by 100, the code will decrement the value of the EXP key of the player variable by the value of the LEVEL key of the player variable multiplied by 100.
The code will then increment the value of the LEVEL key of the player variable by 1.
The code will then set the map at the index of the given string to the player variable.
*/
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

/*
This function returns a string.
The code starts by defining 2 iterator variables.
The code then defines a board variable and sets it to an empty vector.
The code will then define a leaderBoard variable and set it to "------ LEADER BOARD ------".
The code will then enter a for loop that will run for each map in the _players map.
The code will then check if the board vector contains the value of the LEVEL key of the current map multiplied by 100 plus the value of the EXP key of the current map.
If the board vector does not contain the value of the LEVEL key of the current map multiplied by 100 plus the value of the EXP key of the current map, the code will push the value of the LEVEL key of the current map multiplied by 100 plus the value of the EXP key of the current map to the board vector.
The code will then sort the board vector and reverse it.
The code will then enter another for loop that will run for each integer in the board vector.
The code will then enter another for loop that will run for each map in the _players map.
The code will then check if the current integer in the board vector is equal to the value of the LEVEL key of the current map multiplied by 100 plus the value of the EXP key of the current map.
If the current integer in the board vector is equal to the value of the LEVEL key of the current map multiplied by 100 plus the value of the EXP key of the current map, the code will print the position of the player, the name of the player, the level of the player, the EXP of the player, and the EXP needed to level up.
The code will then increment the position variable by 1.
The code will then return the leaderBoard variable.
*/
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

/*
This function takes in a string.
The code starts by sending a message to the given string.
*/
void	Bot::invite(std::string message)
{
	sendMessage("JOIN #", message);
	std::cout << "\033[38;2;255;165;0m[" << this->_name << "] \033[0m" << "Join to " << message << std::endl;
}

/*
This function takes in a string and returns a string.
The code starts by checking if the given string is empty.
If it is not, the code will return the given string.
If it is, the code will define a length variable and set it to the length of the given string minus 1.
The code will then enter a while loop that will run until the length variable is greater than or equal to 0 and the given string at the index of the length variable is a space.
The code will then decrement the length variable by 1.
The code will then return the given string from the start up to the value of the length variable plus 1.
*/
std::string trim(std::string str) 
{
    int length;
    while (!str.empty()  && isspace(*str.begin()))
        str.erase(str.begin());
    for (length = (str.length() - 1); length >= 0 && isspace(str[length]); length--);
    return str.substr(0, length + 1);
}

/*
This function takes in a string.
The code starts by defining a channel variable.
The code will then check if the given string contains "#".
If it does not, the code will return.
If it does, the code will set the channel variable to the given string.
The code will then send a welcome message to the channel.
*/
void	Bot::welcomeChannel(std::string message)
{
	std::string channel = "";
		
	if (message.find("#") == std::string::npos)
		return ;
	
	channel = message.substr(message.find("#"), message.find(":test")).c_str();
	channel = trim(channel);

	sendMessage("PRIVMSG " + channel + " :", BOT_HELLO_CHANNEL(channel, this->_name));
}