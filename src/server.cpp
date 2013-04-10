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


#include <server.h>

using namespace std;

Server::Server(std::string p_label, std::string p_host, int p_port, std::string p_nick, std::string p_ident, std::string p_pass, bool p_ssl)
{
	label = p_label;
	host = p_host;
	port = p_port;
	socket = new Client_socket(host, port, p_ssl);
	nick = p_nick;
	pass = p_pass;
	ident = p_ident;
	ssl = p_ssl;
	connected = false;
	time(&last_reco_attempt);
}

void Server::connect()
{
	socket->connect_socket();
	connected = true;
	log("core", "Connecting to " + host);
}

void Server::identify()
{
	if(!connected)
	{
		connect();
	}
	
	write_line("PASS " + pass);
	write_line("USER " + ident + " " + "localhost " + host + " :A Yarilo IRC robot !");
	write_line("NICK " + nick);
	
	log("core", "Identifying on " + host);
}

void Server::disconnect()
{
	connected = false;
	socket->close_socket();
	log("core", "Disconnecting from " + host);
}

bool Server::is_connected()
{
	return socket->is_connected();
}

std::string Server::getline()
{
	connected = socket->is_connected();
	if(buffer.size() != 0)
	{
		string line = buffer.at(0);
		vector<string> tmp;
		for(unsigned int i = 1; i < buffer.size(); i++)
		{
			tmp.push_back(buffer.at(i));
		}
		
		buffer = tmp;
		
		return line;
	}
	
	if(socket->can_getline())
		return socket->getline();
	
	return "";
}

bool Server::can_getline()
{
	return socket->can_getline();
}

int Server::send(std::string dat)
{
	return socket->write(dat);
}

int Server::write_line(std::string dat)
{
	return send(dat + "\n");
}

int Server::privmsg(std::string cible, std::string message)
{
	return write_line("PRIVMSG " + cible + " :" + message);
}

int Server::join(std::string chan)
{
	return write_line("JOIN " + chan);
}

int Server::part(std::string chan, std::string reason)
{
	return write_line("PART " + chan + " " + reason);
}

int Server::quit(std::string reason)
{
	return write_line("QUIT " + reason);
}

int Server::notice(std::string cible, std::string message)
{
	return write_line("NOTICE " + cible + " " + message);
}

int Server::newnick(std::string newn)
{
	return write_line("NICK " + newn);
}

int Server::kick(std::string cible, std::string chan, std::string message)
{
	return write_line("KICK " + chan + " " + cible + " :" + message);
}

int Server::ctcp(std::string cible, std::string type, std::string param)
{
	return privmsg(cible, "\001" + type + " " + param + "\001");
}

int Server::mode(std::string channel, std::string cible, std::string modes)
{
	return write_line("MODE " + channel + " " + modes + " " + cible);
}

int Server::action(std::string cible, std::string action)
{
	return ctcp(cible, "ACTION", action);
}

User_info Server::get_user_info(std::string n)
{
	User_info res;
	
	write_line("WHOIS " + n);
	
	vector<string> tmp;
	
	bool end_whois = false;
	
	while(!end_whois)
	{
		if(socket->can_getline())
		{
			string line = socket->getline();
			string t = line;
			
			istringstream in(t);
			
			int code;
			string serv, me, target;
			
			in >> serv >> code >> me >> target;
			
			if(code == 318)
			{
				end_whois = true;
			}
			else if(code == 431)
			{
				end_whois = true;
				res.set_exists(false);
			}
			else if(code == 401)
			{
				res.set_exists(false);
			}
			else if(code == 311 && target == n)
			{
				string user, host, star, real;
				in >> user >> host >> star;
				std::getline(in, real);
				if(real.length() > 2)
					real = real.substr(2);
				
				res.set_nick(target);
				res.set_user(user);
				res.set_host(host);
				res.set_exists(true);
			}
			else if(code == 319 && target == n)
			{
				std::vector<std::vector<std::string> > channels;
				string chans;
				std::getline(in,chans);
				chans = chans.substr(2);
				
				istringstream in2(chans);
				string tmpstr = "a";
				
				while(tmpstr != "")
				{
					tmpstr = "";
					
					in2 >> tmpstr;
					
					string mode = "";
					string chan = "";
					
					if(tmpstr != "")
					{
						if(tmpstr.at(0) == '@')
						{
							chan = tmpstr.substr(1);
							mode = "@";
							
							vector<string> v;
							v.push_back(chan);
							v.push_back(mode);
							
							channels.push_back(v);
						}
						else if(tmpstr.at(0) == '%')
						{
							chan = tmpstr.substr(1);
							mode = "%";
							
							vector<string> v;
							v.push_back(chan);
							v.push_back(mode);
							
							channels.push_back(v);
						}
						if(tmpstr.at(0) == '+')
						{
							chan = tmpstr.substr(1);
							mode = "+";
							
							vector<string> v;
							v.push_back(chan);
							v.push_back(mode);
							
							channels.push_back(v);
						}
						if(tmpstr.at(0) == '#')
						{
							chan = tmpstr.substr(1);
							mode = "";
							
							vector<string> v;
							v.push_back(chan);
							v.push_back(mode);
							
							channels.push_back(v);
						}
					}
				}
				
				res.set_channels(channels);
			}
			else
			{
				tmp.push_back(line);
			}
		}
	}
	
	for(unsigned int i = 0; i < tmp.size(); i++)
	{
		buffer.push_back(tmp.at(i));
	}
	
	return res;
}

time_t Server::get_last_reconnection_attempt()
{
	return last_reco_attempt;
}

bool Server::has_timed_out(int sec)
{
	return socket->has_timed_out(sec);
}

void Server::try_reconnect()
{
	socket->reconnect_socket();
	identify();
	time(&last_reco_attempt);
}

Server::~Server()
{
	//delete socket;
}

std::string Server::get_host()
{
	return host;
}

void Server::set_host(std::string p_host)
{
	host = p_host;
}
std::string Server::get_label()
{
	return label;
}

void Server::set_label(std::string p_label)
{
	label = p_label;
}
std::string Server::get_nick()
{
	return nick;
}

void Server::set_nick(std::string p_nick)
{
	nick = p_nick;
}
std::string Server::get_pass()
{
	return pass;
}

void Server::set_pass(std::string p_pass)
{
	pass = p_pass;
}
std::string Server::get_ident()
{
	return ident;
}

void Server::set_ident(std::string p_ident)
{
	ident = p_ident;
}

void Server::set_port(int p_port)
{
	port = p_port;
}

int Server::get_port()
{
	return port;
}
