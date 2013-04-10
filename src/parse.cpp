/*
 *
 *	Copyright (C) 2012 Thomas MAURICE
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License along
 *	with this program; if not, write to the Free Software Foundation, Inc.,
 *	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include <bot.h>
#include <event.h>
#include <misc.h>

using namespace std;
 
void Bot::parse(Server &serv)
{
	// Recos
	if(!serv.is_connected() || serv.has_timed_out(300))
	{
		time_t now;
		time(&now);
		if(!serv.is_connected() && ((now - serv.get_last_reconnection_attempt()) > 300))
		{
			serv.try_reconnect();
		}
	}
	
	while(serv.can_getline() && serv.is_connected())
	{
		string ligne = "";
		ligne = serv.getline();
		
		//cout << ligne << endl;
		string raw = ligne;
		
		if(regex_match(ligne, "^ERROR.*") == true)
		{
            cout << "Disconnected from the server, check the log." << endl;
            cout << "Reconnexion attempt in 120 seconds if i wasn't killed." << endl;
            log("core", ligne);
            return;
        }
        
        
        istringstream in(ligne);
        string auteur;
        in >> auteur;
		
		
        //----------------------------------------------------------------------------
        if(auteur == "PING")
		{
			string resp;
			in >> resp;
			serv.write_line("PONG " + resp.substr(1));
			return;
		}
        else
        {
            // Pour virer les ":"
            auteur = auteur.substr(1);
            // Récupération du nick
            string nick = auteur.substr(0,auteur.find('!'));
            // Récupération du host
            string host = auteur.substr(auteur.find('!')+1);
            
            // Tableau qui contiendra l'user at host
            vector<string> user;
            user.push_back(nick);
            user.push_back(host);
            
            string pos2;
            in >> pos2;
            //----------------------------------------------------------------------------
            if(atoi(pos2.c_str()) != 0)
            {
                string message = "";
                getline(in, message);
                if(message.length() > 2)
					message = message.substr(1);
				
				//on_numeric(socket, bot, pos2, message);
				if(pos2 == "376")
				{
					//on_connect(socket, bot);
					Event *e = new Event;
					e->set_type(CONNECT);
					e->set_raw(raw);
					e->set_bot(this);
					e->set_serv(&serv);
					e->set_user(NULL);
					
					for(unsigned int m = 0; m < modlist.size(); m++)
					{
						modlist.at(m).do_process(e);
					}
					
				}
				else
				{
					Event *e = new Event;
				    e->set_type(NUMERIC);
					e->set_raw(raw);
					e->set_bot(this);
					e->set_serv(&serv);
					e->set_message(message);
					e->set_code(atoi(pos2.c_str()));
					
					for(unsigned int m = 0; m < modlist.size(); m++)
					{
						modlist.at(m).do_process(e);
					}
				}

            }
            else if(pos2 == "KILL")
            {
            	string me, reason;
            	in >> me >> reason;
            	if(me == serv.get_nick())
            	{
            		getline(in, reason);
            		cout << "I got KILLED ! Reconnecting..." << endl;
            		log("core", "KILLED by " + user.at(0) + " " + reason);
            		serv.try_reconnect();
            	}
            }
            //----------------------------------------------------------------------------
            else if(pos2 == "QUIT") // -- On déclenche tout les modules bindés sur le QUIT
            {
                string message = "";
                getline(in, message);
                if(message.length() > 2)
					message = message.substr(2);
				
				Event *e = new Event;
				e->set_type(QUIT);
				e->set_raw(raw);
				e->set_bot(this);
				e->set_serv(&serv);
				e->set_author(user);
				e->set_message(message);
				e->set_user(user_get_ptr_by_nick_host(serv.get_label(), user.at(0), user.at(1)));
					
				for(unsigned int m = 0; m < modlist.size(); m++)
				{
					modlist.at(m).do_process(e);
				}
            }
            //----------------------------------------------------------------------------
            else if(pos2 == "PART") // -- On déclenche tout les modules bindés sur le PART
            {
                string chan = "";
                in >> chan;
                string message = "";
                getline(in, message);
                if(message.length() > 2)
					message = message.substr(1);
				
				Event *e = new Event;
		        e->set_type(PART);
				e->set_raw(raw);
				e->set_bot(this);
				e->set_serv(&serv);
				e->set_channel(chan);
				e->set_message(message);
				e->set_author(user);
				e->set_user(user_get_ptr_by_nick_host(serv.get_label(), user.at(0), user.at(1)));
					
				for(unsigned int m = 0; m < modlist.size(); m++)
				{
					modlist.at(m).do_process(e);
				}
            }
            //----------------------------------------------------------------------------
            else if(pos2 == "JOIN") // -- On déclenche tout les modules bindés sur le JOIN
            {
                string chan = "";
                in >> chan;
                chan = chan.substr(1);
                
                Event *e = new Event;
		        e->set_type(JOIN);
				e->set_raw(raw);
				e->set_bot(this);
				e->set_serv(&serv);
				e->set_channel(chan);
				e->set_author(user);
				e->set_user(user_get_ptr_by_nick_host(serv.get_label(), user.at(0), user.at(1)));
					
				for(unsigned int m = 0; m < modlist.size(); m++)
				{
					modlist.at(m).do_process(e);
				}
            }
            //----------------------------------------------------------------------------
            else if(pos2 == "PRIVMSG") // -- On déclenche tout les modules bindés sur le PM
            {
                string chan = "";
                in >> chan;
                string message = "";
                getline(in, message);
                message = message.substr(2);
                if(message.at(0) == '\001')
                {
                    message = message.substr(1);
                    istringstream ctcp_in(message);
                    string commande = "";
                    ctcp_in >> commande;
                    string parametre;
                    getline(ctcp_in, parametre);
                    if(parametre != "\001" and parametre.length() > 1)
						parametre = parametre.substr(1);
						
                    if(commande == "ACTION")
                    {
                     	Event *e = new Event;
                     	string act = message.substr(7);
                     	act = act.substr(0, act.length()-1);
                     	
		                e->set_type(ACTION);
						e->set_raw(raw);
						e->set_bot(this);
						e->set_serv(&serv);
						e->set_channel(chan);
						e->set_author(user);
						e->set_message(act);
						e->set_user(user_get_ptr_by_nick_host(serv.get_label(), user.at(0), user.at(1)));
					
						for(unsigned int m = 0; m < modlist.size(); m++)
						{
							modlist.at(m).do_process(e);
						}  
						
                        return;
                    }
                    else
                    {
                    	// CTCP
                    	
                        Event *e = new Event;
		                e->set_type(CTCP);
						e->set_raw(raw);
						e->set_bot(this);
						e->set_serv(&serv);
						e->set_channel(chan);
						e->set_author(user);
						e->set_command(commande.substr(0, commande.length()-1));
						e->set_parameters(parametre);
						e->set_user(user_get_ptr_by_nick_host(serv.get_label(), user.at(0), user.at(1)));
					
						for(unsigned int m = 0; m < modlist.size(); m++)
						{
							Module_info* mi = modlist.at(m).module_info();
							if(mi->is_binded(CTCP))
								modlist.at(m).do_process(e);
						}
						return;
                    }
                }
                else
                {
                    if(message != "" && message.at(0) == CMD_CHAR && message.length() > 2)
                    {
                    	istringstream in(message);
                    	string com;
                    	in >> com;
                    	string mod, cmd;
                    	
          				if(com.find(SEP_CHAR) == string::npos)
          				{
          					mod = "";
          					cmd = com.substr(1, com.find(SEP_CHAR)-1);
          				}
          				else
          				{
                    		mod = com.substr(1, com.find(SEP_CHAR)-1);
                    		cmd = com.substr(com.find(SEP_CHAR)+1);
                    	}
                    	
                    	string parametre;
                    	
                    	getline(in, parametre);
                    	if(parametre.length() > 1)
                    	{
                    		parametre = parametre.substr(1);
                    	}
                    	
                    	Event *e = new Event;
                    	e->set_type(COMMAND);
                    	e->set_raw(raw);
                    	e->set_bot(this);
                    	e->set_serv(&serv);
                    	e->set_channel(chan);
                    	e->set_author(user);
                    	e->set_module(mod);
                    	e->set_command(cmd);
                    	e->set_parameters(parametre);
                    	e->set_user(user_get_ptr_by_nick_host(serv.get_label(), user.at(0), user.at(1)));
						
						for(unsigned int m = 0; m < modlist.size(); m++)
						{
							modlist.at(m).do_process(e);
						}
						
						return;
                    	
                    }
                    else
                    {
		                Event *e = new Event;
		                e->set_type(PRIVMSG);
						e->set_raw(raw);
						e->set_bot(this);
						e->set_serv(&serv);
						e->set_channel(chan);
						e->set_author(user);
						e->set_message(message);
						e->set_user(user_get_ptr_by_nick_host(serv.get_label(), user.at(0), user.at(1)));
					
						for(unsigned int m = 0; m < modlist.size(); m++)
						{
							modlist.at(m).do_process(e);
						}
					}
					
					return;
                }
            }
            //----------------------------------------------------------------------------
            else if(pos2 == "KICK") // -- On déclenche tout les modules bindés sur le KICK
            {
                string chan = "";
                in >> chan;
                string cible = "";
                in >> cible;
                string message = "";
                getline(in, message);
                message = message.substr(2);
                
                Event *e = new Event;
				e->set_type(KICK);
				e->set_raw(raw);
				e->set_bot(this);
				e->set_serv(&serv);
				e->set_channel(chan);
				e->set_author(user);
				e->set_target(cible);
				e->set_message(message);
				e->set_user(user_get_ptr_by_nick_host(serv.get_label(), user.at(0), user.at(1)));
					
				for(unsigned int m = 0; m < modlist.size(); m++)
				{
					modlist.at(m).do_process(e);
				}
            }
            //----------------------------------------------------------------------------
            else if(pos2 == "NICK") // -- On déclenche tout les modules bindés sur le NICK
            {
                string newnick = "";
                getline(in, newnick);
                newnick = newnick.substr(2);
                
                Event *e = new Event;
				e->set_type(NICK);
				e->set_raw(raw);
				e->set_bot(this);
				e->set_serv(&serv);
				e->set_author(user);
				e->set_newnick(newnick);
				e->set_user(user_get_ptr_by_nick_host(serv.get_label(), user.at(0), user.at(1)));
					
				for(unsigned int m = 0; m < modlist.size(); m++)
				{
					modlist.at(m).do_process(e);
				}
            }
            //----------------------------------------------------------------------------
            else if(pos2 == "NOTICE") // -- On déclenche tout les modules bindés sur le NOTICE
            {
                string cible = "";
                in >> cible;
                string message = "";
                getline(in, message);
                message = message.substr(2);
                
                Event *e = new Event;
				e->set_type(NOTICE);
				e->set_raw(raw);
				e->set_bot(this);
				e->set_serv(&serv);
				e->set_author(user);
				e->set_channel(cible);
				e->set_target(cible);
				e->set_message(message);
				e->set_user(user_get_ptr_by_nick_host(serv.get_label(), user.at(0), user.at(1)));
					
				for(unsigned int m = 0; m < modlist.size(); m++)
				{
					modlist.at(m).do_process(e);
				}
            }
            //----------------------------------------------------------------------------
            else if(pos2 == "MODE") // -- On déclenche tout les modules bindés sur le MODE
            {
                string chan = "";
                in >> chan;
                string mds = "";
                in >> mds;
                string cible;
                getline(in, cible);
                if(cible.length() > 1)
                	cible = cible.substr(1);
                
                vector<string> modes;
                vector<string> nicks;
                char opt = '+';
                
                for(unsigned int i = 0; i < mds.length(); i++)
                {
                	if(mds.at(i) == '-')
                		opt = '-';
                	else if(mds.at(i) == '+')
                		opt = '+';
                	else
                	{
                		string tmp;
                		tmp += opt;
                		tmp += mds.at(i);
                		modes.push_back(tmp);
                	}
                }
                
                string nick;
                istringstream nstream(cible);
                
                string tmp = "a";
                
                while(tmp != "")
                {
                	tmp = "";
                	nstream >> tmp;
                	if(tmp != "")
                		nicks.push_back(tmp);
                }
                
                if(nicks.size() < modes.size())
                {
                	while(nicks.size() < modes.size())
                	{
                		if(nicks.size() > 0)
                			nicks.push_back(nicks.at(nicks.size()-1));
                		else
                			nicks.push_back("");
                	}
                }
                
                if(modes.size() < nicks.size())
                {
                	while(modes.size() < nicks.size())
                	{
                		if(modes.size() > 0)
                			modes.push_back(modes.at(modes.size()-1));
                		else
                			modes.push_back("");
                	}
                }
                
                Event *e = new Event;
				e->set_type(MODE);
				e->set_raw(raw);
				e->set_bot(this);
				e->set_serv(&serv);
				e->set_author(user);
				e->set_channel(chan);
				e->set_nicks(nicks);
				e->set_modes(modes);
				e->set_user(user_get_ptr_by_nick_host(serv.get_label(), user.at(0), user.at(1)));
					
				for(unsigned int m = 0; m < modlist.size(); m++)
				{
					modlist.at(m).do_process(e);
				}
            }
            //----------------------------------------------------------------------------
        }
    }
}
