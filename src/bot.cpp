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
#include <misc.h>
#include <color_console.h>
#include <dirent.h>

#ifndef WIN32
    #include <sys/types.h>
#endif

using namespace std;

Bot::Bot()
{

}

Bot::~Bot()
{

}

void Bot::load_config()
{
	ifstream cfg("conf/bot.conf", ios::in);

	if(cfg)
	{
		string line;
		int n = 1;
		while(getline(cfg, line))
		{
			if(line == "" || line.at(0) == '#')
			{
				// Comment line, ignored
			}
			else
			{
				string tmp = line;
				istringstream in(tmp);
				
				string cmd;
				
				in >> cmd;
				
				if(cmd == "server")
				{
				 // server irc.epiknet.org 6667 foo bar pass 0
					string label, host, nick, ident, pass;
					int port= -1;
					bool ssl = false;
					
					in >> label >> host >> port >> nick >> ident >> pass >> ssl;
					
					if(label != "" && host != "" && port != -1 && nick != "")
					{
						if(ident == "")
							ident = "yarilo";
						
						Server serv(label, host, port, nick, ident, pass, ssl);
						servlist.push_back(serv);
						cout << "Adding server " << label << endl;
					}
					
				}
				else if(cmd == "loadmodule")
				{
				
				}
				else
				{
					cout << "Warning : " << cmd << " option not recognized in conf/bot.conf line " << n << endl;
				}
			}
			
			n++;
		}
		
		cfg.close();
		
		cout << endl;
		
	}
	else
	{
		cout << "Error, conf file could'nt be loaded" << endl;
		log("core", "Unable to open config file at conf/bot.conf, check permissions !");
	}
}

void Bot::start()
{
	log("core", "Starting up...");
	load_config();
	load_users();
	load_modules();
	
	for(unsigned int i = 0; i < servlist.size(); i++)
	{
		servlist.at(i).connect();
		if(servlist.at(i).is_connected())
		{
			servlist.at(i).identify();
		}
		else
		{
			servlist.erase(servlist.begin()+i);
		}
	}
	
	alive = true;
	
	log("core", "Started.");
	
	if(servlist.size() == 0)
	{
		color_console(RED, BLACK);
		cout << "[!] No servers availables, shutting down" << endl;
		reinit_console();
		log("core", "No servers availables, shutting down");
		
		sleep_(1);
		
		stop();
	}
	
	main_loop();
}

void Bot::stop()
{
	alive = false;
	log("core", "Stopping...");
	
	for(unsigned int i = 0; i < servlist.size(); i++)
	{
		servlist.at(i).quit();
		servlist.at(i).disconnect();
	}
	for(unsigned int i = 0; i < modlist.size(); i++)
	{
		modlist.at(i).unload();
	}
	
	modlist.clear();
	servlist.clear();
	userlist.clear();
	
	cout << "Stopped" << endl;
	
	exit(0);
}

std::vector<Module> Bot::get_modlist()
{
	return modlist;
}

std::vector<Server> Bot::get_servlist()
{
	return servlist;
}

void Bot::do_postponed_actions()
{
	// Reload modules
	for(unsigned int i = 0; i < modules_to_reload.size(); i++)
	{
		reload_module(modules_to_reload.at(i));
	}
	
	modules_to_reload.clear();
	
	// Unload modules
	for(unsigned int i = 0; i < modules_to_unload.size(); i++)
	{
		unload_module(modules_to_unload.at(i));
	}
	
	modules_to_unload.clear();
}

void Bot::main_loop()
{
	time_t last_ts;
	time(&last_ts);
	
	while(alive)
	{
		for(unsigned int i = 0; i < servlist.size(); i++)
		{
			parse(servlist.at(i));
			
			do_postponed_actions();
			
			sleep_(0.75);
			
			// The time bind
			time_t ts;
			time(&ts);
			if(ts - last_ts >= 1)
			{
				time(&last_ts);
				Event *e = new Event;
				e->set_type(TIME);
				e->set_bot(this);
				for(unsigned int m = 0; m < modlist.size(); m++)
				{
					modlist.at(m).do_process(e);
				}
			}
		}
	}
}
