NAME = ircserv

CC = c++  -pedantic
#-fsanitize=address -g

SRCS =	$(shell find src/ -name '*.cpp')
OBJS = $(addsuffix .o, $(basename $(SRCS)))
INCLUDES = $(addprefix -I, $(shell find headers -type d))

all: $(NAME)

%.o:%.cpp
	$(CC) $(INCLUDES) -c -o $@ $<

$(NAME): $(OBJS)
	$(CC) -o $@  $^

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(NAME)

re: clean all

r:
	make && make clean && clear && ./$(NAME) 1234 abc

m:
	make fclean && make clean && clear

.PHONY: all clean re
