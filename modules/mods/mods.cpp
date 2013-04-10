#include <iostream>
#include <sstream>
#include <bot_api.h>
#include <libplugin.h>

#include <dirent.h>
#include <misc.h>

#ifndef WIN32
    #include <sys/types.h>
#endif

using namespace std;

/*
	A few informations about the module
*/

#define MODNAME "mods"
#define AUTHOR  "Epsilon"
#define VERSION "0.01dev"

extern "C"
{
	int DllExport on_load();
	int DllExport on_unload();
	int DllExport process(void *args);
	void* DllExport plugin_info();
}

int DllExport on_load()
{
	cout << "Loading `" << MODNAME << "` (v" << VERSION << ") by " << AUTHOR << endl;
	return 0;
}

int DllExport on_unload()
{
	cout << "Unloading `" << MODNAME << "`" << endl;
	return 0;
}

void* DllExport plugin_info()
{
	Module_info* mi = new Module_info;
	
	mi->set_name(MODNAME);
	mi->set_author(AUTHOR);
	mi->set_version(VERSION);
	
	
	// Bind part !
	
	mi->bind(COMMAND);
	
	OBJ_TO_VOID(mi, modvoid);
	return modvoid;

}

int on_command(Event *e)
{
	if(e->get_module() == MODNAME && e->get_user() != NULL && e->get_user()->match_flag('M'))
	{
		string cmd = e->get_command();
		string param = e->get_parameters();
		istringstream in(param);
		
		string resp = e->get_channel();
			
		if(e->get_channel() == e->get_serv()->get_nick())
			resp = e->get_unick();
	
	
		if(cmd == "load")
		{
			string mod;
			in >> mod;
			if(mod != "")
			{
				if(e->get_bot()->load_module(mod))
					e->get_serv()->privmsg(resp, "Module " + mod + " successfully loaded");
				else
					e->get_serv()->privmsg(resp, "Module " + mod + " failed to load");
			}
			else
				e->get_serv()->privmsg(resp, "Syntax : !mod.load <modname>");
		}
		else if(cmd == "unload")
		{
			string mod;
			in >> mod;
			if(mod != "")
			{
				if(mod != MODNAME)
				{
					e->get_bot()->unload_module(mod);
				}
				else
					e->get_bot()->unload_module_later(mod);
				
				e->get_serv()->privmsg(resp, "Unloading " + mod);
				return 0;
			}
			else
			{
				e->get_serv()->privmsg(resp, "Syntax : !mod.unload <modname>");
			}
		}
		else if(cmd == "reload")
		{
			string mod;
			in >> mod;
			if(mod != "")
			{
				if(mod != MODNAME)
				{
					if(e->get_bot()->reload_module(mod))
						e->get_serv()->privmsg(resp, "Module " + mod + " successfully reloaded");
					else
						e->get_serv()->privmsg(resp, "Module " + mod + " failed to reload");
				}
				else
				{
					e->get_bot()->reload_module_later(mod);
					e->get_serv()->privmsg(resp, "Module " + mod + " will be reloaded");
				}
			}
			else
			{
				e->get_serv()->privmsg(resp, "Syntax : !mod.reload <modname>");
			}
		}
		else if(cmd == "list")
		{
			string arg;
			in >> arg;

			if(arg == "loaded")
			{
				for(unsigned int i = 0; i < e->get_bot()->get_modlist().size(); i++)
					e->get_serv()->privmsg(resp, "[*] " + e->get_bot()->get_modlist().at(i).get_name());
			}
			else if(arg == "available")
			{
				#if defined (WIN32)
					string ext = ".dll";
				#else
					string ext = ".so";
				#endif
				DIR* dir = opendir("./modules/");
				dirent *fic = NULL;
				while((fic = readdir(dir)) != NULL)
				{
					if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
					{
						string modname = fic->d_name;
						string name = modname.substr(0, modname.length() - ext.length());
						if(modname.substr(modname.length() - ext.length(), modname.length() - 1) == ext)
						{
							if(!e->get_bot()->mod_is_loaded(name))
								e->get_serv()->privmsg(resp, "[X] " + name);
						}
					}
				}
			}
			else if(arg == "all" || arg == "")
			{
				#if defined (WIN32)
					string ext = ".dll";
				#else
					string ext = ".so";
				#endif
				DIR* dir = opendir("./modules/");
				dirent *fic = NULL;
				while((fic = readdir(dir)) != NULL)
				{
					if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
					{
						string modname = fic->d_name;
						string name = modname.substr(0, modname.length() - ext.length());
						if(modname.substr(modname.length() - ext.length(), modname.length() - 1) == ext)
						{
							if(e->get_bot()->mod_is_loaded(name))
								e->get_serv()->privmsg(resp, "[*] " + name);
							else
								e->get_serv()->privmsg(resp, "[X] " + name);
						}
					}
				}
			}
			else
			{
				e->get_serv()->privmsg(resp, "Syntax : !mod.list <available|all|loaded>");
			}
		}
		else if(cmd == "help")
		{
			e->get_serv()->privmsg(resp, "Syntax : !mod.[re|un]load <module> | !mod.list [all|available|loaded]");
		}
	}
	return 0;
}

int DllExport process(void *args)
{
	VOID_TO_OBJ(args, Event, e);
	// Exemple binding	
	if(e->get_type() == COMMAND)
	{
		on_command(e);
	}
	
	return 0;
}
