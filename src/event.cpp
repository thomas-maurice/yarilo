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


#include <event.h>

Event::Event()
{
	bot = NULL;
	serv = NULL;
	user = NULL;
}

Event::~Event()
{
	author.push_back("");
	author.push_back("");
}

int Event::get_type()
{
	return type;
}

void Event::set_type(int p_type)
{
	type = p_type;
}

Bot* Event::get_bot()
{
	return bot;
}

void Event::set_bot(Bot* p_bot)
{
	bot = p_bot;
}

Server* Event::get_serv()
{
	return serv;
}

void Event::set_serv(Server* p_serv)
{
	serv = p_serv;
}

std::string Event::get_raw()
{
	return raw;
}

void Event::set_raw(std::string p_raw)
{
	raw = p_raw;
}


User* Event::get_user()
{
	return user;
}

void Event::set_user(User* p_user)
{
	user = p_user;
}

std::vector<std::string> Event::get_author()
{
	return author;
}

void Event::set_author(std::vector<std::string> p_author)
{
	author = p_author;
}

std::string Event::get_message()
{
	return message;
}

void Event::set_message(std::string p_message)
{
	message = p_message;
}

std::string Event::get_target()
{
	return target;
}

void Event::set_target(std::string p_target)
{
	target = p_target;
}

std::string Event::get_channel()
{
	return channel;
}

void Event::set_channel(std::string p_channel)
{
	channel = p_channel;
}

std::string Event::get_command()
{
	return command;
}

void Event::set_command(std::string p_command)
{
	command = p_command;
}

std::string Event::get_parameters()
{
	return parameters;
}

void Event::set_parameters(std::string p_parameters)
{
	parameters = p_parameters;
}

std::string Event::get_module()
{
	return module;
}

void Event::set_module(std::string p_module)
{
	module = p_module;
}

int Event::get_code()
{
	return code;
}

void Event::set_code(int p_code)
{
	code = p_code;
}

std::string Event::get_uhost()
{
	return author.at(1);
}

std::string Event::get_unick()
{
	return author.at(0);
}

std::string Event::get_newnick()
{
	return newnick;
}

void Event::set_newnick(std::string p_newnick)
{
	newnick = p_newnick;
}

std::vector<std::string> Event::get_modes()
{
	return modes;
}

void Event::set_modes(std::vector<std::string> p_modes)
{
	modes = p_modes;
}

std::vector<std::string> Event::get_nicks()
{
	return nicks;
}

void Event::set_nicks(std::vector<std::string> p_nicks)
{
	nicks = p_nicks;
}
