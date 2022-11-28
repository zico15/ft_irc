#include "Server.hpp"
#include "Util.hpp"
#include "Client.hpp"

Server *s = NULL;

int exit_error(std::string msg)
{
	std::cerr << "Error: " << msg << std::endl;
	exit(0);
	return (1);
}

void signal_handler(int signal)
{
	if (s != nullptr)
		s->getSocket()->emitAll("com^Dman^Dd\n");
  	exit(1);
}

int main(int argc, char **argv)
{
	(void) argv;
	std::signal(SIGINT, signal_handler);
	if (argc == 1)
	{
		
		s = new Server("localhost", 1234);
		s->run();
	}
	else
	{
		Client c("localhost", 1234);
		c.run();
	}
    return  (1);
}
