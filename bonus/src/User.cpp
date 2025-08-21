#include "../inc/User.hpp"

using std::string;
using std::cout;
using std::endl;
using std::cin;

User::User() : _nickName(""), _userName(""), _registered(false){}
User::~User(){}
const string& User::getNickName() const
{
	return _nickName;
}

const string& User::getUserName() const
{
	return _userName;
}

void User::setStatus( bool registered )
{
	_registered = registered;
}

void User::setNickName( const string& nickName )
{
	_nickName = nickName;
}

void User::setUserName( const string& userName )
{
	_userName = userName;
}

bool User::getStatus() const
{
	return _registered;
}
