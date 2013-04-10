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

#ifndef USER_CLASS
#define USER_CLASS

#include <iostream>
#include <vector>
#include <sstream>

class User
{
	public:
		User();
		~User();
		// User infos to str()
		std::string to_string();
		
		// Matches user
		bool match(std::string label, std::string nick); // Nick/serv match only
		bool match(std::string label, std::string nick, std::string host); // Nick/host/serv match (better)
		
		// Match flag
		bool match_flag(char flag);
		bool match_flags(std::string flags);
		
		// Servers list
		std::vector<std::string> get_server_list();
		
		
		// Nick modifiers
		// label stands for the server's label.
		void add_nick(std::string label, std::string nick);
		void del_nick(std::string label, std::string nick);
		void list_nicks();
		
		// Host modifier
		void add_host(std::string label, std::string host);
		void del_host(std::string label, std::string host);
		void list_hosts();
		
		// Accessors
		std::string get_id();
		void set_id(std::string p_id);

		std::string get_pl_nick();
		void set_pl_nick(std::string p_pl_nick);

		std::string get_pl_pass();
		void set_pl_pass(std::string p_pl_pass);

		std::string get_flags();
		void set_flags(std::string p_flags);
		void regen_flags();
		void add_flags(std::string f);
		void add_flag(char flag);
		void del_flag(char flag);
		void del_flags(std::string f);
		void trim_flags();
		
		/*
		 * ---- Flags ----
		 * Bot flags
		 * O : Owner
		 * M : Master
		 * R : Privilegied user lvl 3
		 * Q : Privilegied user lvl 2
		 * P : Privilegied user lvl 1
		 * U : Regular user
		 * N : No command bot access
		 * Channel flags
		 * o : Auto-op
		 * h : Auto-hop
		 * v : Auto-voice
		 * d : Auto-deop
		 * k : Auto-kick
		 * m : Martyr (auto deop/voice/hop & kick & no access to nothing)
		 */
		bool get_pl_access();
		void set_pl_access(bool p_pl_access);
		
		// Equality operator
		
		bool operator==(const User &u2);
	
	private:
		std::string id;
		std::vector<std::vector<std::string> > server_host;
		std::vector<std::vector<std::string> > server_nick;
		bool pl_access;
		std::string pl_nick;
		std::string pl_pass;
		std::string flags;
		
};

#endif
