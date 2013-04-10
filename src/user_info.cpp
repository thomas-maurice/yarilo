/*
 *
 *	Copyright (C) 2012 Thomas MAURICE
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License along
 *	with this program; if not, write to the Free Software Foundation, Inc.,
 *	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include <user_info.h>

using namespace std;

User_info::User_info()
{

}

bool User_info::is_op_on_channel(std::string channel)
{
	for(unsigned int i = 0;i < channels.size(); i++)
	{
		if(channels.at(i).at(0) == channel && channels.at(i).at(1) == "@")
		{
			return true;
		}
	}
	
	return false;
}

bool User_info::is_hop_on_channel(std::string channel)
{
	if(is_op_on_channel(channel)) return true;
	
	for(unsigned int i = 0;i < channels.size(); i++)
	{
		if(channels.at(i).at(0) == channel && channels.at(i).at(1) == "%")
		{
			return true;
		}
	}
	
	return false;
}

bool User_info::is_voice_on_channel(std::string channel)
{
	if(is_hop_on_channel(channel)) return true;
	
	for(unsigned int i = 0;i < channels.size(); i++)
	{
		if(channels.at(i).at(0) == channel && channels.at(i).at(1) == "+")
		{
			return true;
		}
	}
	
	return false;
}

bool User_info::is_on_channel(std::string channel)
{
	for(unsigned int i = 0;i < channels.size(); i++)
	{
		if(channels.at(i).at(0) == channel)
		{
			return true;
		}
	}
	
	return false;
}

std::string User_info::get_nick()
{
	return nick;
}

void User_info::set_nick(std::string p_nick)
{
	nick = p_nick;
}

std::string User_info::get_host()
{
	return host;
}

std::string User_info::get_user_at_host()
{
	return user + "@" + host;
}

void User_info::set_host(std::string p_host)
{
	host = p_host;
}

std::string User_info::get_user()
{
	return user;
}

void User_info::set_user(std::string p_user)
{
	user = p_user;
}

std::vector<std::vector<std::string> > User_info::get_channels()
{
	return channels;
}

void User_info::set_channels(std::vector<std::vector<std::string> > p_channels)
{
	channels = p_channels;
}

bool User_info::is_on_server()
{
	return exists;
}

bool User_info::get_exists()
{
	return exists;
}

void User_info::set_exists(bool e)
{
	exists = e;
}
