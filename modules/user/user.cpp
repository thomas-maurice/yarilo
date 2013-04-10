#include <iostream>
#include <bot_api.h>
#include <libplugin.h>

using namespace std;

/*
	A few informations about the module
*/

#define MODNAME "user"
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
	if(e->get_module() != MODNAME)
		return 0;
	
	User* u = e->get_user();
	Server *serv = e->get_serv();
	string chan = e->get_channel();
	
	if(chan == serv->get_nick())
		chan = e->get_unick();
	
	if(u == NULL)
		return 0;
	
	if(u->match_flag('M') == true)
	{
		string cmd = e->get_command();
		if(cmd == "list")
		{
			serv->privmsg(chan, "List of users");
			for(unsigned int i = 0; i < e->get_bot()->get_userlist().size(); i++)
			{
				serv->privmsg(chan, " * " + e->get_bot()->get_userlist().at(i).get_id());
			}
		}
		else if(cmd == "add")
		{
			istringstream in(e->get_parameters());
			string id;
			
			in >> id;
			if(e->get_bot()->user_exists(id))
			{
				serv->privmsg(chan, "Error, this user already exists");
			}
			else if(id == "")
			{
				serv->privmsg(chan, "Syntax : !user.add <id>");
			}
			else
			{
				User user;
				user.set_id(id);
				e->get_bot()->user_add(user);
				serv->privmsg(chan, "User added. Don't forget to save the new configuration.");
			}
		}
		else if(cmd == "addhere")
		{
			istringstream in(e->get_parameters());
			string id;
			
			in >> id;
			
			User_info ui = serv->get_user_info(id);
			
			if(e->get_bot()->user_exists(id))
			{
				serv->privmsg(chan, "Error, this user already exists");
			}
			else if(id == "")
			{
				serv->privmsg(chan, "Syntax : !user.addhere <nickname>");
			}
			else if(ui.is_on_server() == false)
			{
				serv->privmsg(chan, "Error, nick " + id + " is not on the server");
			}
			else
			{
				User user;
				user.set_id(id);
				user.add_host(serv->get_label(), ui.get_user_at_host());
				user.add_nick(serv->get_label(), ui.get_nick());
				user.set_flags("U");
				e->get_bot()->user_add(user);
				serv->privmsg(chan, "User added. Don't forget to save the new configuration.");
			}
		}
		else if(cmd == "delhere")
		{
			istringstream in(e->get_parameters());
			string id;
			in >> id;
			
			User_info ui = serv->get_user_info(id);

			if(!e->get_bot()->user_exists(id))
			{
				serv->privmsg(chan, "Error, this user does not exist");
			}
			else if(id == "")
			{
				serv->privmsg(chan, "Syntax : !user.delhere <nickname>");
			}
			else if(ui.is_on_server() == false)
			{
				serv->privmsg(chan, "Error, nick " + id + " is not on the server");
			}
			else
			{
				User user = e->get_bot()->user_get_by_nick_host(serv->get_label(), ui.get_nick(), ui.get_user_at_host());
				e->get_bot()->user_del(user.get_id());
				serv->privmsg(chan, "User deleted. Don't forget to save the new configuration.");
			}
		}
		else if(cmd == "del")
		{
			istringstream in(e->get_parameters());
			string id;
			
			in >> id;
			if(!e->get_bot()->user_exists(id))
			{
				serv->privmsg(chan, "Error, this user does not exist");
			}
			else if(id == "")
			{
				serv->privmsg(chan, "Syntax : !user.del <id>");
			}
			else
			{
				e->get_bot()->user_del(id);
				serv->privmsg(chan, "User deleted. Don't forget to save the new configuration.");
			}
		}
		else if(cmd == "chid")
		{
			string oldone, newone;
			istringstream in(e->get_parameters());
			in >> oldone >> newone;
			if(oldone != "" && newone != "")
			{
				if(e->get_bot()->user_exists(oldone))
				{
					e->get_bot()->user_get_ptr_by_id(oldone)->set_id(newone);
					serv->privmsg(chan, oldone + "'s ID is now " + newone + ". Don't forget to save the new configuration.");
				}
				else
					serv->privmsg(chan, "Error, user " + oldone + " does not exist");
			}
			else
			{
				serv->privmsg(chan, "Syntax : !user.chid <old id> <new id>");
			}
		}
		else if(cmd == "addflags")
		{
			string user, flags;
			istringstream in(e->get_parameters());
			in >> user >> flags;
			if(user != "" && flags != "")
			{
				if(e->get_bot()->user_exists(user))
				{
					if(u->match_flags("M") && !e->get_bot()->user_get_ptr_by_id(user)->match_flags("O"))
					{
						e->get_bot()->user_get_ptr_by_id(user)->add_flags(flags);
						serv->privmsg(chan, user + " flags changed !");
					}
					else
						serv->privmsg(chan, user + " is to high for you !");
				}
				else
					serv->privmsg(chan, "Error, user " + user + " does not exist");
			}
			else
			{
				serv->privmsg(chan, "Syntax : !user.addflags <id> <flags>");
			}
		}
		else if(cmd == "delflags")
		{
			string user, flags;
			istringstream in(e->get_parameters());
			in >> user >> flags;
			if(user != "" && flags != "")
			{
				if(e->get_bot()->user_exists(user))
				{
					if(u->match_flags("M") && !e->get_bot()->user_get_ptr_by_id(user)->match_flags("O"))
					{
						e->get_bot()->user_get_ptr_by_id(user)->del_flags(flags);
						serv->privmsg(chan, user + " flags changed !");
					}
					else
						serv->privmsg(chan, user + " is to high for you !");
				}
				else
					serv->privmsg(chan, "Error, user " + user + " does not exist");
			}
			else
			{
				serv->privmsg(chan, "Syntax : !user.delflags <id> <flags>");
			}
		}
		else if(cmd == "save")
		{
			serv->privmsg(chan, "Saving users configuration...");
			e->get_bot()->update_users();
		}
		else if(cmd == "info")
		{
			istringstream in(e->get_parameters());
			string nick;
			in >> nick;
			
			if(e->get_bot()->user_exists(nick))
			{
				User user = e->get_bot()->user_get_by_id(nick);
				ostringstream o("");
				o << nick << " is present on " << user.get_server_list().size() << " servers with flags " << user.get_flags();
				serv->privmsg(chan, o.str());
			}
			else
			{
				serv->privmsg(chan, "Error,no such user in my configuration.");
			}
			
		}
	}
	
	if(u->match_flag('U') == true)
	{
		string cmd = e->get_command();
		if(cmd == "help")
		{
			serv->privmsg(chan, "Syntax for admins : !user.[add|del|addhere|delhere|chid|save|list|info]");
			//serv->privmsg(chan, "Syntax for admins (again) : !user.admin.[addnick|addhost|delnick|delhost|chpass]");
			serv->privmsg(chan, "Syntax for users : !user.[addnick|delnick|addhost|delhost]");
		}
		else if(cmd == "addnick")
		{
			istringstream in(e->get_parameters());
			string i;
			in >> i;
			if(i != "")
			{
				u->add_nick(serv->get_label(), i);
				serv->privmsg(chan, "Nick " + i + " added for user " + u->get_id());
			}
			else
				serv->privmsg(chan, "Syntax : !user.addnick <nick>");
			
			e->get_bot()->update_user(*u);
		}
		else if(cmd == "addhost")
		{
			istringstream in(e->get_parameters());
			string i;
			in >> i;
			if(i != "")
			{
				u->add_host(serv->get_label(), i);
				serv->privmsg(chan, "Host " + i + " added for user " + u->get_id());
			}
			else
				serv->privmsg(chan, "Syntax : !user.addhost <host>");
			
			e->get_bot()->update_user(*u);
		}
		else if(cmd == "delnick")
		{
			istringstream in(e->get_parameters());
			string i;
			in >> i;
			if(i != "")
			{
				if(u->match(serv->get_label(), i))
				{
					u->del_nick(serv->get_label(), i);
					serv->privmsg(chan, "Nick " + i + " deleted for user " + u->get_id());
				}
				else
					serv->privmsg(chan, "Error, no such nick in user");
				
				e->get_bot()->update_user(*u);
				
			}
			else
				serv->privmsg(chan, "Syntax : !user.delnick <nick>");
		}
		else if(cmd == "delhost")
		{
			istringstream in(e->get_parameters());
			string i;
			in >> i;
			if(i != "")
			{
				u->del_host(serv->get_label(), i);
				serv->privmsg(chan, "Host " + i + " deleted for user " + u->get_id());
				
				e->get_bot()->update_user(*u);
			}
			else
				serv->privmsg(chan, "Syntax : !user.delhost <host>");
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
