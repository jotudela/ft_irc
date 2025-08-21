/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_tools_print.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotudela <jotudela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:30:35 by rita              #+#    #+#             */
/*   Updated: 2025/07/28 16:15:12 by jotudela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Admin.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::remove;
using std::istringstream;

static string getCurrentTime()
{
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%H:%M", t);
	return string(buffer);
}

void Server::printMsgServer( int fd, const string& msg ) const
{
	if (fd != 0)
	{
		write(fd, RED, strlen(RED));
		write(fd, "(SERVER", 8);
		write(fd, RESET, strlen(RESET));
		write(fd, "🖥", 5);
		write(fd, RED, strlen(RED));
		write(fd, ")", 1);
		write(fd, RESET, strlen(RESET));
		write(fd, " ", 1);

		string time = getCurrentTime();
		write(fd, time.c_str(), time.length());

		write(fd, "\n", 1);
		write(fd, msg.c_str(), msg.length());
		write(fd, "\r\n", 2);
		return ;
	}
	write(1, RED, strlen(RED));
	write(1, "(SERVER", 8);
	write(1, RESET, strlen(RESET));
	write(1, "🖥", 5);
	write(1, RED, strlen(RED));
	write(1, ")", 1);
	write(1, RESET, strlen(RESET));
	write(1, " ", 1);

	string time = getCurrentTime();
	write(1, time.c_str(), time.length());

	write(1, "\n", 1);
	write(1, msg.c_str(), msg.length());
	write(1, "\r\n", 2);
}

void Server::printMsgUser( int fd, const string& msg, const string& nickName ) const
{
	write(fd, CYAN, strlen(CYAN));
	write(fd, "(USER🙂)", 11);
	write(fd, RESET, strlen(RESET));
	write(fd, " ", 1);

	write(fd, nickName.c_str(), nickName.length());
	write(fd, " ", 1);
	string time = getCurrentTime();
	write(fd, time.c_str(), time.length());

	write(fd, "\n", 1);
	write(fd, msg.c_str(), msg.length());
}

void Server::printMsgStaff( int fd, const std::string& msg, const std::string& nickName ) const
{
	write(fd, BLEU, strlen(BLEU));
	write(fd, "(STAFF🤓)", 12);
	write(fd, RESET, strlen(RESET));
	write(fd, " ", 1);

	write(fd, nickName.c_str(), nickName.length());
	write(fd, " ", 1);
	string time = getCurrentTime();
	write(fd, time.c_str(), time.length());

	write(fd, "\n", 1);
	write(fd, msg.c_str(), msg.length());
}

void Server::printMsgAdmin( int fd, const string& msg ) const
{
	if (fd != 0)
	{
		write(fd, VIOLET, strlen(VIOLET));
		write(fd, "(ADMIN😎)", 12);
		write(fd, RESET, strlen(RESET));
		write(fd, " ", 1);

		string nick = this->getAdminNickName();
		write(fd, nick.c_str(), nick.length());
		write(fd, " ", 1);
		string time = getCurrentTime();
		write(fd, time.c_str(), time.length());

		write(fd, "\n", 1);
		write(fd, msg.c_str(), msg.length());
		return ;
	}
	cout << VIOLET "(ADMIN😎)" RESET
		<< " " + this->getAdminNickName()
		<< " " + getCurrentTime()
		<< endl
		<< msg;
}