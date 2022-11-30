/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 20:37:00 by ezequeil          #+#    #+#             */
/*   Updated: 2022/11/30 18:42:31 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <string>
#include <time.h>
#include <cstdlib>
#include "Socket.hpp"


#define RPL_JOIN(source, channel)					":" + source + " JOIN :" + channel


class Server: public Socket {

	private:
		

	public:
		Server();
		Server(std::string name, int port);
		~Server();
		void connect(void *data);
		void msg(void *data);
		void help(void *data);
		void execute(std::string event, void *data = NULL);
	
};

typedef void (Server::*functionsd)(void *data);

#endif
