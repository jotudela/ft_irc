#include "../inc/Server.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cerr;

void print_irc()
{
	cout << "\033[34m";
	cout << "FFFFFFFFFFFFFFFFFFFFFFTTTTTTTTTTTTTTTTTTTTTTT                        IIIIIIIIIIRRRRRRRRRRRRRRRRR           CCCCCCCCCCCCC" << endl;
	cout << "F::::::::::::::::::::FT:::::::::::::::::::::T                        I::::::::IR::::::::::::::::R       CCC::::::::::::C" << endl;
	cout << "F::::::::::::::::::::FT:::::::::::::::::::::T                        I::::::::IR::::::RRRRRR:::::R    CC:::::::::::::::C" << endl;
	cout << "FF::::::FFFFFFFFF::::FT:::::TT:::::::TT:::::T                        II::::::IIRR:::::R     R:::::R  C:::::CCCCCCCC::::C" << endl;
	cout << "  F:::::F       FFFFFFTTTTTT  T:::::T  TTTTTT                          I::::I    R::::R     R:::::R C:::::C       CCCCCC" << endl;
	cout << "  F:::::F                     T:::::T                                  I::::I    R::::R     R:::::RC:::::C              " << endl;
	cout << "  F::::::FFFFFFFFFF           T:::::T                                  I::::I    R::::RRRRRR:::::R C:::::C              " << endl;
	cout << "  F:::::::::::::::F           T:::::T                                  I::::I    R:::::::::::::RR  C:::::C              " << endl;
	cout << "  F:::::::::::::::F           T:::::T                                  I::::I    R::::RRRRRR:::::R C:::::C              " << endl;
	cout << "  F::::::FFFFFFFFFF           T:::::T                                  I::::I    R::::R     R:::::RC:::::C              " << endl;
	cout << "  F:::::F                     T:::::T                                  I::::I    R::::R     R:::::RC:::::C              " << endl;
	cout << "  F:::::F                     T:::::T                                  I::::I    R::::R     R:::::R C:::::C       CCCCCC" << endl;
	cout << "FF:::::::FF                 TT:::::::TT                              II::::::IIRR:::::R     R:::::R  C:::::CCCCCCCC::::C" << endl;
	cout << "F::::::::FF                 T:::::::::T                              I::::::::IR::::::R     R:::::R   CC:::::::::::::::C" << endl;
	cout << "F::::::::FF                 T:::::::::T                              I::::::::IR::::::R     R:::::R     CCC::::::::::::C" << endl;
	cout << "FFFFFFFFFFF                 TTTTTTTTTTT                              IIIIIIIIIIRRRRRRRR     RRRRRRR        CCCCCCCCCCCCC" << endl;
	cout << "                                             ________________________                                                   " << endl;
	cout << "                                             _::::::::::::::::::::::_                                                   " << endl;
	cout << "                                             ________________________                                                   " << endl;
	cout << "\033[0m";
	cout << endl << endl << endl;
}

int main( int ac, char **av )
{
	if (ac != 3)
	{
		cerr << "Good usage is : ./ircserv <port> <password>" << endl;
		return 1;
	}
	print_irc();
	Server serv(std::atoi(av[1]), av[2]);
	serv.setUpServer();
	serv.handler_irc();
}
/*
REMINDER ref : 
	- https://datatracker.ietf.org/doc/html/rfc1459#section-1.2
	- https://www.rfc-editor.org/rfc/rfc2812#page-6
*/