/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 20:37:00 by ezequeil          #+#    #+#             */
/*   Updated: 2022/11/27 23:59:40 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <iostream>
#include <string>
#include <time.h>
#include <cstdlib>
#include "Console.hpp"


#include "Socket.hpp"

#define RPL_JOIN(source, channel)					":" + source + " JOIN :" + channel

class Server {

	private:
		Socket 					*_socket;

	public:
		Server();
		Server(std::string name, int port);
		~Server();
		void	run();
		void	createClient();
		void	runClient(int fd);
		Socket	*getSocket();
	
};

#endif
