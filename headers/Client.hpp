/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:32:55 by edos-san          #+#    #+#             */
/*   Updated: 2022/12/09 18:32:07 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "File.hpp"
#include "Console.hpp"
#include "Channel.hpp"

typedef std::string String;

typedef struct pollfd t_socket;

class Client {

	private:		  
		int					  _fd;
		std::string 		  _nickname;
		std::string			  _username;
		Console				  _console;
		Channel 			 *_channel;

	public:
		Client();
		Client(int fd);
		~Client();
		void receiver();
		bool run();
		void setNickname(std::string nickname);
		
		void setChannel(Channel *channel)
		{
			_channel = channel;
		};

		Channel *getChannel()
		{
			return _channel;
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
