#include <iostream>
#include <bot_api.h>
#include <libplugin.h>
#include "megahal_func.h"

using namespace std;

vector<string> learnIgnoreNicks;
vector<string> replyIgnoreNicks;
vector<string> learnChans;
vector<string> replyChans;
vector<string> hlReplyChans;
vector<string> learnRegexpExcludingFilters;
vector<string> replyRegexpExcludingFilters;

int learnProba;
int replyProba;
int hlReplyProba;

bool is_learning;
bool is_replying;
bool is_hl_replying;

int random(int min, int max)
{
    srand(time(NULL));
    int result = 0;
    result = (rand() % (max - min + 1)) + min;
    
    return result;
}

bool is_a_hl(std::string nick, std::string message)
{
    if(message.find(nick) != string::npos)
    {
        return true;
    }
    return false;
}

void load_config()
{
	ifstream fichier("data/megahal.conf", ios::in);  // on ouvre en lecture

    if(fichier)  // si l'ouverture a fonctionné
    {
        learnIgnoreNicks.clear();
        replyIgnoreNicks.clear();
        learnChans.clear();
        replyChans.clear();
        hlReplyChans.clear();
        learnRegexpExcludingFilters.clear();
        replyRegexpExcludingFilters.clear();
        
        learnProba = 0;
        replyProba = 1000;
        hlReplyProba = 1000;
        
        is_learning = false;
        is_replying = false;
        is_hl_replying = false;
        
        string ligne;  // déclaration d'une chaîne qui contiendra la ligne lue
        while(getline(fichier, ligne))
        {
            if(ligne == "" || ligne == "\n")
            {
                continue;
            }
            else if(ligne.at(0) == '#')
            {
                continue;
            }
            // Si la ligne n'est ni vide, ni un retour a la ligne
            // ni n'est une ligne de commentaire alors on peut
            // envisager que la ligne soit une commande
            istringstream in(ligne);
            string commande;
            in >> commande;
            
            if(lowercase(commande) == "replyproba")
            {
                in >> replyProba;
                //cout << "replyproba:" << replyProba << endl;
            }
            else if(lowercase(commande) == "hlreplyproba")
            {
                in >> hlReplyProba;
                //cout << "hlreplyproba:" << hlReplyProba << endl;
            }
            else if(lowercase(commande) == "learnproba")
            {
                in >> learnProba;
                //cout << "learnproba:" << learnProba << endl;
            }
            else if(lowercase(commande) == "learnexcludingregexp")
            {
                string tmp = "";
                getline(in, tmp);
                if(tmp.length() >= 1)
                {
                    tmp = tmp.substr(1);
                }
                
                learnRegexpExcludingFilters.push_back(tmp);
                //cout << "Filtre d'exclusion learn:" << tmp << endl;
            }
            else if(lowercase(commande) == "replyexcludingregexp")
            {
                string tmp = "";
                getline(in, tmp);
                if(tmp.length() >= 1)
                {
                    tmp = tmp.substr(1);
                }
                
                replyRegexpExcludingFilters.push_back(tmp);
                //cout << "Filtre d'exclusion reply:" << tmp << endl;
            }
            else if(lowercase(commande) == "learnchan")
            {
                string tmp = "";
                getline(in, tmp);
                if(tmp.length() >= 1)
                {
                    tmp = tmp.substr(1);
                }
                
                learnChans.push_back(tmp);
                //cout << "Learnchan:" << tmp << endl;
            }
            else if(lowercase(commande) == "replychan")
            {
                string tmp = "";
                getline(in, tmp);
                if(tmp.length() >= 1)
                {
                    tmp = tmp.substr(1);
                }
                
                replyChans.push_back(tmp);
                //cout << "replychan:" << tmp << endl;
            }
            else if(lowercase(commande) == "hlreplychan")
            {
                string tmp = "";
                getline(in, tmp);
                if(tmp.length() >= 1)
                {
                    tmp = tmp.substr(1);
                }
                
                hlReplyChans.push_back(tmp);
                //cout << "hlreplychan:" << tmp << endl;

            }
            else if(lowercase(commande) == "learnignorenick")
            {
                string tmp = "";
                getline(in, tmp);
                if(tmp.length() >= 1)
                {
                    tmp = tmp.substr(1);
                }
                
                learnIgnoreNicks.push_back(tmp);
                //cout << "learnignorenick:" << tmp << endl;

            }
            else if(lowercase(commande) == "replyignorenick")
            {
                string tmp = "";
                getline(in, tmp);
                if(tmp.length() >= 1)
                {
                    tmp = tmp.substr(1);
                }
                
                replyIgnoreNicks.push_back(tmp);
                //cout << "replyignorenick:" << tmp << endl;

            }
            else if(lowercase(commande) == "nolearn")
            {
                is_learning = false;
            }
            else if(lowercase(commande) == "learn")
            {
                is_learning = true;
            }
            else if(lowercase(commande) == "noreply")
            {
                is_replying = false;
            }
            else if(lowercase(commande) == "reply")
            {
                is_replying = true;
            }
            else if(lowercase(commande) == "nohlreply")
            {
                is_hl_replying = false;
            }
            else if(lowercase(commande) == "hlreply")
            {
                is_hl_replying = true;
            }
        }
    }
}

// Retourne un true si une correspondance a été trouvée et false sinon
int match_regex_list(std::string chaine, std::vector<std::string> liste)
{
    for(unsigned int i = 0 ; i < liste.size() ; i++)
    {
        if(regex_match(chaine, liste.at(i)) == true)
        {
            return true;
        }
    }
    
    return false;
}

bool bot_can_learn(string label, vector<string> author, string channel, string message)
{
	bool resp = true;
	
	// Global check
	if(!is_learning)
		return false;
	
	// Are we on a learn chan ?
	bool learnchan = false;
	for(unsigned int i = 0; i < learnChans.size(); i++)
	{
		string tmp = learnChans.at(i);
		istringstream in(tmp);
		string serv, chan;
		in >> serv >> chan;
		if(serv == label && chan == channel)
		{
			learnchan = true;
		}
	}
	
	if(!learnchan)
		return false;
		
	// Ignore user ?
	for(unsigned int i = 0 ; i < learnIgnoreNicks.size() ; i++)
    {
        string nickatserv = learnIgnoreNicks.at(i);
        istringstream in(nickatserv);
        string serv = "";
        string nick = "";
        in >> serv >> nick;
        if(nick == author.at(0) and serv == label)
        {
            return false;
        }
    }
	
	// Excluding regex matched ?
	if(match_regex_list(message, learnRegexpExcludingFilters) == false)
	{
		resp = true;
	}
	else
	{
		return false;
	}
	
	// Now checking the probas
	if(learnProba == 0 || random(0, learnProba) == 0)
		resp = true;
	else
		resp = false;
	
	return resp;
}

bool bot_can_reply(string label, vector<string> author, string channel, string message, string mynick)
{
	bool resp = false;;
	
	if(is_a_hl(mynick, message)) // HL
	{
		if(!is_hl_replying)
			return false;
			
		// Are we on ah hlreplychan ?
		bool replychan = false;
		for(unsigned int i = 0; i < hlReplyChans.size(); i++)
		{
			string tmp = hlReplyChans.at(i);
			istringstream in(tmp);
			string serv, chan;
			in >> serv >> chan;
			if(serv == label && chan == channel)
			{
				replychan = true;
			}
		}
	
		if(!replychan)
			return false;
		
		// Ignore user ?
		for(unsigned int i = 0 ; i < replyIgnoreNicks.size() ; i++)
		{
		    string nickatserv = replyIgnoreNicks.at(i);
		    istringstream in(nickatserv);
		    string serv = "";
		    string nick = "";
		    in >> serv >> nick;
		    if(nick == author.at(0) and serv == label)
		    {
		        return false;
		    }
		}
	
		// Excluding regex matched ?
		if(match_regex_list(message, replyRegexpExcludingFilters) == false)
		{
			resp = true;
		}
		else
		{
			return false;
		}
	
		// Now checking the probas
		if(hlReplyProba == 0 || random(0, hlReplyProba) == 0)
			resp = true;
		else
			resp = false;
		
		return resp;
	}

	// Global check
	if(!is_replying)
		return false;
		
	// Are we on a reply chan ?
	bool replychan = false;
	
	for(unsigned int i = 0; i < replyChans.size(); i++)
	{
		string tmp = replyChans.at(i);
		istringstream in(tmp);
		string serv, chan;
		in >> serv >> chan;
		if(serv == label && chan == channel)
		{
			replychan = true;
		}
	}

	if(!replychan)
		return false;
					
	// Ignore user ?
	for(unsigned int i = 0 ; i < replyIgnoreNicks.size() ; i++)
    {
        string nickatserv = replyIgnoreNicks.at(i);
        istringstream in(nickatserv);
        string serv = "";
        string nick = "";
        in >> serv >> nick;
        if(nick == author.at(0) and serv == label)
        {
            return false;
        }
    }
	
	// Excluding regex matched ?
	if(match_regex_list(message, replyRegexpExcludingFilters) == false)
	{
		resp = true;
	}
	else
	{
		return false;
	}
	
	// Now checking the probas
	if(replyProba == 0 || random(0, replyProba) == 0)
	{
		resp = true;
	}
	else
	{
		resp = false;
	}

	return resp;
}
