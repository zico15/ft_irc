/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:54:58 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/25 23:56:22 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
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
#include "Util.hpp"

#define BUFFER_SIZE 1024
#define TIME_OUT 3 * 60 * 1000

typedef struct pollfd t_socket;

typedef enum e_type
{
	SERVER,
	CLIENT,
	NONE
} t_type;

class Socket 
{
	private:
		t_type					_type;
		std::string				_hostname;
		struct sockaddr_in		_addr;
		socklen_t         		_size;
		char					_buffer[BUFFER_SIZE];
		int						_port;
		int						_fd;
		id_t					_size_clinets;
		size_t					_maxConnecting;
		t_socket				*_fds;

	public:
		Socket();
		Socket(t_type type, std::string hostname,int port, size_t maxConnecting = 2);
		~Socket();
		int					socketListen(void);
		int					getMaxConnecting();
		int					getFd();
		t_socket			*getEvent(int i);
		std::string	const	&getHostName() const;
		int					socketAccept(void);
		void				setEvent(int i, int fd, short event);
		bool				createClient(void);
		void				recive(int i);
		void				send(int i);
		void 				run();
};

#endif
