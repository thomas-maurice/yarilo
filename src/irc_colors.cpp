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


#include <irc_colors.h>
#include <sstream>
#include <iomanip>

using namespace std;

std::string irc_color(int fg, int bg)
{
	ostringstream o("");
	if(bg != -1)
	{
		o << "\003" << setw(2) << setfill('0') << fg;
		o << ",";
		o << setw(2) << setfill('0') << bg;
	}
	else
		o << "\003" << setw(2) << setfill('0') << fg;
	
	return o.str();
}

std::string irc_style(int style)
{
	string res = "";
	if(style == IRC_BOLD)
		res = "\002";
	else if(style == IRC_UNDERLINE)
		res = "\037";
	
	return res;
}

std::string irc_reinit()
{
	return "\017";
}

