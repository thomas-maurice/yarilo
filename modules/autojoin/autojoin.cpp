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
	cout << "Loading `autojoin` (v0.01dev) by Epsilon" << endl;
	return 0;
}

int DllExport on_unload()
{
	cout << "Unloading `autojoin`" << endl;
	return 0;
}

void* DllExport plugin_info()
{
	Module_info* mi = new Module_info;
	
	mi->set_name("autojoin");
	mi->set_author("Thomas MAURICE");
	mi->set_version("v1");
	mi->bind(PRIVMSG);
	
	OBJ_TO_VOID(mi, modvoid);
	return modvoid;
}

int on_connect(Event *e)
{
	cout << "Connected to " << e->get_serv()->get_label() << endl;
	
	ifstream f("./data/autojoin.lst", ios::in);
	
	if(f)
	{
		string line;
		while(getline(f, line))
		{
			if(line != "" && line.at(0) != '#')
			{
				istringstream in(line);
				string label, chan;
				
				in >> label >> chan;
				
				if(label == e->get_serv()->get_label() && chan != "")
				{
					cout << " * " << e->get_serv()->get_label() << " -> " << chan << endl;
					e->get_serv()->join(chan);
				}
			}		
		}
		
		f.close();
		
	}
	
	return 0;
}

int DllExport process(void *args)
{
	VOID_TO_OBJ(args, Event, e);
	
	if(e->get_type() == CONNECT)
	{
		on_connect(e);
	}
	
	return 0;
}
