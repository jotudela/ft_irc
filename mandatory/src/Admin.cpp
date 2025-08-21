#include "../inc/Admin.hpp"

using std::string;
using std::cout;
using std::endl;

Admin::Admin() : _nickName(""), _userName("")
{
	_admin = false;
	_staff = false;
}

Admin::~Admin(){}

void Admin::setAdminStatus()
{
	_admin = true;
	_staff = false;
}

void Admin::setStaffStatus()
{
	_staff = true;
	_admin = false;
}

void Admin::setStatus( bool registered )
{
	_registered = registered;
}

void Admin::setTStatus( bool topic )
{
	_topic = topic;
}

void Admin::setAdminNames( const std::string& nickName, const std::string& userName )
{
	_nickName = nickName;
	_userName = userName;
}

void Admin::setStaffNames( const std::string& nickName, const std::string& userName )
{
	_nickName = nickName;
	_userName = userName;
}

bool Admin::getAdminStatus() const
{
	return _admin;
}

bool Admin::getStaffStatus() const
{
	return _staff;
}

bool Admin::getStatus() const
{
	return _registered;
}

bool Admin::getTStatus() const
{
	return _topic;
}

const std::string& Admin::getNickName() const
{
	return _nickName;
}

const std::string& Admin::getUserName() const
{
	return _userName;
}