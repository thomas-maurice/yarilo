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
#include <cstdlib>

#include <libtsocket.h>
#include <server.h>
#include <bot.h>
#include <misc.h>
#include <color_console.h>

using namespace std;

int main(int argc, char **argv)
{
	argc = argc;
	argv = argv;
	
	color_console(RED, TEAL);
	
	cout << "    _____.___.            .__.__                     " <<setw(30) << setfill(' ') << endl;       
	cout << "    \\__  |   |____ _______|__|  |   ____             " << endl; 
	cout << "     /   |    \\__  \\_  __ \\  |  |  /  _ \\            " << endl; 
	cout << "     \\____   |/ __  |  | \\/  |  |_(  <_> )           " << endl; 
	cout << "     / ______(____  /__|  |__|____/\\____/            " << endl; 
	cout << "     \\/           \\/  v0.01dev (c) Thomas Maurice    " << endl;
	cout << "                          http://www.yarilo.fr.nf    ";	
	
	reinit_console();
	
	cout << endl;
	
	Abstract_socket::Init_Network();

	Bot bot;
	
	bot.start();

	return EXIT_SUCCESS;
}
