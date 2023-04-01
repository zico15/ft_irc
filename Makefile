# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/21 23:30:53 by rteles            #+#    #+#              #
#    Updated: 2023/04/01 21:00:34 by edos-san         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS 			= $(shell find src/ -name '*.cpp')
OBJS 			= $(addsuffix .o, $(basename $(SRCS)))
INCLUDES 		= $(addprefix -I, $(shell find headers -type d))

CXX 			= c++
CXXFLAGS		= -O0 #-pedantic #-Wall -Wextra -Werror -std=c++98 -Wshadow #-fsanitize=address -g
RM				= rm -f



all: $(NAME)

%.o:%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@  $^

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

r: re
	clear && ./$(NAME) 1234 abc

m:
	make fclean && make clean && clear

.PHONY: all clean fclean re r m
