NAME = ircserv

CXX = c++
CXXFLAGS =  -pedantic #-fsanitize=address -g

SRCS =	$(shell find src/ -name '*.cpp')
OBJS = $(addsuffix .o, $(basename $(SRCS)))
INCLUDES = $(addprefix -I, $(shell find headers -type d))

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
	./$(NAME)

m:
	make fclean && make clean && clear

.PHONY: all clean fclean re r m
