#include "../../inc/Server.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;

// Main user states handler
int Server::handle_user_states(int event_fd, const std::string &input)
{
    switch (_userStates[event_fd])
    {
        case WAIT_NICK:
            handle_wait_nick_state(event_fd, input);
            break;
        case WAIT_USER:
            handle_wait_user_state(event_fd, input);
            break;
        case REGISTERED:
            handle_registered_state(event_fd, input);
            break;
        case WAIT_PASSWORD:
            handle_wait_password_state(event_fd, input);
            break;
        case JOINED:
            handle_joined_state(event_fd, input);
            break;
        default:
            break;
    }
    return 0;
}

// Handle WAIT_NICK state
void Server::handle_wait_nick_state(int event_fd, const std::string &input)
{
    istringstream iss(input);
    string command;
    iss >> command;

    string nick;
    getline(iss, nick);
    nick.erase(0, nick.find_first_not_of(" \t"));

    if (command != "NICK" || nick.empty())
    {
        string msg = "[ERROR] No nickname given\r\n";
        this->printMsgServer(event_fd, msg);
    }
    else
    {
        bool nicknameTaken = false;

        // Vérifier si le nickname existe déjà dans les utilisateurs
        for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); ++it)
        {
            if (it->second && it->second->getNickName() == nick)
            {
                nicknameTaken = true;
                break;
            }
        }

        // Vérifier si le nickname existe déjà dans les admins
        if (!nicknameTaken)
        {
            for (std::map<int, Admin*>::iterator it = _staffs.begin(); it != _staffs.end(); ++it)
            {
                if (it->second && it->second->getNickName() == nick)
                {
                    nicknameTaken = true;
                    break;
                }
            }
        }

        // Vérifier si un autre client l'a temporairement pris
        if (!nicknameTaken)
        {
            for (std::map<int, std::string>::iterator it = _tempNick.begin(); it != _tempNick.end(); ++it)
            {
                if (it->first != event_fd && it->second == nick)
                {
                    nicknameTaken = true;
                    break;
                }
            }
        }

        if (!nicknameTaken)
        {
            if (this->getAdminNickName() == nick)
                nicknameTaken = true;
        }

        if (nicknameTaken)
        {
            string err = "[ERRROR] " + nick + " : Nickname is already in use.";
            this->printMsgServer(event_fd, err);
        }
        else
        {
            _tempNick[event_fd] = nick;
            _userStates[event_fd] = WAIT_USER;
        }
    }
}

// Handle WAIT_USER state
void Server::handle_wait_user_state(int event_fd, const std::string &input)
{
    istringstream iss(input);
    string command, username, hostname, servername, realname;
    iss >> command >> username >> hostname >> servername;
    getline(iss, realname);
    if (!realname.empty() && realname[0] == ' ')
        realname.erase(0,1);
    if (command != "USER" || username.empty())
    {
        string msg = "[ERROR] Not enough parameters.";
        this->printMsgServer(event_fd, msg);
    }
    else
    {
        _tempUser[event_fd] = username;
        _userStates[event_fd] = REGISTERED;
        _users[event_fd]->setNickName(_tempNick[event_fd]);
        _users[event_fd]->setUserName(_tempUser[event_fd]);
        _users[event_fd]->setStatus(true);

        string welcome = _tempNick[event_fd] + " Welcome to the IRC server!";
        this->printMsgServer(event_fd, welcome);

        _tempNick.erase(event_fd);
        _tempUser.erase(event_fd);
    }
}

// Handle REGISTERED state
void Server::handle_registered_state(int event_fd, const std::string &input)
{
    istringstream iss(input);
    string command;
    iss >> command;

    if (command == "JOIN")
    {
        string channel;
        getline(iss, channel);
        channel.erase(0, channel.find_first_not_of(" \t"));
        if (channel.empty())
        {
            string msg = "[ERROR] Not enough parameters.";
            this->printMsgServer(event_fd, msg);
            return;
        }
        if (channel != this->getTopic())
        {
            string msg = "[ERROR] " + channel + " : No such channel.";
            this->printMsgServer(event_fd, msg);
            return;
        }
        if (_invite == true)
        {
            string msg = "[ERROR] " + channel + " : This channel is by invitation only.";
            this->printMsgServer(event_fd, msg);
            return;
        }
        if (_currentUsers == _userLimit)
        {
            string err = "[ERROR] " + _users[event_fd]->getNickName() + " #" + _topic + " : Cannot join channel (limit reached).";
            this->printMsgServer(event_fd, err);
            return;
        }
        if (_passWord.empty())
        {
            string msg = "✅ You have joined channel " + this->getTopic() + ".";
            this->printMsgServer(event_fd, msg);

            this->printMsgServer(0, "User " + _users[event_fd]->getNickName()
                + " joined channel: " + this->getTopic() + ".");

            _users[event_fd]->setStatus(true);
            _userStates[event_fd] = JOINED;
            _currentUsers += 1;
            return;
        }

        channel = "Please enter password : ";
        this->printMsgServer(event_fd, channel);
        _userStates[event_fd] = WAIT_PASSWORD;
    }
    else if (command == "PRIVMSG")
    {
        return;
    }
    else
    {
        string msg = "[ERROR] " + command + " : Unknown command.";
        this->printMsgServer(event_fd, msg);
    }
}

// Handle WAIT_PASSWORD state
void Server::handle_wait_password_state(int event_fd, const std::string &input)
{
    string password = input;
    password.erase(remove(password.begin(), password.end(), '\n'), password.end());
    password.erase(remove(password.begin(), password.end(), '\r'), password.end());

    if (password == this->getPassWord())
    {
        string msg = "✅ You have joined channel " + this->getTopic() + ".";
        this->printMsgServer(event_fd, msg);

        this->printMsgServer(0, "User " + _users[event_fd]->getNickName()
                + " joined channel: " + this->getTopic() + ".");

        _users[event_fd]->setStatus(true);
        _userStates[event_fd] = JOINED;
        _currentUsers += 1;
    }
    else
    {
        string msg = "❌ Password invalid!\r\n";
        this->printMsgServer(event_fd, msg);
        _userStates[event_fd] = REGISTERED;
    }
}

// Handle JOINED state
void Server::handle_joined_state(int event_fd, const std::string &input)
{
    istringstream iss(input);
    string command;
    iss >> command;

    // Récupère le reste de la ligne après la commande
    std::string rest;
    std::getline(iss, rest);

    // Nettoyer les espaces en début
    rest.erase(0, rest.find_first_not_of(" \t"));

    // Séparer le message (commence par :)
    size_t msg_pos = rest.find(" :");
    string target, msg;

    if (msg_pos != string::npos) {
        target = rest.substr(0, msg_pos);
        msg = rest.substr(msg_pos + 2); // saute " :"
    } else {
        target = rest;
        msg = "";
    }

    // Trim le target
    target.erase(0, target.find_first_not_of(" \t"));
    target.erase(target.find_last_not_of(" \t") + 1);

    // Vérification
    if (_users.count(event_fd))
    {
        if (command != "PRIVMSG" || target.empty() || msg.empty()) {
            string err = "[ERROR] PRIVMSG : Not enough parameters.";
            this->printMsgServer(event_fd, err);
            return;
        }
    }

    if (target == ("#" + this->getTopic()))
    {
        string nickname;
        void (Server::*function)(int, const string &, const string &) const;

        if (_users.count(event_fd))
        {
            nickname = _users[event_fd]->getNickName();
            function = &Server::printMsgUser;
        }
        else if (_staffs.count(event_fd))
        {
            nickname = _staffs[event_fd]->getNickName();
            function = &Server::printMsgStaff;
        }

        string fullMsg = "Channel " + target + " : " + msg + "\r\n";

        for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); ++it)
        {
            int fd = it->first;
            if (_userStates[fd] == JOINED && fd != event_fd)
                (this->*function)(fd, fullMsg, nickname);
        }
        for (std::map<int, Admin*>::iterator it = _staffs.begin(); it != _staffs.end(); ++it)
        {
            int fd = it->first;
            if (fd != event_fd)
                (this->*function)(fd, fullMsg, nickname);
        }
        (this->*function)(1, fullMsg, nickname);
		if (_OnOff == true)
			bot.parrot(msg, event_fd, getCurrentTime());
    }
}
