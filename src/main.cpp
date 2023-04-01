/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 08:52:49 by rteles            #+#    #+#             */
/*   Updated: 2023/04/01 21:41:13 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		s->emitAll("com^Dman^Dd\n");//comando enviado para todos
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
		s = new Server("localhost", 1234, argv[2]);
		s->run();
	}

    return  (1);
}
