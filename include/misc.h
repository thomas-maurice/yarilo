
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
#include <iomanip>
#include <ctime>

#ifndef MISC_DEF
#define MISC_DEF

//--------------------------
// Options
//--------------------------


#define DBG 0

// -------------------------
#if defined (linux)
	
#elif defined (WIN32)
	#include <windows.h>
#endif

#if defined (linux)
	#define sleep_(sec) usleep(sec * 1000);
#elif defined (WIN32)
	#define sleep_(sec) Sleep(sec);
#endif

// Macros et autres

#define Debug(str) 	if (DBG == 1) \
						std::cout << __FILE__ << " " << __LINE__ << " : " << str << std::endl;

#if defined (WIN32)
    #include <windows.h>
    #define mkdir_(nom) CreateDirectory(nom, NULL);
#elif defined (linux)
	#include <sys/types.h>
	#include <sys/stat.h>
    #define mkdir_(nom) mkdir(nom, 0766);
#endif

// Fonctions
std::string uppercase(std::string input);
std::string lowercase(std::string input);
std::string random_string(int len, bool majs = true, bool num = true);
bool regex_match(std::string chaine, std::string expression);
std::string sha1(std::string input);
#endif
