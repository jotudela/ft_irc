#include "../inc/Server.hpp"
#include "../inc/Admin.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;

// REMINDER: "HI" is a main handler that will handle features/comands for Admin & Users-Staff(0/1)
void Server::handler_irc()		// HI
{
	while (_running)
	{
		// epoll wait : wait signal "enter" from user. 
		int n = epoll_wait(_epollFD, _events, MAX_EVENTS, -1);
		if (n == -1)
		{
			perror("epoll_wait");
			break;
		}

		for (int i = 0; i < n; ++i)
		{
			int event_fd = _events[i].data.fd;

			if (event_fd == _serverFd)
			{
				if (this->createNewUser() == -1)
					continue;
			}
			else if (event_fd == STDIN_FILENO)
			{
				if (this->handler_admin() == -1)
					continue;
			}
			else
			{
				if (this->handler_users(event_fd) == -1)
					continue;
			}
		}
	}
	this->freeData();
	close(_epollFD);
	close(_serverFd);
}
