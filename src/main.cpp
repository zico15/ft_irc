#include "Server.hpp"
#include "Util.hpp"
#include "Client.hpp"

Server *s = NULL;

int exit_error(std::string msg)
{
	std::cerr << "Error: " << msg << std::endl;
	delete s;
	exit(0);
	return (1);
}

void signal_handler(int signal)
{
	if (s != nullptr)
		s->emitAll("com^Dman^Dd\n");
	delete s;
  	exit(1);
}

int main(int argc, char **argv)
{
	(void) argv;
	std::signal(SIGINT, signal_handler);
	s = new Server("localhost", 1234);
	s->run();
    return  (1);
}
