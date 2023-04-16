# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/21 23:30:53 by rteles            #+#    #+#              #
#    Updated: 2023/04/12 18:02:59 by edos-san         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv
OBJ_PATH	= obj

SRCS		= $(shell find src/ -name '*.cpp')
OBJS		= $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(basename $(SRCS))))
INCLUDES	= $(addprefix -I, $(shell find headers -type d))

CXX			= c++
CXXFLAGS	= -O0 -Wall -Wextra -Werror #-fsanitize=address -g
RM			= rm -f

all: $(NAME)

$(OBJ_PATH)/%.o:%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@  $^

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -rf $(NAME)

re: fclean all

r: re
	clear && ./$(NAME) localhost 1234 abc

v: re
	clear && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(NAME) localhost 1234 abc


	

m:
	make fclean && clear

.PHONY: all clean fclean re r m
