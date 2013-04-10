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



#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <logs.h>

using namespace std;

void log(std::string source, std::string event)
{
    if(event == "" || source == "")
        return;
    
    const char * NumMois[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};

    time_t timestamp;
    struct tm * t;
    
    timestamp = time(NULL);
    t = localtime(&timestamp);
    char date[100];
    char heure[100];
    sprintf(date, "%02u/%s/%04u", t->tm_mday, NumMois[t->tm_mon], 1900 + t->tm_year);
    sprintf(heure, "%02uh%02um%02us", t->tm_hour, t->tm_min, t->tm_sec);
    string dt = "";
    dt += date;
    dt += " ";
    dt += heure;
    
    ofstream fichier("bot.log", ios::out | ios::app);
 
    if(fichier)
    {
            fichier << dt << " " << source << " : " << event << endl;

            fichier.close();
    }
    else
    {
        /* Cas qui ne doit normalement jamais se produire */
    }
}
