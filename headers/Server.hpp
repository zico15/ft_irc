/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 20:37:00 by ezequeil          #+#    #+#             */
/*   Updated: 2023/03/25 17:12:48 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <time.h>
#include <cstdlib>
#include "Socket.hpp"
#include "Channel.hpp"

class Server: public Socket {

	private:
		
		std::map<String, Channel *> _channels;
		std::string					_password;

	public:
		Server();
		Server(std::string name, int port, std::string password);
		~Server();
		void send(Client *client, std::string data, std::string color = "\033[0m");
		void send(Client *client, std::vector<Client *> clients, std::string data, std::string color = "\033[0m");
		void execute(Client *client, std::string event, String data = "");
		std::string &getPassword();
		std::map<String, Channel *> &getChannels();
	
		static void connect(Server *server, Client *client, String data);
		static void errorcommand(Server *server, Client *client, String data);
		static void help(Server *server, Client *client, String data);
		static void nick(Server *server, Client *client, String data);
		static void leave(Server *server, Client *client, String data);
		static void quit(Server *server, Client *client, String data);
		static void who(Server *server, Client *client, String data);
		static void msg_private(Server *server, Client *client, String data);
		static void clear(Server *server, Client *client, String data);
	    static void pass(Server *server, Client *client, String data);
		static void user(Server *server, Client *client, String data);
		static void	ping(Server *server, Client *client, String data);
		static void	cap(Server *server, Client *client, String data);
		static void userhost(Server *server, Client *client, String data);

		void addChannel(std::string const channelName);


};
#endif
