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

#define IRC_WHITE    		0
#define IRC_BLACK   		1
#define IRC_BLUE   			2
#define IRC_GREEN  			3
#define IRC_RED   		 	4
#define IRC_MAROON    		5
#define IRC_ROYAL_RED    	6
#define IRC_ORANGE    		7
#define IRC_YELLOW    		8
#define IRC_LIGHT_GREEN    	9
#define IRC_CYAN    		10
#define IRC_LIGHT_CYAN    	11
#define IRC_LIGHT_BLUE    	12
#define IRC_PINK    		13
#define IRC_GREY    		14
#define IRC_LIGHT_GREY    	15

#define IRC_BOLD 			02
#define IRC_UNDERLINE		37

std::string irc_color(int fg, int bg = -1);

std::string irc_style(int style);

std::string irc_reinit();
