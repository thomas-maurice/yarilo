#include <iostream>
#include <bot_api.h>
#include <libplugin.h>

using namespace std;

/*
	A few informations about the module
*/

#define MODNAME "admin"
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

// Privmsg binding
int on_command(Event* e)
{	
	if(e->get_module() != MODNAME)
		return 0;
	
	string cmd = lowercase(e->get_command());
	
	if(e->get_user() != NULL && e->get_user()->match_flag('O'))
	{
		if(cmd == "die")
			e->get_bot()->stop();
		else if(cmd == "join")
		{
			istringstream in(e->get_parameters());
			string chan;
			in >> chan;
			e->get_serv()->join(chan);
		}
		else if(cmd == "part")
		{
			istringstream in(e->get_parameters());
			string chan;
			in >> chan;
			e->get_serv()->part(chan);
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
