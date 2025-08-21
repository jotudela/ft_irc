#include "../../inc/Server.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;


// Handle client disconnection and cleanup
int Server::handle_client_disconnection(int event_fd)
{
    if (_users[event_fd] && _users[event_fd]->getStatus())
    {
        this->printMsgServer(0, "Client disconnected: " + _users[event_fd]->getNickName());
        close(event_fd);
        epoll_ctl(_epollFD, EPOLL_CTL_DEL, event_fd, NULL);
        delete _users[event_fd];
        _users.erase(event_fd);
        _userStates.erase(event_fd);
        _tempNick.erase(event_fd);
        _tempUser.erase(event_fd);
        _currentUsers -= 1;
        return -1;
    }
    else if (_staffs[event_fd] && _staffs[event_fd]->getStatus())
    {
        this->printMsgServer(0, "Staff disconnected: " + _staffs[event_fd]->getNickName());
        close(event_fd);
        epoll_ctl(_epollFD, EPOLL_CTL_DEL, event_fd, NULL);
        delete _staffs[event_fd];
        _staffs.erase(event_fd);
        _staffStates.erase(event_fd);
        _currentUsers -= 1;
        return -1;
    }
    return 0; // No disconnection occurred
}