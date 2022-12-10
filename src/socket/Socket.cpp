/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:59:02 by edos-san          #+#    #+#             */
/*   Updated: 2022/12/09 20:17:21 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(){}

void Socket::init(t_type type, std::string hostname, int port, size_t maxConnecting)
{
	_type = type;
	_hostname  = hostname;
	_port = port;
	_maxConnecting = maxConnecting;
	bzero((char *) &_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = INADDR_ANY;	
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	_fds = new t_socket[_maxConnecting];
	//====================================
	if (type == SERVER)
	{	
		int opt = true;
		setsockopt(_fd, SOL_SOCKET,  SO_REUSEADDR, &opt, sizeof(opt));
		setsockopt(_fd, SOL_SOCKET,  SO_REUSEPORT, &opt, sizeof(opt));
		fcntl(_fd, F_SETFL, O_NONBLOCK);
		if ((bind(_fd, (struct sockaddr*) &_addr, sizeof(_addr))) < 0) 
			exit_error(strerror(errno));
		if (listen(_fd, maxConnecting) < 0)
			exit_error(strerror(errno));
		_size = sizeof(_addr);
		std::cout << "\t\t...Waiting for connections... \n\n";
		for (size_t i = 1; i < _maxConnecting; i++)
			setEvent(i, -1, 0);
	}
	else if (type == CLIENT)
	{
		struct hostent *sname = gethostbyname(hostname.c_str());
		if (!sname)
			exit_error("hostent");
		bcopy((char *) sname->h_addr, (char *) &_addr.sin_addr.s_addr, sname->h_length);
		if (connect(_fd, (struct sockaddr*) &_addr, sizeof(_addr)))
			exit_error(strerror(errno));	
		std::cout << "Client has been created: " << port << "\n";
	}
	_size_clinets = 1;
	_fds[0].fd = _fd;
  	_fds[0].events = POLLIN;	
}


Socket::~Socket()
{
	std::cout << "~Socket\n";
	delete [] _fds;
}

int Socket::socketListen(void)
{
	return (poll(_fds, getMaxConnecting(), -1));
}

int		Socket::getMaxConnecting()
{
	return (_maxConnecting);
}

int		Socket::getFd()
{
	return (_fd);
}

int	Socket::socketAccept(void)
{
	return (accept(_fd, NULL, NULL));
}


void	Socket::setEvent(int i, int fd, short event, int revents)
{
	_fds[i].fd = fd;
  	_fds[i].events = event;
	_fds[i].revents = revents;
}

std::string const &Socket::getHostName() const
{
	return this->_hostname;
}

t_socket	&Socket::getSocket(int i)
{
	return (_fds[i]);
}

t_socket	*Socket::getSockets(){
	return (_fds);
}

void	Socket::recive(int i)
{
	String 		event;
	String 		value;
	char		buffer[BUFFER_SIZE + 1];
	int 		size;

	while ((size = recv(_fds[i].fd, buffer, BUFFER_SIZE, 0)) > 0)
	{
		buffer[size] = 0;
		value += buffer;
		if (size < BUFFER_SIZE)
			break;
	}
	if (value.empty())
		return;
	event = value.substr(0, value.find_first_of(SPACES, 0));
	value = &value[event.size()];
	value = trim(value);
	execute(_clients[i], event,  value);
	_fds[i].events = POLLIN | POLLHUP;
	_fds[i].revents = 0;
	
}

void	Socket::emit(int i, const std::string &data)
{
	int size;

	size = send(_fds[i].fd, data.c_str(), data.length(), 0);
	_fds[i].revents = 0;
	_fds[i].events = POLLIN;
}

void	Socket::emitAll(const std::string &data)
{
	for (size_t i = 1; i < getMaxConnecting(); i++)
	{
		if (_fds[i].fd > 0)
		{	
			emit(i, data);
			_fds[i].events = POLLHUP;
		}
	}
}


void Socket::on(std::string event, function fun)
{
	_events.insert(std::pair<std::string, function>(event,fun));
}

void Socket::execute(Client *client, std::string event, String data)
{
	return ;
}

void Socket::run()
{
	size_t i = 0;
    while (true)
    {
        try
        {
            int s = socketListen();
            if(s <= 0)
                continue ;
            for (int i = 0; i < getMaxConnecting(); i++)
        	{
        		if(_fds[i].revents == 0)
        			continue;
				if(_fds[i].revents == POLLHUP)
        		{	
					std::cout << "close\n";
					continue;
				}
                if (_fds[i].fd == getFd())
	    		    execute(NULL, "connect");
                else if (_fds[i].revents & POLLIN)
                {
					recive(i);
				}             
	    	}
        }
        catch(const std::exception& e)
        {
            std::cout << "error: " << e.what() << '\n';
        }
    }
}
