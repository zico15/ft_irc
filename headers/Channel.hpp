/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:32:55 by edos-san          #+#    #+#             */
/*   Updated: 2023/03/27 22:45:17 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Channel_HPP
# define Channel_HPP

#include <iostream>
#include <arpa/inet.h>
#include <map>
#include <sys/poll.h>

# include "File.hpp"
# include "Console.hpp"

typedef std::string String;

typedef struct pollfd t_socket;

class Client;
class Server;

class Channel {

	private:
		std::string							_channel;
		std::string							_pass;
		std::vector<Client *>				_clients;
		std::vector<String>					_msg;

	public:	
		Channel(std::string	channel);
		Channel(std::string	channel, std::string channelpass);
		~Channel();
		void add(Client *client, Server *server);
		void remove(Client *client);
		std::string getName();
		std::vector<Client *> getClients();
		size_t getSize();
		std::string getpass();

		bool 		isInTheChannel(Client *client);
		void 		sendMsgForAll(Server *server, Client *client, std::string message);
		std::string nicksOnChannel(void);
		
		static void join(Server *server, Client *client, String data);
		static void leave(Server *server, Client *client, String data);
		void 		list(Server *server, Client *client);
		void 		who(Server *server, Client *client);
		static void 		mode(Server *server, Client *client, String data);
		static void 		kick(Server *server, Client *client, String data);


};

#include "Client.hpp"

std::ostream& operator<<(std::ostream& os, Channel *channel);


#endif
