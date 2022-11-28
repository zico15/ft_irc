/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Console.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:32:55 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/28 00:01:59 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSOLE_HPP
# define CONSOLE_HPP

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <sys/poll.h>
#include <bits/stdc++.h>
#include "Util.hpp"

#define COLOUR_RED "\033[31m"
#define COLOUR_YELLO "\033[33m"

#define COLOUR_GREEN "\033[7;1;32m"
#define COLOUR_END "\033[0m"
#define  COLOUR_YELLOW "\033[7;1;33m"


class Console {

	private:
		int						 w;
		int						 h;
		int     				 index_h;
		int						 index_line;
		int						 font;
		int						 marge;
		std::vector<std::string> data;
		std::stringstream		 out;
		
	public:
		Console();
		~Console();
		void render();
		void cread_line(std::string text);
		void render_line(std::string left, std::string center, std::string right);
		void add_data(std::string txt);
		std::string getOut();
};
#endif
