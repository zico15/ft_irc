/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Console.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 22:51:58 by edos-san          #+#    #+#             */
/*   Updated: 2022/12/04 12:43:27 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Console.hpp"

Console::Console(){
    w = 100;
    h = 150;
    font = 10;
    marge = 2;
}

Console::~Console(){}

void Console::cread_line(std::string text)
{
    int index_text = 0;
    out << COLOUR_END << "┃" << COLOUR_RED;
    for (size_t i = 0; i < w;)
    {
        if (i > marge && text.size() > 0 && index_text < text.size())
            out << text[index_text++];
        else
            out << " ";
        i++;
    }
    out << COLOUR_END << "┃\n";
}

void Console::render_line(std::string left, std::string center, std::string right)
{
    out << COLOUR_END << left;
    for (size_t i = 1; i <= w; i++)
        out << center;
    out << right <<"\n";
}

void Console::add_data(std::string txt)
{
   std::vector<std::string> list =  split(txt, "\n\r");
   for (size_t i = 0; i < list.size(); i++)
        data.insert(data.end(), list[i]);
}



void Console::render()
{
    std::cout << getOut();
}

std::string Console::getOut()
{
    index_line = 0;
    index_h = 0;
    out.clear();
    out << "\x1B[2J\x1B[H";
    render_line("┏", "━", "┓");
    while (index_h < h)
    {
        cread_line(index_line < data.size() ? data[index_line] : "");
        index_h += font;
        index_line++;
    }
    render_line("┗", "━", "┛");
    out << "\033[32m [ Gust_1 ] " << COLOUR_END;
    return (out.str());
}