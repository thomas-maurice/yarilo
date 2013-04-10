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

#include <iostream>
#include <vector>

#ifndef USERINFO
#define USERINFO

class User_info
{
	public:
		User_info();
		
		bool is_op_on_channel(std::string channel);
		bool is_hop_on_channel(std::string channel);
		bool is_voice_on_channel(std::string channel);
		bool is_on_channel(std::string channel);
		bool is_on_server();
		
		bool get_exists();
		void set_exists(bool e);
		
		std::string get_nick();
		void set_nick(std::string p_nick);

		std::string get_host();
		void set_host(std::string p_host);

		std::string get_user();
		void set_user(std::string p_user);
		
		std::string get_user_at_host();
		
		std::vector<std::vector<std::string> > get_channels();
		void set_channels(std::vector<std::vector<std::string> > p_channels);

	private:
		std::string nick;
		std::string user;
		std::string host;
		bool exists;
		std::vector<std::vector<std::string> > channels;
};

#endif
