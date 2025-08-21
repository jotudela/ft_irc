/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_getters.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rita <rita@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:30:18 by rita              #+#    #+#             */
/*   Updated: 2025/07/22 18:30:18 by rita             ###   ########.fr       */
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

//* Getters :
int Server::getPort() const
{
	return _port;
}

const string& Server::getTopic() const
{
	return _topic;
}

const string& Server::getPassWord() const
{
	return _passWord;
}

const std::string& Server::getAdminNickName() const
{ 
	return _admin.getNickName(); 
}

const std::string& Server::getAdminUserName() const
{
	return _admin.getUserName();
}

const std::string& Server::getStaffNickName() const
{
	return _admin.getNickName();
}

const std::string& Server::getStaffUserName() const
{
	return _admin.getUserName();
}