# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rteles <rteles@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/02 23:25:53 by rteles            #+#    #+#              #
#    Updated: 2023/04/06 15:16:12 by rteles           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= Bot

SRCS			= 	./main.cpp \
					./Bot.cpp \
					./Bot_Game.cpp \
					./Game.cpp \

SRCS_OBJS		= $(SRCS:.cpp=.o)

CXX				= c++
CXXFLAGS		= -Wall -Wextra -Werror -std=c++98 -Wshadow -fsanitize=address
RM				= rm -f

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(SRCS_OBJS)
	$(CXX) $(CXXFLAGS) $(SRCS_OBJS) -o $(NAME)

clean:
	$(RM) $(SRCS_OBJS)
	
fclean: clean
	$(RM) $(NAME)

r: re
	clear && ./$(NAME) localhost 1234 abc

re:	fclean all

.PHONY: all clean fclean re