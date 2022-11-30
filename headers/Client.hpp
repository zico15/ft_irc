/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:32:55 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/30 19:27:41 by edos-san         ###   ########.fr       */
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
#include <thread>
#include <sys/socket.h>
#include <sys/poll.h>
#include <bits/stdc++.h>

# include "File.hpp"
# include "Console.hpp"

typedef struct pollfd t_socket;

class Client {

	private:		  
		int					  _fd;
		std::string 		  _nickname;
		std::string			  _username;
		Console				  console;

	public:
		Client();
		Client(int fd);
		~Client();
		void receiver();
		bool run();
		void setNickname(std::string nickname){
			_nickname = nickname;
		};
		
		std::string &getNickname(){
			return _nickname;
		};
		
		void setUsername(std::string username){
			_username = username;
		};

		std::string &getUsername(){
			return _username;
		};

		int getFd(){
			return _fd;
		};
};
#endif
