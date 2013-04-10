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
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <iomanip>
#include <dirent.h>
#include <misc.h>

#ifndef WIN32
    #include <sys/types.h>
#endif

// Modules managing functions (class : bot)

using namespace std;

void Bot::load_modules()
{
	#if defined (WIN32)
		string ext = ".dll";
	#else
		string ext = ".so";
	#endif
	
	cout << "Loading modules..." << endl;
	
	ifstream f("./conf/bot.conf", ios::in);
	
	bool loadall = false;
	vector<string> list;
	
	if(f)
	{
		string line;
		while(getline(f, line))
		{
			if(line != "" && line.at(0) != '#')
			{
				string cmd;
				istringstream in(line);
				in >> cmd;
				
				if(lowercase(cmd) == "loadmodule")
				{
					string name;
					in >>name;
					
					if(name == "all")
					{
						loadall = true;
						cout << "Loading all the modules" << endl;
						break;
					}
					else
					{
						list.push_back(name);
					}
				}
			}
		}
		
		f.close();
	}
	else
	{
		cout << "Error, unable to open conf/bot.conf check permissions !" << endl;	
		return;
	}
	
	DIR* dir = opendir("./modules/");
	if(dir == NULL)
	{
		cout << "Error, unable to open modules/ check permissions !" << endl;
	}
	
	dirent *fic = NULL;
	while((fic = readdir(dir)) != NULL)
	{
		if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
		{
			string modname = fic->d_name;
			if(modname.substr(modname.length() - ext.length(), modname.length() - 1) == ext)
			{
				string name = modname.substr(0, modname.length() - ext.length());
				if(loadall == true)
				{
					load_module(name);
				}
				else
				{
					for(unsigned int i = 0; i < list.size(); i++)
					{
						if(name == list.at(i))
						{	
							load_module(name);
							break;
						}
					}
				}
			}
		}
	}
}

bool Bot::load_module(std::string module)
{
	int found = -1;
	for(unsigned int i = 0; i < modlist.size(); i++)
	{
		if(modlist.at(i).get_name() == module)
		{
			found = i;
			break;
		}
	}
	
	if(found == -1)
	{
		modlist.push_back(Module(module));
		found = modlist.size() - 1;
	}
	
	return modlist.at(found).is_loaded();
}

void Bot::unload_module(std::string module)
{
	for(unsigned int i = 0; i < modlist.size(); i++)
	{
		if(modlist.at(i).get_name() == module)
			modlist.at(i).unload();
	}
	
	trim_modules();
}

bool Bot::reload_module(std::string module)
{
	unload_module(module);
	return load_module(module);
}

void Bot::unload_module_later(std::string module)
{
	bool found = false;
	for(unsigned int i = 0; i < modules_to_unload.size(); i++)
	{
		if(modules_to_unload.at(i) == module)
		{
			found = true;
			break;
		}
	}
	
	if(!found)
		modules_to_unload.push_back(module);
}

void Bot::reload_module_later(std::string module)
{
	bool found = false;
	for(unsigned int i = 0; i < modules_to_reload.size(); i++)
	{
		if(modules_to_reload.at(i) == module)
		{
			found = true;
			break;
		}
	}
	
	if(!found)
		modules_to_reload.push_back(module);
}

void Bot::trim_modules()
{
	vector<Module> tmp;
	for(unsigned int i = 0; i < modlist.size(); i++)
	{
		if(modlist.at(i).is_loaded() == true)
			tmp.push_back(modlist.at(i));
	}
	
	modlist = tmp;
}

bool Bot::mod_is_loaded(std::string module)
{
	for(unsigned int i = 0; i < modlist.size(); i++)
	{
		if(modlist.at(i).get_name() == module && modlist.at(i).is_loaded())
			return true;
	}
	
	return false;
}
