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


#include <color_console.h>

using namespace std;

#if defined (WIN32)
void color_console(int fg,int bg)
#else
void color_console(string fg, string bg)
#endif
{
	#if defined (WIN32)
    	HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    		SetConsoleTextAttribute(H,bg*16+fg);
    #else
    	cout << "\033[3" + fg + "m" << "\033[4" + bg + "m";
    #endif
}

void clear_console()
{
	#if defined (WIN32)
    	system("cls"); // TO FIX !!!
    #else
    	cout << "\033[H\033[2J";
    #endif
}

void reinit_console()
{
	#if defined (WIN32)
    	color_console(WHITE, BLACK);
    #else
    	cout << "\033[0m";
    #endif
}
