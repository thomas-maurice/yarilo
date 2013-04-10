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


#include <module.h>

Module::Module(std::string p_name)
{
	#if defined (WIN32)
		std::string ext = ".dll";
	#else
		std::string ext = ".so";
	#endif
	
	plugin = Plugin("modules/" + p_name + ext);
	name = p_name;
	
	load();
}

Module::~Module()
{

}

Plugin Module::get_plugin()
{
	return plugin;
}

bool Module::is_loaded()
{
	return plugin.is_loaded();
}

int Module::load()
{
	log("core", "Loading module " + name);
	return plugin.load();
}

int Module::unload()
{
	log("core", "Unloading module " + name);
	return plugin.unload();
}

int Module::reload()
{
	return plugin.reload();
}

Module_info* Module::module_info()
{
	void* vd = plugin.plugin_info();
	VOID_TO_OBJ(vd, Module_info, mi);
	return mi;
}

int Module::do_process(Event* event)
{
	OBJ_TO_VOID(event, arg);
	return plugin.do_process(arg);
}

std::string Module::get_name()
{
	return name;
}
