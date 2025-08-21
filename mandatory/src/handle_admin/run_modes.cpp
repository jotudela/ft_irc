#include "../../inc/Server.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;

// Modes :

// TODO expected behavior : should update the password.
void Server::run_mode_k(std::string &newPassword) {
	// string rest;
	// getline(iss, rest);                                        // récupère tout ce qui reste après "k"
	
	// Trim leading spaces
	newPassword.erase(0, newPassword.find_first_not_of(" \t"));

	if (newPassword.empty())
	{
		_passWord = "";
		this->printMsgServer(0, "[MODE k] Password deleted.");
	}
	else
	{
		_passWord = newPassword;
		this->printMsgServer(0, "[MODE k] The new password is : " + _passWord + ".");
	}   
}

void Server::run_mode_i() {
	_invite = !_invite;                             // update to the oposite state. 
	this->printMsgServer(0, "[MODE i] Channel mode invitation " + string(_invite ? "ON." : "OFF."));   
}

void Server::run_mode_o(std::string nickname) {
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
					this->printMsgServer(0, "[PROMOTION] " + nickname + " is now staff.");
					this->printMsgServer(fd, "[PROMOTION] " + nickname + " you are now staff.");
				}
				else
					this->printMsgServer(0, "[ERROR] Impossible to promote " + nickname + ".");
				return ;
			}
			else
				this->printMsgServer(0, "[ERROR] " + nickname + " is not in channel.");
			return ;
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
					this->printMsgServer(0, "[DEMOTION] " + nickname + " has become again user.");
					this->printMsgServer(fd, "[DEMOTION] " + nickname + " you become again user.");
				}
				else
					this->printMsgServer(0, "[ERROR] Impossible to downgrade " + nickname);
				return ;
			}
			else
				this->printMsgServer(0, "[ERROR] " + nickname + " is not in channel.");
			return ;
		}
	}

	this->printMsgServer(0, "[ERROR] No users found with the nickname : " + nickname + ".");
}

void Server::run_mode_t(std::string nickname) {
	std::map<int, Admin*>::iterator sit;
	for (sit = _staffs.begin(); sit != _staffs.end(); ++sit)
	{
		if (sit->second->getNickName() == nickname)
		{
			if (sit->second->getTStatus() == true)
			{
				sit->second->setTStatus(false);
				string msg = "You are no longer entitled to the topic command.";
				this->printMsgServer(0, nickname + " is no longer entitled to the topic command.");
				this->printMsgServer(sit->first, msg);
				return ;
			}
			else if (sit->second->getTStatus() == false)
			{
				sit->second->setTStatus(true);
				string msg = "You have rights again for the topic command.";
				this->printMsgServer(0, nickname + " have rights again for the topic command.");
				this->printMsgServer(sit->first, msg);
				return ;
			}
		}
	}
	this->printMsgServer(0, "[ERROR] No admin found with the nickname : " + nickname + ".");
}

void Server::run_mode_l(std::string nickname) {
	if (nickname.empty())  // Pas de paramètre => suppression de la limite
	{
		_userLimit = -1;
		this->printMsgServer(0, "[MODE l] User limit removed.");
	}
	else  // MODE l <nombre>
	{
		istringstream numStream(nickname);
		int limit;
		numStream >> limit;
		if (limit < _currentUsers)
			this->printMsgServer(0, "[MODE l] Cannot define limit of users under number of current users.");
		else if (limit > -1)
		{
			_userLimit = limit;
			std::ostringstream oss;
			oss << limit;
			string strLimit = oss.str();
			this->printMsgServer(0, string("[MODE] User limit defined at ") + strLimit + ".");
		}
		else
			this->printMsgServer(0, "[ERROR] Invalid value for the limit.");
	}
}
