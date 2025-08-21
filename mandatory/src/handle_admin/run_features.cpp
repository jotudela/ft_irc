#include "../../inc/Server.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;

// Xs handler modes
bool Server::run_feature_modes(const std::string& subcmd, std::string& arg, std::istringstream& iss)
{
	string rest;
	getline(iss, rest);
	if (subcmd == "k") {
		run_mode_k(rest);
		return true;
	}
	arg = rest;
	arg.erase(remove(arg.begin(), arg.end(), '\n'), arg.end());
	arg.erase(remove(arg.begin(), arg.end(), ' '), arg.end());
	if (subcmd == "i" && arg.empty()) {
		run_mode_i();
		return true;
	} else if (subcmd == "o" && !arg.empty()) {
		run_mode_o(arg);
		return true;
	} else if (subcmd == "t" && !arg.empty()) {
		run_mode_t(arg);
		return true;
	} else if (subcmd == "l") {
		run_mode_l(arg); // arg may be empty or a number
		return true;
	}

	// Unknown subcommand
	return false;
}

void Server::run_feature_quit() {
	this->printMsgAdmin(0, "Shutting down server.\r\n");
	_running = false;
}

void Server::run_feature_kick(std::istringstream &iss) {
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

		if (fdToKick != -1 && _userStates[fdToKick] == JOINED)
		{
			_userStates[fdToKick] = REGISTERED;
			_currentUsers -= 1;
			string kickMsg = "You are kick from channel : " + getTopic();
			this->printMsgServer(fdToKick, kickMsg);
			this->printMsgServer(0, "[KICK] " + kick_nick + " has been kicked from channel.");
		}
		else
			this->printMsgServer(0, "[ERROR] No users found with the nickname : " + kick_nick + ".");
	}
	else
		this->printMsgServer(0, "[ERROR] Syntaxe : KICK <nickname>.");
}

void Server::run_feature_invite(std::istringstream &iss) {
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
					this->printMsgServer(0, msg);
					// return 0;
					break;
				}
				_userStates[uit->first] = JOINED;
				_currentUsers += 1;
				this->printMsgServer(0, "[INVITE] " + invite_nick + " has been invited to join the channel.");

				string notice = invite_nick + " : You have been invited to join the channel.";
				this->printMsgServer(uit->first, notice);

				found = true;
				break;
			}
		}

		if (!found)
			this->printMsgServer(0, "[ERROR] No users found with the nickname : " + invite_nick + ".");
	}
	else
		this->printMsgServer(0, "[ERREUR] Syntaxe : INVITE <nickname>.");
}

void Server::run_feature_topic(std::istringstream &iss) {
	string channel_or_topic;
	getline(iss, channel_or_topic);

	// Trim leading spaces
	channel_or_topic.erase(0, channel_or_topic.find_first_not_of(" \t"));

	if (channel_or_topic.empty())
	{
		if (_topic.empty())
			this->printMsgServer(0, "[TOPIC] None topic defined.");
		else
			this->printMsgServer(0, "[TOPIC] Current topic : " + _topic);
	}
	else
	{
		// Nouveau sujet
		size_t pos = channel_or_topic.find(':');
		if (pos != string::npos)
			_topic = channel_or_topic.substr(pos + 1);
		else
			_topic = channel_or_topic;

		this->printMsgServer(0, "[TOPIC] New topic defined : " + _topic);
	}
}

void Server::run_feature_privmsg(std::istringstream &iss)
{
	string rest;
	getline(iss, rest);

	// Nettoyer les espaces en début
	rest.erase(0, rest.find_first_not_of(" \t"));

	// Séparer la cible et le message (message commence par :)
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

	// Nettoyage
	target.erase(0, target.find_first_not_of(" \t"));
	target.erase(target.find_last_not_of(" \t") + 1);

	// Vérif de base
	if (target.empty() || msg.empty())
	{
		cout << "[PRIVMSG] Erreur : PRIVMSG <cible> :<message>\n";
		return ;
	}

	string adminNick = this->getAdminNickName();

	// Cas 1 : message vers un canal
	if (target == "#" + this->getTopic())
	{
		string fullMsg = string("Channel ") + ("#") + _topic + " : " + msg + "\r\n";

		// Envoyer aux users
		for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); ++it)
		{
			int fd = it->first;
			if (_userStates[fd] == JOINED)
				this->printMsgAdmin(fd, fullMsg);
		}

		// Envoyer aux autres staffs
		for (std::map<int, Admin*>::iterator it = _staffs.begin(); it != _staffs.end(); ++it)
			this->printMsgAdmin(it->first, fullMsg);
		return ;
	}

	// Cas 2 : message privé à un user/staff
	int receiver_fd = -1;

	// Cherche parmi les users
	for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second && it->second->getNickName() == target)
		{
			receiver_fd = it->first;
			break;
		}
	}

	// Sinon cherche parmi les staffs
	if (receiver_fd == -1)
	{
		for (std::map<int, Admin*>::iterator it = _staffs.begin(); it != _staffs.end(); ++it)
		{
			if (it->second && it->second->getNickName() == target)
			{
				receiver_fd = it->first;
				break;
			}
		}
	}

	string fullMsg = "MP to you : " + msg + "\r\n";

	if (receiver_fd != -1)
		this->printMsgAdmin(receiver_fd, fullMsg);
	else
		this->printMsgServer(0, "[ERROR] User " + target + " not found.");
}
