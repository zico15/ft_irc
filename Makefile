# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rteles <rteles@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/21 23:30:53 by rteles            #+#    #+#              #
#    Updated: 2023/03/21 23:45:58 by rteles           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS 			= $(shell find src/ -name '*.cpp')
OBJS 			= $(addsuffix .o, $(basename $(SRCS)))
INCLUDES 		= $(addprefix -I, $(shell find headers -type d))

CXX 			= c++ -pedantic #-fsanitize=address -g
CXXFLAGS		= #-Wall -Wextra -Werror -std=c++98 -Wshadow
RM				= rm -f



all: $(NAME)

%.o:%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@  $^

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(NAME)

re: clean all

r:
	make && make clean && clear && ./$(NAME) 1234 abc

m:
	make fclean && make clean && clear

.PHONY: all clean fclean re
