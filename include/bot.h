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


#ifndef BOT_CLASS
#define BOT_CLASS

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <libplugin.h>

#include <server.h>
#include <user.h>
#include <module.h>
#include <logs.h>
#include <module_info.h>

#include <filesystem.h>

#include <classes.h>

#include <event.h>

class Bot
{
	public:
		Bot();
		~Bot();
		
		// User managing function
		
		bool user_exists(std::string id);
		User user_get_by_id(std::string id);
		void user_add(User u);
		void user_del(std::string id);
		bool user_match(std::string serv, std::string nick);
		bool user_match(std::string serv, std::string nick, std::string host);
		User user_get_by_nick(std::string serv, std::string nick);
		User user_get_by_nick_host(std::string serv, std::string nick, std::string host);
		User* user_get_ptr_by_id(std::string id);
		User* user_get_ptr_by_nick(std::string serv, std::string nick);
		User* user_get_ptr_by_nick_host(std::string serv, std::string nick, std::string host);
		void reload_users();
		void update_user(User user);
		void update_users();
		void trim_users();
		
		// Modules managing functions
		void trim_modules();
		bool load_module(std::string module);
		void unload_module(std::string module);
		bool reload_module(std::string module);
		void reload_module_later(std::string module);
		void unload_module_later(std::string module);
		bool mod_is_loaded(std::string module);
		
		std::vector<User> get_userlist();
		std::vector<Server> get_servlist();
		std::vector<Module> get_modlist();
		
		void parse(Server& serv);
		
		void load_config();
		void load_users();
		void load_modules();
		
		void start();
		void stop();
		
		void main_loop();
		void do_postponed_actions();
		
	private:
		std::vector<Server> servlist;
		std::vector<User> userlist;
		std::vector<Module> modlist;
		
		std::vector<std::string> modules_to_reload;
		std::vector<std::string> modules_to_unload;
		
		bool alive;
};

#endif
