/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:54:58 by edos-san          #+#    #+#             */
/*   Updated: 2023/03/20 21:05:43 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <iostream>
//#include <string.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <arpa/inet.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/ioctl.h>
//#include <sys/poll.h>
//#include <fcntl.h>
#include <map>
//#include <sys/socket.h>
//#include <sys/poll.h>
//#include <bits/stdc++.h>
#include "Util.hpp"
#include "Data.hpp"
#include <vector>
#include "Client.hpp"
#include "Msg.hpp"

#define TIME_OUT 3 * 60 * 1000
#define BUFFER_SIZE 1024

typedef struct pollfd t_socket;

class Server;

typedef void (Server::*function)(Client *client, String data);

typedef enum e_type
{
	SERVER,
	CLIENT,
	NONE
} t_type;

typedef enum e_events
{
	SEND,
	RECV,
	CONNECTION,
	CLOSE
} t_events;



class Socket 
{
	protected:
		t_type								_type;
		std::string							_hostname;
		struct sockaddr_in					_addr;
		socklen_t         					_size;
		char								_buffer[BUFFER_SIZE];
		int									_port;
		int									_fd;
		id_t								_size_clinets;
		size_t								_maxConnecting;
		t_socket							*_fds;
		std::map<std::string, function>		_events;
		std::map<int, t_data *>				_datas;
		std::map<int, Client *>				_clients;
		function							_function_default;


	public:
		Socket();
		~Socket();
		void 				init(t_type type, std::string hostname,int port, size_t maxConnecting = 2);
		int					socketListen(void);
		int					getMaxConnecting();
		int					getFd();
		t_socket			*getSockets();
		t_socket			&getSocket(int i);
		std::string	const	&getHostName() const;
		int					socketAccept(void);
		void				setEvent(int i, int fd, short event, int revents = 0);
		void				recive(int i);
		void 				run();
		void				emit(int i, const std::string &data);
		void				emitAll(const std::string &data);
		void 				on(std::string event, void (Server::*function)(Client *client, String data));
		virtual void		execute(Client *client, std::string event, String data = "");
};	




#endif
