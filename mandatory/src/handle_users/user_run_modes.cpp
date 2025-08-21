#include "../../inc/Server.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;

// MODE k handler
int Server::user_run_mode_k(int event_fd, std::istringstream &iss)
{
    string rest;
    getline(iss, rest);
    
    // Trim leading spaces
    rest.erase(0, rest.find_first_not_of(" \t"));

    if (rest.empty())
    {
        _passWord = "";
        this->printMsgServer(event_fd, "[MODE k] Password deleted.");
    }
    else
    {
        _passWord = rest;
        this->printMsgServer(event_fd, "[MODE k] The new password is : " + _passWord + ".");
    }
    return 0;
}

// MODE i handler
int Server::user_run_mode_i(int event_fd)
{
    _invite = !_invite;
    this->printMsgServer(event_fd, "[MODE i] Channel mode invitation " + string(_invite ? "ON." : "OFF."));
    return 0;
}

// MODE o handler
int Server::user_run_mode_o(int event_fd, const std::string &nickname)
{
    // Promotion
    std::map<int, User*>::iterator uit;
    for (uit = _users.begin(); uit != _users.end(); ++uit)
    {
        if (uit->second->getNickName() == nickname)
        {
            int fd = uit->first;
            if (_userStates[fd] == JOINED)
            {
                if (userToStaff(fd) == 0)
                {
                    this->printMsgServer(event_fd, "[PROMOTION] " + nickname + " is now staff.");
                    this->printMsgServer(fd, "[PROMOTION] " + nickname + " you are now staff.");
                }
                else
                    this->printMsgServer(event_fd, "[ERROR] Impossible to promote " + nickname + ".");
                return 0;
            }
            else
                this->printMsgServer(event_fd, "[ERROR] " + nickname + " is not in channel.");
            return 0;
        }
    }

    // Démotion
    std::map<int, Admin*>::iterator sit;
    for (sit = _staffs.begin(); sit != _staffs.end(); ++sit)
    {
        if (sit->second->getNickName() == nickname)
        {
            int fd = sit->first;
            if (_staffStates[fd] == JOINED)
            {
                if (staffToUser(fd) == 0)
                {
                    this->printMsgServer(event_fd, "[DEMOTION] " + nickname + " has become again user.");
                    this->printMsgServer(fd, "[DEMOTION] " + nickname + " you become again user.");
                }
                else
                    this->printMsgServer(event_fd, "[ERROR] Impossible to downgrade " + nickname);
                return 0;
            }
            else
                this->printMsgServer(event_fd, "[ERROR] " + nickname + " is not in channel.");
            return 0;
        }
    }

    this->printMsgServer(event_fd, "[ERROR] No users found with the nickname : " + nickname + ".");
    return 0;
}

// MODE t handler
int Server::user_run_mode_t(int event_fd, const std::string &nickname)
{
    std::map<int, Admin*>::iterator sit;
    for (sit = _staffs.begin(); sit != _staffs.end(); ++sit)
    {
        if (sit->second->getNickName() == nickname)
        {
            if (sit->second->getTStatus() == true)
            {
                sit->second->setTStatus(false);
                string msg = "You are no longer entitled to the topic command.";
                this->printMsgServer(event_fd, nickname + " is no longer entitled to the topic command.");
                this->printMsgServer(sit->first, msg);
                return 0;
            }
            else if (sit->second->getTStatus() == false)
            {
                sit->second->setTStatus(true);
                string msg = "You have rights again for the topic command.";
                this->printMsgServer(event_fd, nickname + " have rights again for the topic command.");
                this->printMsgServer(sit->first, msg);
                return 0;
            }
        }
    }
    this->printMsgServer(event_fd, "[ERROR] No admin found with the nickname : " + nickname + ".");
    return 0;
}

// MODE l handler
int Server::user_run_mode_l(int event_fd, const std::string &nickname)
{
    if (nickname.empty())  // No parameter => remove limit
    {
        _userLimit = -1;
        this->printMsgServer(event_fd, "[MODE l] User limit removed.");
    }
    else  // MODE l <number>
    {
        istringstream numStream(nickname);
        int limit;
        numStream >> limit;
        if (limit < _currentUsers)
            this->printMsgServer(event_fd, "[MODE l] Cannot define limit of users under number of current users.");
        else if (limit > -1)
        {
            _userLimit = limit;
            std::ostringstream oss;
            oss << limit;
            string strLimit = oss.str();
            this->printMsgServer(event_fd, string("[MODE] User limit defined at ") + strLimit + ".");
        }
        else
            this->printMsgServer(event_fd, "[ERROR] Invalid value for the limit.");
    }
    return 0;
}
