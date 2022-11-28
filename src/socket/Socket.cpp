/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:59:02 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/28 00:13:40 by edos-san         ###   ########.fr       */
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
	return (poll(_fds, getMaxConnecting(), __INT_MAX__));
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
	t_data		*data;	
	char		buffer[BUFFER_SIZE + 1];
	int 		size;

	size = recv(_fds[i].fd, buffer, BUFFER_SIZE, 0);
	if (size == -1)
		return;
	buffer[size] = 0;
	std::cout << "Buffer: " << buffer << "\n";
	data = Data::read(std::string(buffer));
	if (data->sends == 1)
	{	
		execute(data->event, (void *) data->data.c_str());
		delete data;
	}
	else
	{
		t_data *tmp = _datas[data->id];
		if (tmp == NULL)
			_datas[data->id] = data;
		else
		{
			tmp->data += data->data;
			if (data->send == data->sends)
			{
				execute(tmp->event, (void *) tmp->data.c_str());
  				_datas.erase (_datas.find(data->id));
				delete tmp;
			}
			delete data;
		}	
	}
	_fds[i].events = POLLOUT;
	_fds[i].revents = 0;
}

void	Socket::emit(int i, const std::string &data)
{
	int size;

	console.add_data(data);
	std::string a = console.getOut() + "\n";
	std::cout << a;
	size = send(_fds[i].fd, a.c_str(), a.length(), 0);
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

void Socket::execute(std::string event, void *data)
{
	function fun = _events[event];
	if (fun)
		fun(data);
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
				std::cout << "fd: " << _fds[i].fd << " POLLIN: " << (_fds[i].revents & POLLIN) << " POLLOUT: " << (_fds[i].revents & POLLOUT) << "\n";
                if (_fds[i].fd == getFd())
	    		    execute("connect", this);
                else if (_fds[i].revents & POLLIN)
                {
					recive(i);
					emit(i, "hello word!");
				}             
	    	}
        }
        catch(const std::exception& e)
        {
            std::cout << "error: " << e.what() << '\n';
        }
    }
}
