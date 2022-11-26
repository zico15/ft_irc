/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:59:02 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/25 23:56:55 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(){}

Socket::Socket(t_type type, std::string hostname, int port, size_t maxConnecting):
_type(type), _hostname(hostname), _port(port), _maxConnecting(maxConnecting)
{

	bzero((char *) &_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = INADDR_ANY;	
	//memset(&(_addr.sin_zero), '\0', 8);
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
			setEvent(i, 0, 0);
	}
	else if (type == CLIENT)
	{
		struct hostent *sname = gethostbyname(hostname.c_str());
		if (!sname)
			exit_error("hostent");
		_addr.sin_family = AF_INET;
		bcopy((char *) sname->h_addr, (char *) &_addr.sin_addr.s_addr, sname->h_length);
		if (connect(_fd, (struct sockaddr*) &_addr, sizeof(_addr)) < 0)
			exit_error(strerror(errno));
		//setEvent(1, , POLLIN | POLL_OUT);
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

	return (poll(_fds, getMaxConnecting(), 2));
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

t_socket	*Socket::getEvent(int i)
{
	return (&_fds[i]);
}

void	Socket::setEvent(int i, int fd, short event)
{
	_fds[i].fd = fd;
  	_fds[i].events = event;
	_fds[i].revents = 0;
}

std::string const &Socket::getHostName() const
{
	return this->_hostname;
}

bool Socket::createClient()
{
	int fd_client = socketAccept();
	if (fd_client < 0)
		return (false);
	for (size_t i = 1; i < _maxConnecting; i++)
    {
        if (_fds[i].fd == 0)
		{
		
			setEvent(i, fd_client, POLLIN | POLLOUT);
			_size_clinets++;
			std::cout << "new client: " << _fds[i].fd << "\n";
			return (true);
		}
    }
	return (false);
}

void	Socket::recive(int i)
{
	char buffer[BUFFER_SIZE + 1];
	int size;
	bool pin = _fds[i].revents & POLLIN;
	bool pou = _fds[i].revents & POLLOUT;

	size = recv(_fds[i].fd, buffer, BUFFER_SIZE, 0);
	if (size == -1)
		return;
	buffer[size] = 0;
	std::cout << "FD: " << _fds[i].fd << " POLLIN: " << pin;
	std::cout << " POLLOUT: " << pou << " BUFFER: " << buffer << "\n";
	_fds[i].events = POLL_IN;
}

void	Socket::send(int i)
{
	char buffer[BUFFER_SIZE + 1];
	int size;
	bool pin = _fds[i].revents & POLLIN;
	bool pou = _fds[i].revents & POLLOUT;

	size = recv(_fds[i].fd, buffer, BUFFER_SIZE, 0);
	if (size == -1)
		return;
	buffer[size] = 0;
	std::cout << "FD: " << _fds[i].fd << " POLLIN: " << pin;
	std::cout << " POLLOUT: " << pou << " BUFFER: " << buffer << "\n";
	_fds[i].events = POLL_IN;
}

void Socket::run()
{
    while (true)
    {
        try
        {
            int s = socketListen();
            if(s <= 0)
                continue ;
            for (int i = 0; i < getMaxConnecting(); i++)
        	{
        		if(getEvent(i)->revents == 0)
        			continue;
                else 
                {
                    if (getEvent(i)->fd == getFd())
	    			    createClient();
                    else if (getEvent(i)->revents & POLLIN)
                    {    
						recive(i);
						send(i);
					}
                }
	    	}
        }
        catch(const std::exception& e)
        {
            std::cout << "error: " << e.what() << '\n';
        }
    }
}
