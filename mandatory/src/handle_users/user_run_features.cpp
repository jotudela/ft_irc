#include "../../inc/Server.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;

// Staff MODE command handler
int Server::user_run_feature_mode(int event_fd, std::istringstream &iss)
{
	string subcmd, nickname;
	iss >> subcmd;

	if (subcmd == "k")
		return user_run_mode_k(event_fd, iss);

	iss >> nickname;

	if (subcmd == "i" && nickname.empty())
		return user_run_mode_i(event_fd);

	if (subcmd == "o" && !nickname.empty() && nickname != _staffs[event_fd]->getNickName())
		return user_run_mode_o(event_fd, nickname);

	if (subcmd == "t" && !nickname.empty() && nickname != _staffs[event_fd]->getNickName())
		return user_run_mode_t(event_fd, nickname);

	if (subcmd == "l")
		return user_run_mode_l(event_fd, nickname);

	// Invalid command
	string err3 = "[ERROR] Invalid syntax for MODE.";
	this->printMsgServer(event_fd, err3);
	return 0;
}

// Staff TOPIC command handler
void Server::user_run_feature_topic(int event_fd, std::string channel_or_topic)
{
	// Trim leading spaces
	channel_or_topic.erase(0, channel_or_topic.find_first_not_of(" \t"));

	if (channel_or_topic.empty())
	{
		if (_topic.empty())
			this->printMsgServer(event_fd, "[TOPIC] None topic defined.");
		else
			this->printMsgServer(event_fd, "[TOPIC] Current topic : " + _topic);
	}
	else
	{
		// Nouveau sujet
		size_t pos = channel_or_topic.find(':');
		if (pos != string::npos)
			_topic = channel_or_topic.substr(pos + 1);
		else
			_topic = channel_or_topic;

		this->printMsgServer(event_fd, "[TOPIC] New topic defined : " + _topic);
	}
}


// Staff INVITE command handler
int Server::user_run_feature_invite(int event_fd, std::istringstream &iss)
{
	string invite_nick;
	iss >> invite_nick;

	if (!invite_nick.empty())
	{
		bool found = false;

		std::map<int, User*>::iterator uit;
		for (uit = _users.begin(); uit != _users.end(); ++uit)
		{
			if (uit->second && uit->second->getNickName() == invite_nick)
			{
				if (_currentUsers >= _userLimit)
				{
					string msg = "Cannot invite " + _users[uit->first]->getNickName() + " : the user limit is reached.";
					this->printMsgServer(event_fd, msg);
					return 0;
				}
				_userStates[uit->first] = JOINED;
				_currentUsers += 1;

				this->printMsgServer(event_fd, "[INVITE] " + invite_nick + " has been invited to join the channel.");

				string notice = invite_nick + " : You have been invited to join the channel.";
				this->printMsgServer(uit->first, notice);

				found = true;
				break;
			}
		}

		if (!found)
			this->printMsgServer(event_fd, "[ERROR] No users found with the nickname : " + invite_nick + ".");
		return 0;
	}
	else
	{
		this->printMsgServer(event_fd, "[ERREUR] Syntaxe : INVITE <nickname>.");
		return 0;
	}
}


// Staff KICK command handler
int Server::user_run_feature_kick(int event_fd, std::istringstream &iss)
{
	string kick_nick;
	iss >> kick_nick;

	if (!kick_nick.empty())
	{
		int fdToKick = -1;
		std::map<int, User*>::iterator it;
		for (it = _users.begin(); it != _users.end(); ++it)
		{
			if (it->second->getNickName() == kick_nick)
			{
				fdToKick = it->first;
				break;
			}
		}

		if (fdToKick != -1)
		{
			_userStates[fdToKick] = REGISTERED;
			_currentUsers -= 1;
			string kickMsg = "You are kick from channel : " + getTopic();
			this->printMsgServer(fdToKick, kickMsg);
			this->printMsgServer(event_fd, "[KICK] " + kick_nick + " has been kicked from channel.");
		}
		else
			this->printMsgServer(event_fd, "[ERROR] No users found with the nickname : " + kick_nick + ".");
	}
	else
		this->printMsgServer(event_fd, "[ERROR] Syntaxe : KICK <nickname>.");
	
	return 0;
}

// Handle PRIVMSG command for users and staff
int Server::user_run_feature_privmsg(int event_fd, std::istringstream &iss, const std::string &command)
{
	// Récupère le reste de la ligne après la commande
	string rest;
	getline(iss, rest);

	// Nettoyer les espaces en début
	rest.erase(0, rest.find_first_not_of(" \t"));

	// Séparer le message (commence par :)
	size_t msg_pos = rest.find(" :");
	string target, msg;

	if (msg_pos != string::npos)
	{
		target = rest.substr(0, msg_pos);
		msg = rest.substr(msg_pos + 2); // saute " :"
	}
	else
	{
		target = rest;
		msg = "";
	}

	// Trim le target
	target.erase(0, target.find_first_not_of(" \t"));
	target.erase(target.find_last_not_of(" \t") + 1);

	// Vérification
	if (command == "PRIVMSG")
	{
		if (target.empty() || msg.empty())
		{
			string err = "[ERROR] PRIVMSG : Not enough parameters.";
			this->printMsgServer(event_fd, err);
			return -1;
		}
	}

	if ((_users.count(event_fd) || _staffs.count(event_fd)) && command == "PRIVMSG")
	{
		// Vérifie que l'expéditeur est valide et récupère son nickname
		string nickname;
		void (Server::*function)(int, const string &, const string &) const;
		if (_users.count(event_fd) && _users[event_fd])
		{
			nickname = _users[event_fd]->getNickName();
			function = &Server::printMsgUser;
		}
		else if (_staffs.count(event_fd) && _staffs[event_fd])
		{
			nickname = _staffs[event_fd]->getNickName();
			function = &Server::printMsgStaff;
		}
		else
			return -1; // Expéditeur non valide

		// Recherche d'un destinataire avec ce nick
		int receiver_fd = -1;
		std::map<int, User*>::iterator uit;
		for (uit = _users.begin(); uit != _users.end(); ++uit)
		{
			if (uit->second && uit->second->getNickName() == target)
			{
				receiver_fd = uit->first;
				break;
			}
		}

		if (receiver_fd == -1)
		{
			std::map<int, Admin*>::iterator ait;
			for (ait = _staffs.begin(); ait != _staffs.end(); ++ait)
			{
				if (ait->second && ait->second->getNickName() == target)
				{
					receiver_fd = ait->first;
					break;
				}
			}
		}

		string fullMsg = "MP to you : " + msg + "\r\n";
		if (target == this->getAdminNickName())
			(this->*function)(1, fullMsg, nickname);
		else if (receiver_fd != -1)
			(this->*function)(receiver_fd, fullMsg, nickname);
		else
		{
			string topic = this->getTopic();
			if (target != topic && target != "#" + topic)
			{
				string err = "[ERROR] PRIVMSG " + target + " : No such nick/channel.";
				this->printMsgServer(event_fd, err);
			}
		}
	}
	return 0;
}