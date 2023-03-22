/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 21:54:58 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/26 07:23:39 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_HPP
# define DATA_HPP

#include <iostream>
//#include <string>
//#include <string.h>
#include <vector>

#define DATA_SIZE 20

/*
	ID:SEND:SENDS:EVENT:DATA
*/
typedef struct s_data
{
	int			id;
	int			send;
	int			sends;
	std::string	event;
	std::string	data;
	size_t		size;

} t_data;

class Data 
{

	
	public:
		static std::vector<t_data*> write(int id, std::string	event, std::string	data)
		{
			data = event + ":" + data;		
			size_t	sends = (data.size() / DATA_SIZE) + 1;
			

			std::vector<t_data*> datas;
			std::cout << "sends: " << sends << " size: " << data.size() << "\n";
			for (size_t send = 0; send < sends;)
			{			
				std::stringstream	s;
				t_data				*d = new t_data;

				d->id = id;
				d->send = (++send);
				d->sends = sends;
				s << d->id << ":" << d->send << ":" << d->sends << ":"	<< data.substr(0, DATA_SIZE) << "\r\n";	
				d->data = s.str();
				d->size = d->data.length();
				if (data.size() > DATA_SIZE)			
					data = data.substr(DATA_SIZE, data.size());
				datas.push_back(d);	
			}
			return (datas);
		};
		
				
		static t_data *read(std::string	pack)
		{
			t_data				*d = new t_data;
			int					index;

			index = pack.find(':', 0);
			d->id = std::atoi(pack.substr(0, index).c_str());
			pack = &pack[index + 1];
			//=======================
			index = pack.find(':', 0);
			d->send = std::atoi(pack.substr(0, index).c_str());	
			pack = &pack[index + 1];
			//=======================
			index = pack.find(':', 0);
			d->sends = std::atoi(pack.substr(0, index).c_str());
			pack = &pack[index + 1];
			//=======================
			index = pack.find(':', 0);
			if (index < pack.size())
			{
				d->event = pack.substr(0, index);	
				pack = &pack[index + 1];
			}			
			//=======================
			d->data = pack.substr(0, pack.find_last_of('\n', pack.size()));
			return (d);
		};
};

#endif
