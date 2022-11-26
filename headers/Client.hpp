/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:32:55 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/25 12:10:17 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <map>
#include <sys/socket.h>
#include <sys/poll.h>
#include <bits/stdc++.h>

# include "File.hpp"
# include "Socket.hpp"

typedef struct pollfd t_socket;

class Client {

	private:
		Socket 		*_socket;

	public:
		Client();
		Client(std::string hostname, int port);
		~Client();
		bool run();
};
#endif