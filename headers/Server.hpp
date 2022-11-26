/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 20:37:00 by ezequeil          #+#    #+#             */
/*   Updated: 2022/11/26 13:56:51 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <iostream>
#include <vector>

#include "Socket.hpp"

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
