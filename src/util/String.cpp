/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   String.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 19:28:51 by edos-san          #+#    #+#             */
/*   Updated: 2022/12/09 19:24:03 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//# include <string>
# include <iostream>
# include <vector>//added
//# include  <bits/stdc++.h>

bool	isEqual(char c, const std::string &str)
{
	return (str.find_first_of(c) != std::string::npos ? true : false);
}

std::vector<std::string>	split(std::string str, std::string set)
{
    std::vector<std::string> ret;
	const char    *tmp;
    size_t  i;

    tmp = str.c_str();
    while (*tmp)
    {
        while (*tmp && isEqual(*tmp, set))
            tmp++;
        str = tmp;
        for (i = 0; tmp[i] && !isEqual(tmp[i], set); )
            i++;
        ret.push_back(str.substr(0, i));
        tmp += i;
    }
	return (ret);
}

std::string trim(std::string str) 
{
    int length;
    while (!str.empty()  && isspace(*str.begin()))
        str.erase(str.begin());
    for (length = (str.length() - 1); length >= 0 && isspace(str[length]); length--);
    return str.substr(0, length + 1);
}

std::string uppcase(std::string str)
{
    for (int j = 0; j < str.size(); j++)
		str[j] = std::toupper(str[j]);
    return str; 
}