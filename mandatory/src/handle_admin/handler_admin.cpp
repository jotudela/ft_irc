#include "../../inc/Server.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;

// Mega handler
int Server::handler_admin()
{
	string input = sanitize_input(this->readInput(0));

	if (input == "QUIT")
	{
		run_feature_quit();
		return -1;
	}
	istringstream iss(input);
	string cmd;
	iss >> cmd;
	if (cmd == "MODE")
	{
		std::string subcmd, arg;
		iss >> subcmd;

		if (!run_feature_modes(subcmd, arg, iss)) {
			this->printMsgServer(0, "[ERROR] Invalid or missing syntax for MODE command.");
		}
		return 0;
	}
	else if (cmd == "KICK") //  out of mode FEATURE. Feature : kick (only users and not staff)
		run_feature_kick(iss);
	else if (cmd == "INVITE")
		run_feature_invite(iss);
	else if (cmd == "TOPIC")
		run_feature_topic(iss);
	else if (cmd == "PRIVMSG")
		run_feature_privmsg(iss);
	else
		this->printMsgServer(0, "[ERROR] Command not found : " + cmd + ".");

	return 0;
}
