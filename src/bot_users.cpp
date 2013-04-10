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


#include <bot.h>
#include <user.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <misc.h>
#include <iomanip>
#include <dirent.h>

#ifndef WIN32
    #include <sys/types.h>
#endif

// User managing functions (class : bot)

using namespace std;

void Bot::load_users()
{
	userlist.clear();	
	
	DIR* dir = opendir("./conf/users");
	
	if(dir == NULL)
	{
		cout << "load_users : Error, unable to open conf/users directory, check existence !" << endl;
		exit(0);
	}
	else
	{
		cout << "Loading users" << endl;
		
		dirent *fic = NULL;
		while((fic = readdir(dir)) != NULL)
		{
			if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
			{
				string username = fic->d_name;
				cout << "|- " << username;
				
				ifstream uf(string("conf/users/" + username).c_str());
				
				if(uf)
				{
					string line;
					string id;
					vector<vector<string> > server_host;
					vector<vector<string> > server_nick;
					string flags;
					bool pl_access;
					string pl_nick;
					string pl_pass;
					
					User user;
					
					while(getline(uf, line))
					{
						if(line != "" && line.at(0) != '#')
						{
							istringstream in(line);
							string cmd;
							in >> cmd;
							cmd = uppercase(cmd);
							
							if(cmd == "ID")
							{
								in >> id;
								user.set_id(id);
								//cout << setw(30) << left << " |- User ID" << ": " << id << endl;
							}
							else if(cmd == "SERVER_N")
							{
								string serv = "";
								string nick = "";
								in >> serv >> nick;
								if(serv != "" && nick != "")
								{
									user.add_nick(serv, nick);
								}
								else
								{
									cout << "Error, SERVER_N directive is invalid. Should be SERVER_N <label> <nick>" << endl;
								}
							}
							else if(cmd == "SERVER_H")
							{
								string serv = "";
								string host = "";
								in >> serv >> host;
								if(serv != "" && host != "")
								{
									user.add_host(serv, host);
								}
								else
								{
									cout << "Error, SERVER_H directive is invalid. Should be SERVER_H <label> <host>" << endl;
								}
							}
							else if(cmd == "PL")
							{
								in >> pl_access;
								user.set_pl_access(pl_access);
								//cout << setw(30) << left << " |- Partyline access" << ": " << setw(30) << pl_access << endl;
							}
							else if(cmd == "PL_N")
							{
								in >> pl_nick;
								user.set_pl_nick(pl_nick);
								//cout << setw(30) << left << " |- Partyline nickname" << ": " << pl_nick << endl;
							}
							else if(cmd == "PL_P")
							{
								in >> pl_pass;
								user.set_pl_pass(pl_pass);
							}
							else if(cmd == "FLAGS")
							{
								in >> flags;
								string uflags = "";
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
								
								user.set_flags(uflags);
								//cout << setw(30) << left << " |- Flags" << ": " << uflags << endl;
							}
						}
					}
					
					cout << " - Loaded" << endl;
					
					//user.list_nicks();
					//user.list_hosts();
					
					userlist.push_back(user);
					
					uf.close();
				}
				else
				{
					cout << " - Not loaded" << endl;
					cout << "load_users : Error, unable to open conf/users/" << username << ", check permissions !" << endl;
				}
			}
		}
		
		cout << "`- Done" << endl << endl;
		
	}
}

void Bot::reload_users()
{
	userlist.clear();
	load_users();
}

std::vector<User> Bot::get_userlist()
{
	return userlist;
}

bool Bot::user_exists(string id)
{
	for(unsigned int i = 0; i < userlist.size(); i++)
	{
		if(userlist.at(i).get_id() == id)
			return true;
	}
	
	return false;
}

User Bot::user_get_by_id(string id)
{
	for(unsigned int i = 0; i < userlist.size(); i++)
	{
		if(userlist.at(i).get_id() == id)
			return userlist.at(i);
	}
	
	return User();
}

void Bot::user_add(User u)
{
	if(user_exists(u.get_id()) == false)
		userlist.push_back(u);
}

void Bot::user_del(string id)
{
	vector<User> tmp;
	for(unsigned int i = 0; i < userlist.size(); i++)
	{
		if(userlist.at(i).get_id() != id)
			tmp.push_back(userlist.at(i));
	}
	
	userlist = tmp;
}

bool Bot::user_match(std::string serv, std::string nick)
{
	for(unsigned int i = 0; i < userlist.size(); i++)
	{
		if(userlist.at(i).match(serv, nick))
			return true;
	}
	
	return false;
}

bool Bot::user_match(std::string serv, std::string nick, std::string host)
{
	for(unsigned int i = 0; i < userlist.size(); i++)
	{
		if(userlist.at(i).match(serv, nick, host))
			return true;
	}
	
	return false;
}

User Bot::user_get_by_nick(std::string serv, std::string nick)
{
	for(unsigned int i = 0; i < userlist.size(); i++)
	{
		if(userlist.at(i).match(serv, nick))
			return userlist.at(i);
	}
	
	return User();
}

User Bot::user_get_by_nick_host(std::string serv, std::string nick, std::string host)
{
	for(unsigned int i = 0; i < userlist.size(); i++)
	{
		if(userlist.at(i).match(serv, nick, host))
			return userlist.at(i);
	}
	
	return User();
}

User* Bot::user_get_ptr_by_nick(std::string serv, std::string nick)
{
	for(unsigned int i = 0; i < userlist.size(); i++)
	{
		if(userlist.at(i).match(serv, nick))
			return &userlist.at(i);
	}
	
	return NULL;
}

User* Bot::user_get_ptr_by_nick_host(std::string serv, std::string nick, std::string host)
{
	for(unsigned int i = 0; i < userlist.size(); i++)
	{
		if(userlist.at(i).match(serv, nick, host))
			return &userlist.at(i);
	}
	
	return NULL;
}

User* Bot::user_get_ptr_by_id(std::string id)
{
	for(unsigned int i = 0; i < userlist.size(); i++)
	{
		if(userlist.at(i).get_id() == id)
			return &userlist.at(i);
	}
	
	return NULL;
}

void Bot::update_user(User user)
{
	ofstream fic(string("./conf/users/" + user.get_id()).c_str(), ios::out | ios::trunc);
	
	if(fic)
	{
		fic << user.to_string();
		fic.close();
	}
	else
	{
		cout << "update_users : Error, unable to open conf/users/" << user.get_id() << ", check permissions !" << endl;
	}
}

void Bot::update_users()
{
	for(unsigned int i = 0; i < userlist.size(); i++)
	{
		update_user(userlist.at(i));
	}
	
	trim_users();
}

void Bot::trim_users()
{
	DIR* dir = opendir(string("./conf/users/").c_str());
	
	if(dir == NULL)
	{
		cout << "trim_users : Error, unable to open conf/users directory, check existence !" << endl;
	}
	else
	{
		dirent *fic = NULL;
		while((fic = readdir(dir)) != NULL)
		{
			if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
			{
				string user = fic->d_name;
				string f = "./conf/users/" + user;
    			if(!user_exists(user))
					remove(f.c_str());
			}
    	}
    }
}
