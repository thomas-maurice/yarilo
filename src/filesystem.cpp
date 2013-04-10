/*
 *
 *	Copyright (C) 2013 Thomas MAURICE
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
 
#include <filesystem.h>

using namespace std;

int make_directory(string path) {
	#ifdef linux
		return mkdir(path.c_str(), 0755);
	#elif defined WIN32
		return CreateDirectory(path.c_str(), NULL);
	#endif
}

bool directory_exists(string dir) {
	DIR* d = opendir(dir.c_str());
	if(d ==NULL) {
		closedir(d);
		return false;
	} else {
		closedir(d);
		return true;
	}
}

void build_standard_fs() {
	if(!directory_exists("conf")) make_directory("conf");
	if(!directory_exists("conf/users")) make_directory("conf/users");
	if(!directory_exists("data")) make_directory("data");
	if(!directory_exists("modules")) make_directory("modules");
}

bool is_standard_fs() {
	if(!directory_exists("conf")) return false;
	if(!directory_exists("conf/users")) return false;
	if(!directory_exists("data")) return false;
	if(!directory_exists("modules")) return false;
	
	return true;
}
