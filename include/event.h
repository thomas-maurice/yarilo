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

#ifndef EVENT_HEADER
#define EVENT_HEADER

#include <iostream>
#include <vector>

#include <bot.h>
#include <server.h>

// Types d'évènements

#define MOTD    1
#define CONNECT 2
#define PRIVMSG 3
#define JOIN    4
#define PART    5
#define QUIT    6
#define NOTICE  7
#define ACTION  8
#define CTCP    9
#define COMMAND 10
#define NUMERIC 11
#define KICK    12
#define NICK    13
#define MODE    14
#define TIME    15


#define CMD_CHAR '!'
#define SEP_CHAR '.' // Exemple !admin.die

class Event
{
	public:
		Event();
		~Event();
		
		// Accessors
		int get_type();
		void set_type(int p_type);

		Bot* get_bot();
		void set_bot(Bot* p_bot);

		Server* get_serv();
		void set_serv(Server* p_serv);
		
		std::string get_raw();
		void set_raw(std::string p_raw);
		
		User* get_user();
		void set_user(User* p_user);

		std::vector<std::string> get_author();
		void set_author(std::vector<std::string> p_author);
		
		std::string get_message();
		void set_message(std::string p_message);

		std::string get_target();
		void set_target(std::string p_target);

		std::string get_channel();
		void set_channel(std::string p_channel);
		
		std::string get_command();
		void set_command(std::string p_command);

		std::string get_parameters();
		void set_parameters(std::string p_parameters);
		
		std::string get_module();
		void set_module(std::string p_module);
		
		int get_code();
		void set_code(int p_code);
		
		std::string get_newnick();
		void set_newnick(std::string p_newnick);
		
		std::vector<std::string> get_modes();
		void set_modes(std::vector<std::string> p_modes);

		std::vector<std::string> get_nicks();
		void set_nicks(std::vector<std::string> p_nicks);
		
		std::string get_unick();
		std::string get_uhost();

		
	private:
		int type;
		Bot *bot;
		Server *serv;
		User *user;
		std::vector<std::string> author;
		std::string raw;
		std::string message;
		std::string channel;
		std::string target;
		std::string command;
		std::string parameters;
		std::string module;
		std::string newnick;
		std::vector<std::string> modes;
		std::vector<std::string> nicks;
		int code;
};

#endif
