#include "Server.hpp"
#include "Util.hpp"
#include "Client.hpp"

int exit_error(std::string msg)
{
	std::cerr << "Error: " << msg << std::endl;
	exit(0);
	return (1);
}

void signal_handler(int signal)
{
  	exit(0);
}

int main(int argc, char **argv)
{
	(void) argv;
	std::signal(SIGINT, signal_handler);
	if (argc == 1)
	{
		
		Server s("localhost", 1234);
		//while (true)
			s.run();
	}
	else
	{
		Client c("localhost", 1234);
		while (true)
			c.run();
	}
    return  (0);
}
