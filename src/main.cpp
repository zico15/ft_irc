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
		s->emitAll("com^Dman^Dd\n");//comando enviado para todos
	delete s;
  	exit(1);
}

int main(int argc, char **argv)
{
	(void) argv;
	std::signal(SIGINT, signal_handler);
	if (argc < 3 || !argv[2])
		std::cout << "ARGS: porta,  password\n";
	else
	{
			s = new Server("localhost", 1234, argv[2]);
	s->run();
	}

    return  (1);
}
