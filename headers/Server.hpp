/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 20:37:00 by ezequeil          #+#    #+#             */
/*   Updated: 2023/03/20 23:08:43 by rteles           ###   ########.fr       */
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
		void connect(Client *client, String data);
		void msg(Client *client, String data);
		void help(Client *client, String data);
		void nick(Client *client, String data);
		void join(Client *client, String data);
		void execute(Client *client, std::string event, String data = "");
		void leave(Client *client, String data);
		void quit(Client *client, String data);
		void who(Client *client, String data);
		void msg_private(Client *client, String data);
		void clear(Client *client, String data);
	    void pass(Client *client, String data);
		void user(Client *client, String data);
		std::string &getPassword();

		typedef void (Server::*functionsd)(void *data);
};
#endif
