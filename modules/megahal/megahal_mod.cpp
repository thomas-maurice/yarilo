#include <iostream>
#include <bot_api.h>
#include <ctime>
#include <libplugin.h>
#include "megahal_func.h"
#include "megahal_misc.h"

using namespace std;

/*
	A few informations about the module
*/

#define MODNAME "megahal"
#define AUTHOR  "Jason L. Hutchens, ^Baron^ and Epsilon (portage from eggdrop)"
#define VERSION "0.01dev"

void load_config();
bool bot_can_learn(string label, vector<string> author, string channel, string message);
bool bot_can_reply(string label, vector<string> author, string channel, string message, string mynick);

time_t last_save;
long save_delay = 3600; // Sauvegarde horaire du brain

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
	megahal_start();
	load_config();
	time(&last_save);
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
	
	mi->bind(PRIVMSG);
	mi->bind(TIME);
	mi->bind(COMMAND);
	
	OBJ_TO_VOID(mi, modvoid);
	return modvoid;
}

int on_time(Event *e)
{
	e=e;
	time_t now;
	time(&now);
	if(now - last_save > save_delay)
	{
		log("megahal", "Saving the brain...");
		save_brain();
		time(&last_save);
		log("megahal", "Brain saved...");
	}
	
	return 0;
}

// An exemple function
int on_privmsg(Event *e)
{
	bool can_learn = false;
	bool can_reply = false;
	
	can_learn = bot_can_learn(e->get_serv()->get_label(), e->get_author(), e->get_channel(), e->get_message());
	can_reply = bot_can_reply(e->get_serv()->get_label(), e->get_author(), e->get_channel(), e->get_message(), e->get_serv()->get_nick());
	
	if(can_reply)
		e->get_serv()->privmsg(e->get_channel(), do_reply(const_cast<char *>(e->get_message().c_str())));
	if(can_learn)
		do_learn(const_cast<char *>(e->get_message().c_str()));
	
	return 0;
}

int DllExport process(void *args)
{
	VOID_TO_OBJ(args, Event, e);
	// Exemple binding	
	if(e->get_type() == PRIVMSG)
	{
		on_privmsg(e);
	}
	else if(e->get_type() == TIME)
		on_time(e);
	else if(e->get_type() == COMMAND)
	{
		if(e->get_module() == MODNAME && e->get_user() != NULL && e->get_user()->match_flag('M'))
		{ // Commandes admin
			if(e->get_command() == "save")
			{
				e->get_serv()->privmsg(e->get_channel(), "Je sauvegarde mon cerveau...");
				save_brain();
				e->get_serv()->privmsg(e->get_channel(), "Sauvegarde du cerveau terminée.");
			}
			else if(e->get_command() == "know")
			{
				istringstream in(e->get_parameters());
				string phr;
				getline(in, phr);
				string cp = phr;// Pour que l'output soit pas en caps lock
				
				if(megahal_find_phrase(const_cast<char*>(phr.c_str())))
					e->get_serv()->privmsg(e->get_channel(), "Ah, oui, \"" + cp + "\", je vois de quoi tu parles !");
				else
					e->get_serv()->privmsg(e->get_channel(), "\"" + cp + "\" ? Mmh non, jamais entendu parler.");
			}
			else if(e->get_command() == "forget")
			{
				istringstream in(e->get_parameters());
				string phr;
				getline(in, phr);
				bool forgotten = forget(const_cast<char*>(phr.c_str()));
				
				if(forgotten)
					e->get_serv()->privmsg(e->get_channel(), "\"" + phr + "\" ? D'accord c'est oublié !");
				else
					e->get_serv()->privmsg(e->get_channel(), "Je ne sais pas de quoi il s'agit.");
			}
			else if(e->get_command() == "knowword")
			{
				istringstream in(e->get_parameters());
				string mot;
				in >> mot;
				int c = count_word(const_cast<char*>(mot.c_str()));
				ostringstream num("");
				num << c;
				
				if(c != 0)
					e->get_serv()->privmsg(e->get_channel(), "Ce mot m'a déjà été mentionné " + num.str() + " fois par le passé.");
				else
					e->get_serv()->privmsg(e->get_channel(), "Je n'ai jamais entendu ce mot.");
			}
			else if(e->get_command() == "forgetword")
			{
				istringstream in(e->get_parameters());
				string mot;
				in >> mot;
				int c = count_word(const_cast<char*>(mot.c_str()));
				ostringstream num("");
				num << c;
				
				if(c != 0)
				{
					if(forget_word(const_cast<char*>(mot.c_str())))
					{
						e->get_serv()->privmsg(e->get_channel(), "Ce mot m'a déjà été mentionné " + num.str() + " fois par le passé, mais c'est oublié maintenant.");
					}
					else
					{
						e->get_serv()->privmsg(e->get_channel(), "Ce mot m'a déjà été mentionné " + num.str() + " fois par le passé, mais je ne peux pas l'oublier.");
					}
				}
				else
				{
					e->get_serv()->privmsg(e->get_channel(), "Je n'ai jamais entendu le mot \"" + mot + "\"");
				}
			}
		}
		if(e->get_module() == MODNAME && e->get_user() != NULL && e->get_user()->match_flag('U'))
		{	// Commandes publiques
			if(e->get_command() == "braininfo")
			{
				ostringstream o("");
				o << "Mon cerveau comporte " << cnt_words() << " mots, " << cnt_nodes() << " noeuds d'associations";
				long size = file_size("data/megahal.brn") + file_size("data/megahal.dic") + file_size("data/megahal.phr");
				o << " et fait une taille totale de " << size / 1024 << "," << size % 1024<< "Ko.";
				o << " Je vais chercher mes répliques dans une profondeur de contexte de " << get_context();
				if(get_surprise() == 0)
					o << " et le mode improvisation est inactif.";
				else
					o << " et le mode improvisation est actif.";
				e->get_serv()->privmsg(e->get_channel(), o.str());
			}
			else if(e->get_command() == "info")
			{
				e->get_serv()->privmsg(e->get_channel(), "MegaHAL pour Yarilo, v" + string(VERSION) + " par Jason L. Hutchens (programme original) ^Baron^ (portage sur eggdrop) and Epsilon (portage depuis eggdrop sur Yarilo)");
			}
		}
	}
	
	return 0;
}
