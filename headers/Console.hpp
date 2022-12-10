/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Console.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:32:55 by edos-san          #+#    #+#             */
/*   Updated: 2022/12/09 22:26:45 by edos-san         ###   ########.fr       */
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
#define COLOUR_YELLOW "\033[7;1;33m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */



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
