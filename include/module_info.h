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
#include <iomanip>
#include <vector>

#ifndef MODULE_INFO
#define MODULE_INFO

class Module_info
{
	public:
		Module_info();
		~Module_info();
		
		std::string get_author();
		void set_author(std::string p_author);

		std::string get_version();
		void set_version(std::string p_version);

		std::vector<int> get_binds();
		void set_binds(std::vector<int> p_binds);

		std::string get_name();
		void set_name(std::string p_name);
		
		bool is_binded(int b);
		void bind(int b);
		
	private:
		std::string author;
		std::string version;
		std::vector<int> binds;
		std::string name;


};

#endif
