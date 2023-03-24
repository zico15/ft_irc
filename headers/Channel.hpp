/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:32:55 by edos-san          #+#    #+#             */
/*   Updated: 2022/12/10 10:13:24 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Channel_HPP
# define Channel_HPP

#include <iostream>
//#include <string.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/ioctl.h>
//#include <sys/poll.h>
//#include <fcntl.h>
#include <map>
//#include <thread>
//#include <sys/socket.h>
#include <sys/poll.h>
//#include <bits/stdc++.h>
//#include <iostream>

# include "File.hpp"
# include "Console.hpp"

typedef std::string String;

typedef struct pollfd t_socket;

class Client;
class Server;

class Channel {

	private:
		std::string							_channel;	  
		std::vector<Client *>				_clients;
		std::vector<String>					_msg;

	public:	
		Channel(std::string	channel);
		~Channel();
		void add(Client *client);
		void remove(Client *client);
		std::string getName();
		std::vector<Client *> getClients();
		size_t getSize();
		static void join(Server *server, Client *client, String data);
};

#include "Client.hpp"

std::ostream& operator<<(std::ostream& os, Channel *channel);


#endif
