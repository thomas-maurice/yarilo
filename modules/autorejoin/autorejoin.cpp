#include <iostream>
#include <bot_api.h>
#include <libplugin.h>

using namespace std;

/*
	A few informations about the module
*/

#define MODNAME "autorejoin"
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
	
	mi->bind(KICK);
	
	OBJ_TO_VOID(mi, modvoid);
	return modvoid;
}


int on_kick(Event *e)
{
	if(e->get_target() == e->get_serv()->get_nick())
	{
		e->get_serv()->join(e->get_channel());
	}
	
	return 0;
}

int DllExport process(void *args)
{
	VOID_TO_OBJ(args, Event, e);
	// Exemple binding	
	if(e->get_type() == KICK)
	{
		on_kick(e);
	}
	
	return 0;
}
