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

#ifndef SERVER_CLASS
#define SERVER_CLASS

#include <iostream>
#include <vector>
#include <libtsocket.h>
#include <sstream>
#include <logs.h>
#include <user_info.h>

class Server
{
	public:
		Server(std::string p_label, std::string p_host, int p_port = 6667, std::string p_nick = "Program", std::string p_ident = "bot", std::string p_pass = "", bool p_ssl = false);
		~Server();
		
		void connect();
		bool is_connected();
		void disconnect();
		
		bool can_getline();
		std::string getline();
		
		void identify();
		
		// Send commands
		int send(std::string dat);
		int write_line(std::string dat);
		
		int privmsg(std::string cible, std::string message);
		int join(std::string chan);
		int part(std::string chan, std::string reason = "");
		int quit(std::string reason = "");
		int notice(std::string cible, std::string message);
		int newnick(std::string n);
		int kick(std::string cible, std::string chan, std::string message = "");
		int ctcp(std::string cible, std::string type, std::string param);
		int mode(std::string channel, std::string cible, std::string modes);
		int action(std::string cible, std::string action);
		
		// Get user info
		User_info get_user_info(std::string nick);
		
		// Has timed out ?
		time_t get_last_reconnection_attempt();
		bool has_timed_out(int sec);
		void try_reconnect();
		
		// Accessors
		std::string get_host();
		void set_host(std::string p_host);
		std::string get_label();
		void set_label(std::string p_label);
		std::string get_nick();
		void set_nick(std::string p_nick);
		std::string get_pass();
		void set_pass(std::string p_pass);
		std::string get_ident();
		void set_ident(std::string p_ident);
		int get_port();
		void set_port(int p_port);

	private:
		Client_socket *socket;
		int port;
		std::string host;
		std::string label;
		std::string nick;
		std::string pass;
		std::string ident;
		std::vector<std::string> buffer;
		bool ssl;
		time_t last_reco_attempt;
		bool connected;
		
		
};

#endif
