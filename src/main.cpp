/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles <rteles@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 08:52:49 by rteles            #+#    #+#             */
/*   Updated: 2023/04/03 18:56:56 by rteles           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>

#include "Server.hpp"
#include "Util.hpp"
#include "Client.hpp"

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
	if (s != nullptr)
		s->sendAll(nullptr, "com^Dman^Dd");//comando enviado para todos
	delete s;
  	exit(0);
}

int main(int argc, char **argv)
{
	(void) argv;
	std::signal(SIGINT, signal_handler);
	if (argc < 3 || !argv[2])
		std::cout << "ARGS: porta,  password\n";
	else
	{
		s = new Server(argv[1], 1234, argv[2]);
		s->run();
	}

    return  (1);
}
