/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Console.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:32:55 by edos-san          #+#    #+#             */
/*   Updated: 2022/12/11 14:18:31 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSOLE_HPP
# define CONSOLE_HPP

#include <iostream>
//#include <string.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/ioctl.h>
//#include <sys/poll.h>
//#include <fcntl.h>
//#include <vector>
//#include <thread>
//#include <sys/socket.h>
//#include <sys/poll.h>
//#include <bits/stdc++.h>
#include "Util.hpp"




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
