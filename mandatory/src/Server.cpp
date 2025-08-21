#include "../inc/Server.hpp"
#include "../inc/Admin.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;

//* Destructor : 
Server::~Server(){}

//* constructor :
Server::Server( int port, string passWord ) : _port(port), _serverFd(-1),
_epollFD(-1), _userLimit(10), _currentUsers(0), _passWord(passWord),
_running(true), _invite(false)
{
	string nickName("admin");
	string userName("ircadmin");

	_topic = "The Best Channel !";

	_admin.setAdminStatus();
	_admin.setAdminNames(nickName, userName);

	cout << RED "[/!\\ INFO /!\\] " << RESET;
	cout << "Nick name admin : " << nickName << ".\n" << endl;
	cout << RED "[/!\\ INFO /!\\] " << RESET;
	cout << "User name admin : " << userName << ".\n" << endl;
	cout << RED "[/!\\ INFO /!\\] " << RESET;
	cout << "Topic of channel : " << _topic << ".\n" << endl;
}

// CTRL + D
std::string Server::readInput(int fd)
{
	ssize_t nbytes;
	std::string courant;
	std::vector<std::string> paquets;

	memset(_buffer, 0, IRC_MESSAGE_MAX);
	while (true)
	{
		if (fd == 0)
			nbytes = read(fd, _buffer, sizeof(_buffer));  // terminal       // Only Admin terminal 
		else
			nbytes = recv(fd, _buffer, sizeof(_buffer), 0); // clients      // only Staff and users terminal
		if (nbytes == 0)
		{
			// Le client a fermé la connexion
			if (!courant.empty())
				paquets.push_back(courant);
			break;
		}

		for (ssize_t i = 0; i < nbytes; ++i)
		{
			char c = _buffer[i];

			if (c == '\4') { // '\4' == Ctrl+D (EOT: End Of Transmission )
				paquets.push_back(courant);
				courant.clear();
			}
			else if (c == '\n') {
				if (!courant.empty()) {
					paquets.push_back(courant);
					courant.clear();
				}
				std::string result;
				for (size_t i = 0; i < paquets.size(); ++i)
					result += paquets[i];
				return result;
			}
			else {
				courant += c;
			}
		}
	}

	return "";
}

