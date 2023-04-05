# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rteles <rteles@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/21 23:30:53 by rteles            #+#    #+#              #
#    Updated: 2023/04/05 22:04:41 by rteles           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv
OBJ_PATH	= obj

SRCS		= $(shell find src/ -name '*.cpp')
OBJS		= $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(basename $(SRCS))))
INCLUDES	= $(addprefix -I, $(shell find headers -type d))

CXX			= c++
CXXFLAGS	= -O0 -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g
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
	clear && ./$(NAME) localhost abc

m:
	make fclean && clear

.PHONY: all clean fclean re r m
