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


#include <user.h>

using namespace std;

User::User()
{
	id = "";
	pl_access = false;
	pl_nick = "";
	pl_pass = "";
	flags = "";
}

User::~User()
{
	// No dynamically allocated attributes
	// Nothing to do
}

void User::add_nick(std::string label, std::string nick)
{
	bool serv_exists = false;
	bool nick_exists = false;
	
	for(unsigned int i = 0; i < server_nick.size(); i++)
	{
		if(server_nick.at(i).size() != 0)
		{
			if(server_nick.at(i).at(0) == label)
			{
				serv_exists = true;
				if(server_nick.at(i).size() > 1)
				{
					for(unsigned int j = 1; j < server_nick.at(i).size(); j++)
					{
						if(server_nick.at(i).at(j) == nick)
						{
							nick_exists = true;
							return; // Nothing to do
						}
					}
				}
				
				if(!nick_exists)
				{
					server_nick.at(i).push_back(nick);
					return;
				}
			}
		}
	}
	
	if(!serv_exists)
	{
		vector<string> tmp;
		tmp.push_back(label);
		tmp.push_back(nick);
		server_nick.push_back(tmp);
	}
	
}

void User::del_nick(std::string label, std::string nick)
{
	for(unsigned int i = 0; i < server_nick.size(); i++)
	{
		if(server_nick.at(i).size() != 0)
		{
			if(server_nick.at(i).at(0) == label)
			{
				if(server_nick.at(i).size() > 1)
				{
					for(unsigned int j = 1; j < server_nick.at(i).size(); j++)
					{
						if(server_nick.at(i).at(j) == nick)
						{
							server_nick.at(i).erase(server_nick.at(i).begin()+j);
						}
					}
				}
			}
		}
	}
}

void User::del_host(std::string label, std::string host)
{
	for(unsigned int i = 0; i < server_host.size(); i++)
	{
		if(server_host.at(i).size() != 0)
		{
			if(server_host.at(i).at(0) == label)
			{
				if(server_host.at(i).size() > 1)
				{
					for(unsigned int j = 1; j < server_host.at(i).size(); j++)
					{
						if(server_host.at(i).at(j) == host)
						{
							server_host.at(i).erase(server_host.at(i).begin() + j);
						}
					}
				}
			}
		}
	}
}

void User::add_host(std::string label, std::string host)
{
	bool serv_exists = false;
	bool host_exists = false;
	
	for(unsigned int i = 0; i < server_host.size(); i++)
	{
		if(server_host.at(i).size() != 0)
		{
			if(server_host.at(i).at(0) == label)
			{
				serv_exists = true;
				if(server_host.at(i).size() > 1)
				{
					for(unsigned int j = 1; j < server_host.at(i).size(); j++)
					{
						if(server_host.at(i).at(j) == host)
						{
							host_exists = true;
							return; // Nothing to do
						}
					}
				}
				
				if(!host_exists)
				{
					server_host.at(i).push_back(host);
					return;
				}
			}
		}
	}
	
	if(!serv_exists)
	{
		vector<string> tmp;
		tmp.push_back(label);
		tmp.push_back(host);
		server_host.push_back(tmp);
	}
	
}

std::string User::to_string()
{
	string res;
	res += "# User " + id + " for Yarilo bot\n\n";
	res += "# General settings\n";
	
	res += "ID " + id + "\n\n";
	
	res += "# Servers configuration";
	for(unsigned int i = 0; i < server_nick.size(); i++)
	{
		string serv = server_nick.at(i).at(0);
		res += "\n# Nicknames configuration for " + serv + "\n";
		for(unsigned int j = 1; j < server_nick.at(i).size(); j++)
		{
			res += "SERVER_N " + serv + " " + server_nick.at(i).at(j) + "\n";
		}
	}
	
	for(unsigned int i = 0; i < server_host.size(); i++)
	{
		string serv = server_host.at(i).at(0);
		res += "\n# Hosts configuration for " + serv + "\n";
		for(unsigned int j = 1; j < server_host.at(i).size(); j++)
		{
			res += "SERVER_H " + serv + " " + server_host.at(i).at(j) + "\n";
		}
	}
	
	res += "\n# Partyline configuration\n";
	
	ostringstream o("");
	o << pl_access;
	res += "PL " + o.str() + "\n";
	res += "PL_N " + pl_nick + "\n";
	res += "PL_P " + pl_pass + "\n";
	res += "FLAGS " + flags + "\n\n";
	res += "# End of " + id + " configuration";
	
	return res;
}

void User::list_nicks()
{
	cout << "Nicks for " << id << endl;
	for(unsigned int i = 0; i < server_nick.size(); i++)
	{
		for(unsigned int j = 0; j < server_nick.at(i).size(); j++)
		{
			cout << server_nick.at(i).at(j) << " ";
		}
		cout << endl;
	}
}

bool User::match(std::string label, std::string nick)
{
	bool res = false;
	
	for(unsigned int i = 0; i < server_nick.size(); i++)
	{
		//cout << server_nick.at(i).at(1) << endl;
		if(server_nick.at(i).size() > 0 && server_nick.at(i).at(0) == label)
		{
			for(unsigned int j = 0; j < server_nick.at(i).size(); j++)
			{	
				if(server_nick.at(i).at(j) == nick)
					res = true;
			}
		}
	}
	
	return res;
}

bool User::match(std::string label, std::string nick, std::string host)
{
	bool res_nick = match(label, nick);
	
	if(!res_nick) // If no nickname is matched, then return false
		return false;
	
	// Otherwise let's check if a host is matchedin the list
	
	bool res_host = false;
	
	for(unsigned int i = 0; i < server_host.size(); i++)
	{
		if(server_host.at(i).size() > 0 && server_host.at(i).at(0) == label)
		{
			for(unsigned int j = 0; j < server_host.at(i).size(); j++)
			{
				if(server_host.at(i).at(j) == host)
					res_host = true;
			}
		}
	}
	
	return res_host;
}

void User::list_hosts()
{
	cout << "Hosts for " << id << endl;
	for(unsigned int i = 0; i < server_host.size(); i++)
	{
		for(unsigned int j = 0; j < server_host.at(i).size(); j++)
		{
			cout << server_host.at(i).at(j) << " ";
		}
		cout << endl;
	}
}

void User::regen_flags()
{
	string O,M,R,Q,P,U,N,o,h,v,d,k,m;
	O = "OMRQPU";
	M = "MRQPU";
	R = "RQPU";
	Q = "QPU";
	P = "PU";
	U = "U";
	N = "N";
	o = "ohv";
	h = "hv";
	v = "v";
	d = "d";
	k = "dk";
	m = "dkm";
	
	string uflags = "";
	
	if(flags.find("O") != string::npos)
	{
		uflags = O;
	}
	else if(flags.find("M") != string::npos)
	{
		uflags = M;
	}
	else if(flags.find("R") != string::npos)
	{
		uflags = R;
	}
	else if(flags.find("Q") != string::npos)
	{
		uflags = Q;
	}
	else if(flags.find("P") != string::npos)
	{
		uflags = P;
	}
	else if(flags.find("U") != string::npos)
	{
		uflags = U;
	}
	else if(flags.find("N") != string::npos)
	{
		uflags = N;
	}
	else
	{
	
	}
	
	if(flags.find("o") != string::npos)
	{
		uflags += o;
	}
	else if(flags.find("h") != string::npos)
	{
		uflags += h;
	}
	else if(flags.find("v") != string::npos)
	{
		uflags += v;
	}
	else if(flags.find("m") != string::npos)
	{
		uflags += m;
	}
	else if(flags.find("d") != string::npos)
	{
		uflags += d;
	}
	else if(flags.find("k") != string::npos)
	{
		uflags += k;
	}
	else if(flags.find("m") != string::npos)
	{
		uflags += m;
	}
	
	flags = uflags;
	
	trim_flags();
}

void User::add_flag(char flag)
{
	flags += flag;
	regen_flags();
}

void User::add_flags(std::string f)
{
	flags += f;
	regen_flags();
}

void User::del_flag(char flag)
{
	string O,M,R,Q,P,U,N,o,h,v,d,k,m;
	O = "O";
	M = "MO";
	R = "RMO";
	Q = "QRMO";
	P = "PQRMO";
	U = "UPQRMO";
	N = "N";
	o = "o";
	h = "ho";
	v = "vho";
	d = "dkm";
	k = "km";
	m = "m";
	
	string s;
	
	switch (flag)
	{
		case 'O':
			s = O;
			break;
		case 'M':
			s = M;
			break;
		case 'R':
			s = R;
			break;
		case 'Q':
			s = Q;
			break;
		case 'P':
			s = P;
			break;
		case 'U':
			s = U;
			break;
		case 'N':
			s = N;
			break;
		case 'o':
			s = o;
			break;
		case 'h':
			s = h;
			break;
		case 'v':
			s = v;
			break;
		case 'd':
			s = d;
			break;
		case 'k':
			s = k;
			break;
		case 'm':
			s = m;
			break;
		default:
			break;
	}
	
	string tmp = "";
	if(flag == 'U')
		tmp += 'N';
		
	for(unsigned int i = 0; i < flags.length(); i++)
	{
		if(s.find(flags.at(i)) == string::npos)
			tmp += flags.at(i);
	}
	
	flags = tmp;
	
	regen_flags();
}

void User::del_flags(std::string f)
{
	for(unsigned int i = 0; i < f.length(); i++)
	{
		del_flag(f.at(i));
	}
}

void User::trim_flags()
{
	string newflags = "";
	for(unsigned int i = 0; i < flags.length(); i++)
	{
		if(newflags.find(flags.at(i)) == string::npos)
			newflags += flags.at(i);
	}
	
	flags = newflags;
}

bool User::match_flag(char flag)
{
	if(flags.find(flag) != string::npos)
		return  true;
	return false;
}

bool User::match_flags(std::string p_flags)
{
	bool res = true;
	
	for(unsigned int i = 0; i < p_flags.length(); i++)
	{
		if(!match_flag(p_flags.at(i)))
			return false;
	}
	
	return res;
}

bool User::operator==(const User &u2) // Two users are equals if they share the same identifying string
{
	 bool res = (id == u2.id);
	 return res;
}

std::string User::get_id()
{
	return id;
}

void User::set_id(std::string p_id)
{
	id = p_id;
}

std::string User::get_pl_nick()
{
	return pl_nick;
}

void User::set_pl_nick(std::string p_pl_nick)
{
	pl_nick = p_pl_nick;
}

std::string User::get_pl_pass()
{
	return pl_pass;
}

void User::set_pl_pass(std::string p_pl_pass)
{
	pl_pass = p_pl_pass;
}

std::string User::get_flags()
{
	return flags;
}

void User::set_flags(std::string p_flags)
{
	flags = p_flags;
}

bool User::get_pl_access()
{
	return pl_access;
}

void User::set_pl_access(bool p_pl_access)
{
	pl_access = p_pl_access;
}

vector<string> User::get_server_list()
{
	vector<string> res;
	for(unsigned int i = 0; i < server_nick.size(); i++)
	{
		if(server_nick.at(i).size() >= 1)
			res.push_back(server_nick.at(i).at(0));
	}
	
	return res;
}
