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

#if defined (WIN32)
void color_console(int fg,int bg);
#else
void color_console(std::string fg, std::string bg);
#endif

void clear_console();
void reinit_console();

#if defined (WIN32)
	#include <windows.h>
	#define BLACK 		0
	#define DARK_BLUE 	1
	#define GREEN 		2
	#define TEAL 		3
	#define MAROON 		4
	#define DARK_RED 	5
	#define KAKI 		6
	#define LIGHT_GREY 	7
	#define GREY		8
	#define BLUE		9
	#define FLUO_GREEN 	10
	#define GREEN_BLUE	11
	#define RED			12
	#define FLUO_PINK	13
	#define YELLOW		14
	#define WHITE		15
#else
	#define BLACK 		"00"
	#define DARK_BLUE 	"4"
	#define GREEN 		"2"
	#define TEAL 		"6"
	#define MAROON 		"1"
	#define DARK_RED 	"1"
	#define KAKI 		"2"
	#define LIGHT_GREY 	"7"
	#define GREY		"7"
	#define BLUE		"4"
	#define FLUO_GREEN 	"2"
	#define GREEN_BLUE	"6"
	#define RED			"1"
	#define FLUO_PINK	"5"
	#define YELLOW		"3"
	#define WHITE		"7"
#endif
