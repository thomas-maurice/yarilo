#include <iostream>
#include <bot_api.h>
#include <libplugin.h>

using namespace std;

/*
	A few informations about the module
*/

#define MODNAME "automodes"
#define AUTHOR  "Thomas Maurice"
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
	
	mi->bind(JOIN);
	mi->bind(MODE);
	
	OBJ_TO_VOID(mi, modvoid);
	return modvoid;
}

// An exemple function
int on_join(Event *e)
{
	if(e->get_user() != NULL)
	{
		User* u = e->get_user();
		Server* s = e->get_serv();
		string nick = e->get_unick();
		string chan = e->get_channel();
		
		if(u->match_flag('o'))
		{
			s->mode(chan, nick, "+o");
		}
		else if(u->match_flag('h'))
		{
			s->mode(chan, nick, "+h");
		}
		else if(u->match_flag('v'))
		{
			s->mode(chan, nick, "+v");
		}
	}
	
	return 0;
}

int on_mode(Event* e)
{
	Server *s = e->get_serv();
	vector<string> nicks = e->get_nicks();
	vector<string> modes = e->get_modes();
	
	for(unsigned int i = 0; i < nicks.size(); i++)
	{
		if(e->get_bot()->user_match(s->get_label(), nicks.at(i)))
		{
			User u = e->get_bot()->user_get_by_nick(s->get_label(), nicks.at(i));
			string mode = modes.at(i);

			if(mode.length() == 2)
			{
				string tmp = "";
				tmp += mode.at(1);
				mode = tmp;
			}
				
			if(u.match_flag('d'))
			{
				s->mode(e->get_channel(), nicks.at(i), "-" + mode);
			}
		}
	}
	
	return 0;
}

int DllExport process(void *args)
{
	VOID_TO_OBJ(args, Event, e);
	// Exemple binding	
	if(e->get_type() == JOIN)
	{
		on_join(e);
	}
	else if(e->get_type() == MODE)
	{
		on_mode(e);
	}
	
	return 0;
}
