#include <iostream>
#include <bot_api.h>
#include <libplugin.h>

#include <dirent.h>

using namespace std;

/*
	A few informations about the module
*/

#define MODNAME "logs"
#define AUTHOR  "Thomas Maurice"
#define VERSION "0.01dev"

extern "C"
{
	int DllExport on_load();
	int DllExport on_unload();
	int DllExport process(void *args);
	void* DllExport plugin_info();
}

void do_log(std::string serv, std::string chan, std::string ligne);

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
	
	mi->bind(PRIVMSG);
	
	OBJ_TO_VOID(mi, modvoid);
	return modvoid;
}

int DllExport process(void *args)
{
	VOID_TO_OBJ(args, Event, e);
	
	int t = e->get_type();
	
	if(	t == PRIVMSG ||
		t == ACTION  ||
		t == JOIN ||
		t == PART ||
		t == QUIT ||
		t == KICK ||
		t == NICK)
	{
	
		string s = e->get_serv()->get_label();
		string c = e->get_channel();
		string u = e->get_unick();
		
		if(c == e->get_serv()->get_nick()) // Do not log PMs
			return 0;
	
		if(e->get_type() == PRIVMSG)
		{
			do_log(s, c, u + " : " + e->get_message());
		}
		else if(e->get_type() == ACTION)
		{
			do_log(s, c, " * " + u + " " +  e->get_message());
		}
		else if(e->get_type() == JOIN)
		{
			do_log(s, c, "---> " + u + " a rejoint le canal");
		}
		else if(e->get_type() == PART)
		{
			do_log(s, c, "<--- " + u + " a quitté le canal (" + e->get_message() + ")");
		}
		else if(e->get_type() == KICK)
		{
			do_log(s, c, "<<<- " + u + " a kické " + e->get_target() + " du canal (" + e->get_message() + ")");
		}
		else if(e->get_type() == NICK)
		{
			DIR* dir = opendir(string("data/logs/" + s).c_str());
	
			if(dir == NULL)
			{
				cout << "Error, unable to open the data/logs/ directory, check existence !" << endl;
			}
			else
			{
				dirent *fic = NULL;
				while((fic = readdir(dir)) != NULL)
				{
					if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
					{
						string channame = fic->d_name;
						do_log(s, channame, u + " s'appelle maintenant " + e->get_newnick());
					}
				}
			}
			
			closedir(dir);
		}
		else if(e->get_type() == QUIT)
		{
			DIR* dir = opendir(string("data/logs/" + s).c_str());
	
			if(dir == NULL)
			{
				cout << "Error, unable to open the data/logs/ directory, check existence !" << endl;
			}
			else
			{
				dirent *fic = NULL;
				while((fic = readdir(dir)) != NULL)
				{
					if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
					{
						string channame = fic->d_name;
						do_log(s, channame, u + " a quitté le serveur (" + e->get_message() + ")");
					}
				}
			}
			
			closedir(dir);
		}
	}
	return 0;
}

void do_log(std::string serv, std::string chan, std::string ligne)
{

    const char * NumMois[] = {"janvier", "fevrier", "mars", "avril", "mai", "juin", "juillet", "aout", "septembre", "octobre", "novembre", "decembre"};

    time_t timestamp;
    struct tm * t;
       
    timestamp = time(NULL);
    t = localtime(&timestamp);
    char date[100];
    char heure[100];
    sprintf(date, "%02u_%s_%04u", t->tm_mday, NumMois[t->tm_mon], 1900 + t->tm_year);
    sprintf(heure, "%02u:%02u:%02u", t->tm_hour, t->tm_min, t->tm_sec);
    string dt = "";
    dt += date;
    dt += " ";
    dt += heure;
    
    DIR* logs = NULL;
    logs = opendir("data/logs");
    if(logs == NULL)
    	mkdir_("data/logs");
    
    closedir(logs);
    
    DIR* logs_serv = NULL;
    logs_serv = opendir(string("data/logs/" + serv).c_str());
    if(logs_serv == NULL)
    	mkdir_(string("data/logs/" + serv).c_str());
    
    closedir(logs_serv);
    
    DIR* logs_serv_chan = NULL;
    logs_serv_chan = opendir(string("data/logs/" + serv + "/" + chan).c_str());
    if(logs_serv_chan == NULL)
    	mkdir_(string("data/logs/" + serv + "/" + chan).c_str());
	
	closedir(logs_serv_chan);
	
    ofstream fichier(string("data/logs/" + serv + "/" + chan + "/" + date).c_str(), ios::out | ios::app);
 	
    if(fichier)
    {
            fichier << heure << " " << ligne << endl;

            fichier.close();
    }
    else
    {

    }
}
