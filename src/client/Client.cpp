/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 22:40:38 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/26 14:13:26 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"



Client::Client()
{
    return ;
}

Client::Client(std::string hostname, int port)
{
    _socket = new Socket(CLIENT, hostname, port, 10);
    fcntl(_socket->getFd(), F_SETFL, O_NONBLOCK);
    fcntl(1, F_SETFL, O_NONBLOCK);
    _socket->setEvent(1, 1, POLLIN);
}

Client::~Client() 
{
    
     
}

Socket *Client::getSocket(){
    return (_socket);
}

void Client::receiver()
{
	char buffer[256];
	int length = 0;
    
	while ((length = recv(_socket->getFd(), buffer, 256, 0)) > 0) {
		buffer[length] = 0;
		std::cout << "recv: " << buffer << "\n";
	}
}


bool Client::run()
{
    while (true)
    {

        int s = _socket->socketListen();
        if(s <= 0)
            continue ;
        usleep(1000);
        receiver();
        if (_datas.size() > 0)
        { 
            if(send(_socket->getFd(), _datas[0]->data.c_str(), _datas[0]->size, 0) < 0)
                continue;
            delete _datas[0];
            _datas.erase(_datas.begin());
        }
        else
        {
            std::string 		  line;
            char buffer[256];
	        int length = 0;

	        if  ((length = read(1, buffer, 255)) > 0) {
	        	buffer[length] = 0;
                line = buffer;
	        	std::cout << "line: " << buffer << "\n";
                _datas = Data::write(4, "msg", line);
	        }    
        }
    }
    return (true);
}
