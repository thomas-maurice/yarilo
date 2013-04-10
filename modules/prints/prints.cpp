#include <iostream>
#include <bot_api.h>
#include <libplugin.h>

using namespace std;

extern "C"
{
	int DllExport on_load();
	int DllExport on_unload();
	int DllExport process(void *args);
	void* DllExport plugin_info();
}

int DllExport on_load()
{
	cout << "Loading `prints`" << endl;
	return 0;
}

int DllExport on_unload()
{
	cout << "Unloading `prints`" << endl;
	return 0;
}

void* DllExport plugin_info()
{
	Module_info* mi = new Module_info;
	
	mi->set_name("prints");
	mi->set_author("Epsilon");
	mi->set_version("v0.1");
	mi->bind(PRIVMSG);
	
	OBJ_TO_VOID(mi, modvoid);
	return modvoid;
}

int on_privmsg(Event* e)
{	
	if(e->get_user() != NULL)
	{
		cout << "I " << e->get_author().at(0) << "@" << e->get_channel() << " : " << e->get_message() << endl;	
		e->get_serv()->privmsg(e->get_channel(), e->get_message());	
	}
	else
	{
		cout << "U " << e->get_author().at(0) << "@" << e->get_channel() << " : " << e->get_message() << endl;	
	}
	
	return 0;
}

int DllExport process(void *args)
{
	VOID_TO_OBJ(args, Event, e);
	
	if(e->get_type() == PRIVMSG)
	{
		on_privmsg(e);
	}
	
	return 0;
}
