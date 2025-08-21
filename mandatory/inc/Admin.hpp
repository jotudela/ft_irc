#pragma once

#include "libs.hpp"

class Admin
{
	private:

		bool _admin;
		bool _staff;
		bool _registered;
		bool _topic;
		std::string _nickName;
		std::string _userName;

		Admin( const Admin & );
		Admin& operator=( const Admin & );

	public:

		Admin();
		~Admin();

		void setAdminStatus();
		void setStaffStatus();
		void setAdminNames( const std::string& nickName, const std::string& userName );
		void setStaffNames( const std::string& nickName, const std::string& userName );
		void setStatus( bool registered );
		void setTStatus( bool topic );

		bool getAdminStatus() const;
		bool getStaffStatus() const;
		bool getStatus() const;
		bool getTStatus() const;

		const std::string& getNickName() const;
		const std::string& getUserName() const;
};