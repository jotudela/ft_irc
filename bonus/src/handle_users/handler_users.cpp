#include "../../inc/Server.hpp"
#include "../../inc/Admin.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;

// Mega Handler
// focus on Users and staff members (no Admin)
int Server::handler_users( int event_fd )
{
	string msg_input = this->readInput(event_fd);
	if (msg_input.empty())
		return handle_client_disconnection(event_fd);

	string input = sanitize_input(msg_input);

	// States handlers.
	if (_userStates[event_fd] == REGISTERED || _userStates[event_fd] == JOINED || _staffStates[event_fd] == JOINED)
	{
		istringstream iss(input);
		string command;
		iss >> command;

		if (_staffStates[event_fd] == JOINED)  //* only staff (comand)
		{
			if (command == "MODE")
				return (user_run_feature_mode(event_fd, iss));
			else if (command == "KICK")
				return (user_run_feature_kick(event_fd, iss));
			else if (command == "INVITE")
				return (user_run_feature_invite(event_fd, iss));
			else if (command == "TOPIC" && _staffs[event_fd]->getTStatus() == true)
			{
				string channel_or_topic;
				getline(iss, channel_or_topic);
				user_run_feature_topic(event_fd, channel_or_topic);
				return 0;
			}
		}
		if (command == "PRIVMSG")
			user_run_feature_privmsg(event_fd, iss, command);
	}
	return (handle_user_states(event_fd, input));
}
