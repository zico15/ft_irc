/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:32:55 by edos-san          #+#    #+#             */
/*   Updated: 2023/04/05 21:51:28 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Channel.hpp"

typedef struct pollfd t_socket;

class Server;

class Client {

	private:
		int					  _fd;
		int					  _index_fd;
		std::string 		  _hostname;

		std::string 		  _nickname;
		std::string			  _username;
		std::string			  _realname;
		std::string			  _password;
		bool 				  _isConnect;
		bool				  _capend;

		std::map<std::string, Channel *> _channels;
		Client();

	public:
		Client(int fd, int index, std::string hostname);
		~Client();
		void receiver();
		bool run();
		void setNickname(const std::string& nickname);
		bool	isValid();
		void	addChannel(Channel *channel);
		void	removeChannel(Channel *channel);
		std::string getPrefix() const;

		void setConnect(bool status){
			_isConnect = status;
		};

		void setcapend(bool status){
			_capend = status;
		};

		std::string &getHostname()
		{
			return _hostname;
		};

		std::map<std::string, Channel *> &getChannels()
		{
			return _channels;
		};

		std::string &getNickname(){
			return _nickname;
		};

		std::string &getRealname(){
			return _realname;
		};

		std::string &getPassword(){
			return _password;
		};

		void setPassword(const std::string& data){
			_password = data;
		};

		void setRealname(const std::string& realname){
			_realname = realname;
		};

		void setUsername(const std::string& username){
			_username = username;
		};

		std::string &getUsername(){
			return _username;
		};

		int getFd(){
			return _fd;
		};

		int getIndexFd(){
			return _index_fd;
		};

		bool isConnect(){
			return _isConnect;
		};

		static bool isNickname(std::map<int, Client *> clients, Client *client);
		static void msgPrivate(Server *server, Client *client, std::string data);
		static void userHost(Server *server, Client *client, std::string data);
		static void nick(Server *server, Client *client, std::string data);
		static void user(Server *server, Client *client, std::string data);
};	


#endif
