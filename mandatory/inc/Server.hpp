#pragma once

#include "libs.hpp"
#include "Admin.hpp"
#include "User.hpp"

/* 
	An User can have these differents states. 
	When registered can only send PRIVMSG to only 1 user (if its know the nickname). 

	In case the user has the name / password of the channel. Is going to have the status JOINED so it can send public mesages to the channel.
*/
enum InputState { 
	WAIT_NICK = 0,
	WAIT_USER = 1,
	REGISTERED = 2,
	WAIT_PASSWORD = 3,
	JOINED = 4
};

class Server
{
	private:

		// config server :
		int _port;
		int _serverFd;
		int _epollFD;
		int _opt;

		int _userLimit;
		int _currentUsers;
		std::string _topic;
		std::string _passWord;

		bool _running;
		bool _invite;

		Admin _admin;
		std::map<int, Admin *> _staffs;
		std::map<int, int> _staffStates;
		std::map<int, User *> _users;
		std::map<int, int> _userStates;
		std::map<int, std::string> _tempNick;
		std::map<int, std::string> _tempUser;

		struct sockaddr_in _serverAddr;
		struct epoll_event _events[MAX_EVENTS];
		char _buffer[IRC_MESSAGE_MAX];

		Server( const Server & );
		Server& operator=( const Server & );

	public:

		Server( int port, std::string passWord );
		~Server();

		const std::string& getPassWord() const;
		const std::string& getTopic() const;
		int getPort() const;

		const std::string& getAdminNickName() const;
		const std::string& getAdminUserName() const;
		const std::string& getStaffNickName() const;
		const std::string& getStaffUserName() const;

		void makeNonBlocking(int fd);
		void setUpServer();

		// XXL BIG BOSS Mega handler (highest point in the iceberg)
		void handler_irc();
		
		//-------
		// Admin
		//-------
		// Mega handler : Modes, features				//* only Admins
		int handler_admin();							// == HCA
		//* from HCA => Mode feature.
		bool run_feature_modes(const std::string& subcmd, std::string& arg, std::istringstream& iss);
		void run_mode_k(std::string &newPassword);
		void run_mode_i();
		void run_mode_o(std::string nickname);
		void run_mode_t(std::string nickname);
		void run_mode_l(std::string nickname);
		void run_feature_quit();
		void run_feature_kick(std::istringstream &iss);
		void run_feature_invite(std::istringstream &iss);
		void run_feature_topic(std::istringstream &iss);
		void run_feature_privmsg(std::istringstream &iss);

		//------
		// User 
		//------
		//Mega handler : Modes, commands				//* Only Users - staff(0/1)
		int handler_users( int event_fd );		// == HCU
		
		int handle_client_disconnection(int event_fd);	// disconection & clean up
		std::string sanitize_input(const std::string& raw_input);
		//* From HCU => features
		void user_run_feature_topic(int event_fd, std::string channel_or_topic);
		int user_run_feature_invite(int event_fd, std::istringstream &iss);
		int user_run_feature_kick(int event_fd, std::istringstream &iss);
		int user_run_feature_mode(int event_fd, std::istringstream &iss);
		int user_run_feature_privmsg(int event_fd, std::istringstream &iss, const std::string &command);
		//* HCU => modes
		int user_run_mode_k(int event_fd, std::istringstream &iss);
		int user_run_mode_i(int event_fd);
		int user_run_mode_o(int event_fd, const std::string &nickname);
		int user_run_mode_t(int event_fd, const std::string &nickname);
		int user_run_mode_l(int event_fd, const std::string &nickname);
		//* HCU => states
		int handle_user_states(int event_fd, const std::string &input);
		void handle_wait_nick_state(int event_fd, const std::string &input);
		void handle_wait_user_state(int event_fd, const std::string &input);
		void handle_registered_state(int event_fd, const std::string &input);
		void handle_wait_password_state(int event_fd, const std::string &input);
		void handle_joined_state(int event_fd, const std::string &input);



		static void sigint_handler( int signal );
		void freeData();
		int createNewUser();
		int userToStaff(int fd);
		int staffToUser(int fd);

		std::string readInput(int fd);

		void printMsgAdmin(int fd, const std::string& msg) const;
		void printMsgServer(int fd, const std::string& msg) const;
		void printMsgUser(int fd, const std::string& msg, const std::string& nickName) const;
		void printMsgStaff(int fd, const std::string& msg, const std::string& nickName) const;
};