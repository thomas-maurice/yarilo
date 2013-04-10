#include <iostream>
#include <bot_api.h>
#include <libplugin.h>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <ctime>

using namespace std;

/*
	A few informations about the module
*/

#define MODNAME "memo"
#define AUTHOR  "Epsilon"
#define VERSION "0.01dev"

#define MBOX_LIMIT 15

#define MESSAGE  0
#define READ_ACC 1
#define SYSTEM   2

#define OK 				  0
#define ERR_MEMOBOX_FULL -1
#define ERR_NO_SUCH_USER -2
#define ERR_IO           -3

bool is_memobox_ok(string user);
void make_memobox(string user);
int send_message(string emitter, string dest, string message, int type);
bool is_memobox_full(string user);
int count_new(string user);
void clean_old(string user);
vector<string> get_message(string user);
void purge_all();
void send_global_message(string emitter, string message, int type);

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
	
	// Checking the tree of memoboxes
	DIR* memo = opendir("./data/memo");
	if(memo == NULL)
		mkdir_("./data/memo");
	
	DIR* dir = opendir("./conf/users");
	
	if(dir == NULL)
	{
		cout << "Error, unable to open conf/users directory, check existence !" << endl;
	}
	else
	{
		dirent *fic = NULL;
		while((fic = readdir(dir)) != NULL)
		{
			if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
			{
				string username = fic->d_name;
				if(!is_memobox_ok(username))
					make_memobox(username);
			}
		}
	}
	
	closedir(dir);
	
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

int on_join(Event* e)
{
	if(e->get_user() == NULL)
		return 0;
	
	int c = count_new(e->get_user()->get_id());
	
	if(c != 0)
	{
		regex_match("test", "test");
		ostringstream line("");
		line << "You have [" << irc_color(02, 01) << "" << c << "" << irc_reinit() << "] new memos in your memobox. Enter";
		line << irc_color(IRC_GREEN) << " !memo.read " << irc_reinit() << "to read them.";
		e->get_serv()->notice(e->get_unick(), line.str());
	}
	
	return 0;
}

int on_command(Event *e)
{
	if(e->get_module() != MODNAME)
	{
		return 0;
	}
	
	if(e->get_user() == NULL || e->get_user()->match_flag('U') == false)
	{
		return 0;
	}
	string cmd = e->get_command();
	Server* serv = e->get_serv();
	User* u = e->get_user();
	
	string chan = e->get_channel();
	if(chan == serv->get_nick())
		chan = e->get_unick();
	
	if(cmd == "send")
	{
		istringstream in(e->get_parameters());
		string target, message;
		in >> target;
		
		getline(in, message);
		if(message.length() > 1)
			message = message.substr(1);
		
		if(e->get_bot()->user_exists(target))
		{	
			int s = send_message(u->get_id(), target, message, MESSAGE);
			if(s == ERR_IO)
				serv->privmsg(chan, "I/O error, contact administrator");
			else if(s == ERR_MEMOBOX_FULL)
				serv->privmsg(chan, "User's memobox is full ! Cannot send more.");
			else
				serv->privmsg(chan, "Message sent, you will get a reading notifier once read !");
		}
		else
		{
			serv->privmsg(chan, "Error, user " + target + " does not exist");
		}
		
	}
	else if(cmd == "read")
	{
		istringstream in(e->get_parameters());
		string numstr;
		
		int num = atoi(numstr.c_str());
		if(num == 0)
			num = 1;
		
		if(count_new(u->get_id()) == 0)
		{
			serv->notice(e->get_unick(), "You have no memo.");
		}
		
		int n = 0;
		
		while(n < num && count_new(u->get_id()) != 0)
		{
			vector<string> message = get_message(u->get_id());
			if(message.size() != 4)
			{
				serv->notice(e->get_unick(), "Error, malformed message");
			}
			else
			{
				ostringstream o("");
				if(message.at(1) == "MESSAGE")
					o << irc_style(IRC_BOLD) << irc_color(IRC_BLUE) << "[Message]" << irc_reinit() << " ";
				else if(message.at(1) == "READ_ACC")
					o << irc_style(IRC_BOLD) << irc_color(IRC_YELLOW) << "[Reading confirmation]" << irc_reinit() << " ";
				else if(message.at(1) == "SYSTEM")
					o << irc_style(IRC_BOLD) << irc_color(IRC_RED) << "[System]" << irc_reinit() << " ";
				
				o << "From " << irc_style(IRC_UNDERLINE) << message.at(0) << irc_reinit() << " on " << message.at(2) << " : " << message.at(3);
				
				serv->notice(e->get_unick(), o.str());
			}
			
			n++;
		}
	}
	else if(cmd == "purge")
	{
		if(e->get_parameters() == "all" && u->match_flag('M'))
		{
			serv->notice(e->get_unick(), "Cleaning all old messages...");
			purge_all();
			send_global_message(u->get_id(), "All your old messages have been purged by the admins.", SYSTEM);
		}
		else
		{
			serv->notice(e->get_unick(), "Cleaning your old messages...");
			clean_old(u->get_id());
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
	else if(e->get_type() == JOIN)
	{
		on_join(e);
	}
	
	return 0;
}

void purge_all()
{
	DIR* dir = opendir("./conf/users");
	
	if(dir == NULL)
	{
		cout << "Error, unable to open conf/users directory, check existence !" << endl;
	}
	else
	{
		dirent *fic = NULL;
		while((fic = readdir(dir)) != NULL)
		{
			if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
			{
				string user = fic->d_name;
				clean_old(user);
			}
		}
	}
}

void send_global_message(string emitter, string message, int type)
{
	DIR* dir = opendir("./data/memo/");
	
	if(dir == NULL)
	{
		cout << "Error, unable to open data/memo directory, check existence !" << endl;
	}
	else
	{
		dirent *fic = NULL;
		while((fic = readdir(dir)) != NULL)
		{
			if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
			{
				string user = fic->d_name;
				send_message(emitter, user, message, type);
			}
		}
	}
}

int send_message(string emitter, string dest, string message, int type)
{
	if(!is_memobox_ok(dest))
		make_memobox(dest);
				
	if(type == MESSAGE && is_memobox_full(dest))
		return ERR_MEMOBOX_FULL;
		
	ostringstream fname("");
	time_t timestamp;
	string seed = random_string(5, true, true);
	time(&timestamp);
			
	fname << timestamp << "." << seed;
			
	ofstream f(string("./data/memo/" + dest + "/new/" + fname.str()).c_str(), ios::out | ios::trunc); 
			
	if(!f)
	{
		return ERR_IO;
	}
	else
	{
		struct tm * t;
		t = localtime(&timestamp);
		char date[100];
		char heure[100];
		const char * NumMois[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
		sprintf(date, "%02u/%s/%04u", t->tm_mday, NumMois[t->tm_mon], 1900 + t->tm_year);
		sprintf(heure, "%02u:%02u:%02u", t->tm_hour, t->tm_min, t->tm_sec);
				
		f << emitter << endl;
		if(type == MESSAGE)
			f << "MESSAGE" << endl;
		else if(type == READ_ACC)
			f << "READ_ACC" << endl;
		else if(type == SYSTEM)
			f << "SYSTEM" << endl;
		f << date << ", " << heure << endl;
		f << message << endl;
				
		f.close();
		
		return OK;
	}
	
	return OK;
}

vector<string> get_message(string user)
{
	if(!is_memobox_ok(user))
		make_memobox(user);
	
	vector<string> res;
	// 0 : Emitter
	// 1 : Type
	// 2 : Date
	// 3 : Message
	
    #if defined (WIN32)
	DIR* dir = opendir(string("./data/memo/" + user + "/new/").c_str());
	dirent *fic = NULL;
	while((fic = readdir(dir)) != NULL)
	{
		string fname = string(fic->d_name);
		
		if(fname != "." && fname != "..")
		{
	#else
	struct dirent **namelist;
    int n;
    n = scandir(string("./data/memo/" + user + "/new/").c_str(), &namelist, 0, alphasort);
    
    for(int i = 0; i < n; i++)
    {
		if(string(namelist[i]->d_name) != "." && string(namelist[i]->d_name) != "..")
    	{
			string fname = namelist[i]->d_name;
	#endif
    		
    		ifstream f(string("./data/memo/" + user + "/new/" + fname).c_str(), ios::in);
    		ofstream o(string("./data/memo/" + user + "/old/" + fname).c_str(), ios::out | ios::trunc);
    		
    		if(f)
    		{
    			string line;
    			while(getline(f, line))
    			{
    				o << line << endl;
    				res.push_back(line);
    			}
    			
    			f.close();
    			o.close();
    			
    			remove(string("./data/memo/" + user + "/new/" + fname).c_str());
    			
    			if(res.size() > 3 && (res.at(1) == "READ_ACC" || res.at(1) == "SYSTEM"))
    			{
    				remove(string("./data/memo/" + user + "/old/" + fname).c_str());
    			}
    			
    			break;
    		}
    		else
    		{
    			res.push_back("System");
    			res.push_back("SYSTEM");
    			res.push_back("Right now");
    			res.push_back("An I/O error occured while trying accessing your messages, contact administrator.");
    		}

    	}
    }
    
    if(res.at(1) == "MESSAGE")
    {
    	send_message(user, res.at(0), res.at(3), 1);
    }
    
    return res;
}

void clean_old(string user)
{
	if(!is_memobox_ok(user))
		make_memobox(user);
		
	DIR* dir = opendir(string("./data/memo/" + user + "/old/").c_str());
	
	if(dir == NULL)
	{
		cout << "Error, unable to open memo directory, check existence !" << endl;
	}
	else
	{
		dirent *fic = NULL;
		while((fic = readdir(dir)) != NULL)
		{
			if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
			{
				string user = fic->d_name;
				string f = "./data/memo/" + user + "/old/" + string(fic->d_name);
    		
				remove(f.c_str());
			}
    	}
    }
}

bool is_memobox_full(string user)
{
	if(!is_memobox_ok(user))
		make_memobox(user);
		
	DIR* dir = opendir(string("./data/memo/" + user + "/new/").c_str());
	int count = 0;
	dirent *fic = NULL;
	while((fic = readdir(dir)) != NULL)
	{
		if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
		{
			count++;
		}
	}
	
	if(count < MBOX_LIMIT)
		return false;
	
	return true;
}

int count_new(string user)
{
	if(!is_memobox_ok(user))
		make_memobox(user);
		
	DIR* dir = opendir(string("./data/memo/" + user + "/new/").c_str());
	int count = 0;
	dirent *fic = NULL;
	while((fic = readdir(dir)) != NULL)
	{
		if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
		{
			count++;
		}
	}
	
	return count;
}

bool is_memobox_ok(string user)
{
	DIR* mb = opendir(string("./data/memo/" + user).c_str());
	DIR* mbib = opendir(string("./data/memo/" + user + "/new/").c_str());
	DIR* mbob = opendir(string("./data/memo/" + user + "/old/").c_str());
	if(mb == NULL || mbib == NULL || mbob == NULL)
		return false;
	else
		return true;
	
	return true;
}

void make_memobox(string user)
{
	mkdir_(string("./data/memo/").c_str());
	mkdir_(string("./data/memo/" + user).c_str());
	mkdir_(string("./data/memo/" + user + "/new/").c_str());
	mkdir_(string("./data/memo/" + user + "/old/").c_str());
}
