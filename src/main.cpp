/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 08:52:49 by rteles            #+#    #+#             */
/*   Updated: 2023/04/05 22:07:40 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include "Server.hpp"
#include "Util.hpp"
#include "Client.hpp"
#include <cstdlib>

Server *s = NULL;

int exit_error(std::string msg)
{
	std::cerr << "Error: " << msg << std::endl;
	delete s;
	exit(0);
	return (1);
}

void signal_handler(int signal)
{
	(void)signal;
	if (s != NULL)
		s->sendAll(NULL, "com^Dman^Dd");//comando enviado para todos
	delete s;
  	exit(0);
}

int main(int argc, char **argv)
{
	(void) argv;
	std::signal(SIGINT, signal_handler);
	if (argc != 4 || !argv[2] || !argv[3])
		std::cout << "ARGS: Port, Password." << std::endl;
	else
	{
		if (!isNumber(argv[2]))
		{
			std::cerr << "The Port need to be all Number!" << std::endl;
			return (1);
		}
		s = new Server(argv[1], atof(argv[2]), argv[3]);
		s->run();
	}
    return  (1);
}
