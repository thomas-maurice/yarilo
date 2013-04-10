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


#include <misc.h>
#include <cstdio>
#include <sha1.h>
#include <stdio.h>
#include <stdlib.h>
extern "C" {
	#include <regex.h>
}
#include <time.h>

using namespace std;

std::string uppercase(std::string input)
{
    char maj[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char min[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    string ret = "";
    for(unsigned int i = 0 ; i < input.length() ; i++)
    {
        bool found = false;
        int index = -1;
        for(unsigned int j = 0 ; j < 26 ; j++)
        {
            if(input.at(i) == min[j])
            {
                found = true;
                index = j;
                break;
            }
        }
        if(found)
            ret += maj[index];
        else 
            ret += input.at(i);
    }
    
    return ret;
    
}

std::string lowercase(std::string input)
{
    char maj[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char min[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    string ret = "";
    for(unsigned int i = 0 ; i < input.length() ; i++)
    {
        bool found = false;
        int index = -1;
        for(unsigned int j = 0 ; j < 26 ; j++)
        {
            if(input.at(i) == maj[j])
            {
                found = true;
                index = j;
                break;
            }
        }
        if(found)
            ret += min[index];
        else 
            ret += input.at(i);
    }
    
    return ret;
    
}


std::string sha1(std::string input)
{
	string res = "";
	unsigned message_digest[5];
	SHA1 sha;
	sha.Reset();
	
	for(unsigned int i = 0; i < input.length(); i++)
	{
		sha.Input(input.at(i));
	}
	
	if(!sha.Result(message_digest))
	{
		return "";
	}
	else
	{
		char buf[100];
		sprintf(buf, "%08X%08X%08X%08X%08X",
					message_digest[0],
                    message_digest[1],
                    message_digest[2],
                    message_digest[3],
                    message_digest[4]);
		
		res = buf;
	}
	
	return lowercase(res);
}

bool regex_match(std::string chaine, std::string expression)
{
    int err;
    regex_t preg;
    int match = 0;
    
    err = regcomp (&preg, expression.c_str(), REG_NOSUB | REG_EXTENDED);
    if(err == 0)
    {
        match = regexec(&preg, chaine.c_str(), 0, NULL, 0);
        regfree(&preg);
		
		if(match == REG_NOMATCH)
			return false;
		else
			return true;
    }
    else
    {
        regfree(&preg);
        return false;
    }
	return false;
}

int random_number(int min, int max)
{
    srand(time(NULL));
    int result = 0;
    result = (rand() % (max - min + 1)) + min;
    
    return result;
}

std::string random_string(int len, bool majs, bool num)
{
	srand(time(NULL));
    string min = "azertyuiopqsdfghjklmwxcvbn";
	string maj = "AZERTYUIOPQSDFGHJKLMWXCVBN";
	string cf = "0123456789";
	string res = "";
	
	if(majs)
		min += maj;
	
	if(num)
		min += cf;
	
    const int MAX = min.length() - 1, MIN = 0;

    for(int i = 0; i < len ; i++)
	{
		int in = (rand() % (MAX - MIN + 1)) + MIN;
		res += min.at(in);
	}
	
	return res;
}
