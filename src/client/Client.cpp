/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 22:40:38 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/28 00:11:52 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"



Client::Client()
{
    return ;
}

Client::Client(std::string hostname, int port)
{
    _socket = new Socket(CLIENT, hostname, port, 2);
    fcntl(_socket->getFd(), F_SETFL, O_NONBLOCK);
    _socket->setEvent(0, 1, POLLIN);
}

Client::~Client() 
{
    
     
}

Socket *Client::getSocket(){
    return (_socket);
}

void Client::receiver()
{
	char buffer[1001];
	int length = 0;
    
	while ((length = recv(_socket->getFd(), buffer, 1000, 0)) > 0) {
		buffer[length] = 0;
        std::cout << buffer;
	}
}


bool Client::run()
{
    while (true)
    {

        int s = _socket->socketListen();
        if(s <= 0)
            continue ;
        receiver();
        if (_datas.size() > 0)
        { 
            if(send(_socket->getFd(), _datas[0]->data.c_str(), _datas[0]->size, 0) < 0)
                continue;
            delete _datas[0];
            _datas.erase(_datas.begin());
            if (_datas.size() == 0)
                _socket->setEvent(0, _socket->getFd(), POLLIN);
        }
        else
        {
            std::string 		  line;
            char buffer[256];
	        int length = 0;

	        if  ((length = read(0, buffer, 255)) > 0) {
	        	buffer[length] = 0;
                line = buffer;
                _datas = Data::write(4, "msg", line);
                _socket->setEvent(0, _socket->getFd(), POLLOUT);
	        }    
        }
    }
    return (true);
}
