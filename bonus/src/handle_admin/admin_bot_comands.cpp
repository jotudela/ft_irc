#include "../../inc/Server.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;

// Handle BOT ON command
int Server::handle_bot_on()
{
    if (_OnOff == true)
    {
        this->printMsgServer(0, "[ERROR] Bot is already ON.");
        return -1;
    }
    
    _OnOff = true;
    string fullMsg = "Bot Parrot is now ON.\r\n";
    
    broadcast_bot_status_message(fullMsg);
    this->printMsgServer(0, "Bot Parrot is now ON.");
    
    return 0;
}

// Handle BOT OFF command
int Server::handle_bot_off()
{
    if (_OnOff == false)
    {
        this->printMsgServer(0, "[ERROR] Bot is already OFF.");
        return -1;
    }
    
    _OnOff = false;
    string fullMsg = "Bot Parrot is now OFF.\r\n";
    
    broadcast_bot_status_message(fullMsg);
    this->printMsgServer(0, "Bot Parrot is now OFF.");
    
    return 0;
}

// Broadcast bot status message to all joined users and staff
void Server::broadcast_bot_status_message(const std::string &message)
{
    // Send to all joined users
    for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); ++it)
    {
        int fd = it->first;
        if (_userStates[fd] == JOINED)
            this->printMsgAdmin(fd, message);
    }
    
    // Send to all staff
    for (std::map<int, Admin*>::iterator it = _staffs.begin(); it != _staffs.end(); ++it)
        this->printMsgAdmin(it->first, message);
}