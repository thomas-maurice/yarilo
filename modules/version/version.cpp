#include <iostream>
#include <bot_api.h>
#include <libplugin.h>

using namespace std;

/*
	A few informations about the module
*/

#define MODNAME "version"
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
	
	mi->bind(CTCP);
	
	OBJ_TO_VOID(mi, modvoid);
	return modvoid;
}


int on_ctcp(Event *e)
{
	if(uppercase(e->get_command()) == "VERSION")
	{
		ifstream f("data/version.txt", ios::in);
		if(f)
		{
			string v;
			getline(f, v);
			e->get_serv()->notice(e->get_unick(), v);
			f.close();
		}
		else
		{
			e->get_serv()->notice(e->get_unick(), "I'm a Yarilo IRC robot, v0.01 !");
		}
	}
	
	return 0;
}

int DllExport process(void *args)
{
	VOID_TO_OBJ(args, Event, e);
	// Exemple binding	
	if(e->get_type() == CTCP)
	{
		on_ctcp(e);
	}
	
	return 0;
}
