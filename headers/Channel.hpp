/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:32:55 by edos-san          #+#    #+#             */
/*   Updated: 2023/04/01 21:31:38 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Channel_HPP
# define Channel_HPP

#include <iostream>
#include <arpa/inet.h>
#include <map>
#include <sys/poll.h>
#include <vector>

typedef struct pollfd t_socket;

class Client;
class Server;

class Channel {

	private:
		std::string							_channel;
		std::string							_pass;
		std::vector<Client *>				_clients;
		std::vector<std::string>			_msg;

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
		void 		send(Server *server, Client *client, std::string message);
		std::string nicksOnChannel(void);
		
		void 		who(Server *server, Client *client);
		

		static void join(Server *server, Client *client, std::string data);
		static void leave(Server *server, Client *client, std::string data);
		static void list(Server *server, Client *client, std::string data);
		static void 		mode(Server *server, Client *client, std::string data);
		static void 		kick(Server *server, Client *client, std::string data);


};



std::ostream& operator<<(std::ostream& os, Channel *channel);


#endif
