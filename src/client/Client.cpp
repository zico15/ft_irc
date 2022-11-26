/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 22:40:38 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/26 07:49:08 by edos-san         ###   ########.fr       */
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
   // std::cout << "receiver\n";
}


bool Client::run()
{
    usleep(100);
    receiver();
    if (_datas.size() > 0)
    {    

        if(send(_socket->getFd(), _datas[0]->data.c_str(), _datas[0]->size, 0) < 0)
            return (false);
        delete _datas[0];
        _datas.erase(_datas.begin());
        //close(_socket->getFd());
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
        if (line.size() > 0)
        {   
            
            //std::cout << "line: " << line;
        }
            
    }
    return (true);
}
