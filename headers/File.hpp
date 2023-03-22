/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 15:48:35 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/24 16:51:32 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
# define FILE_HPP

# include <iostream>
# include <sstream>//added
# include <fstream>//added
//# include <string>
//# include  <bits/stdc++.h>

class File
{
	private:
		std::string 		_path;
		std::stringstream   _out;
		size_t				_size;				
	public:
		File(std::string path);
		File(std::string absolute, std::string file);
		~File();
		std::string read();
		void		write(std::string value);
		size_t		size();
		std::string getExtensao();
};


#endif
