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


#include <module_info.h>

using namespace std;

Module_info::Module_info()
{

}

Module_info::~Module_info()
{

}

void Module_info::bind(int b)
{
	binds.push_back(b);
}

bool Module_info::is_binded(int b)
{
	for(unsigned int i = 0; i < binds.size(); i++)
	{
		if(binds.at(i) == b)
			return true;
	}
	
	return false;
}

std::string Module_info::get_author()
{
	return author;
}

void Module_info::set_author(std::string p_author)
{
	author = p_author;
}

std::string Module_info::get_version()
{
	return version;
}

void Module_info::set_version(std::string p_version)
{
	version = p_version;
}

std::vector<int> Module_info::get_binds()
{
	return binds;
}

void Module_info::set_binds(std::vector<int> p_binds)
{
	binds = p_binds;
}

std::string Module_info::get_name()
{
	return name;
}

void Module_info::set_name(std::string p_name)
{
	name = p_name;
}

