/*
	RamAi - A general game-playing AI that uses RAM states as input to a value function
	Copyright (C) 2015 Sean Latham

	This program is free software; you can redistribute it and / or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.
*/

#pragma once

#include <memory>

//Laziness...
#define RAMAI_CONSOLE_SPECS_SPACE() 

#define RAMAI_CONSOLE_SPECS_ACCESSOR(TYPE, NAME, DEFAULT_VALUE) \
	public: static RAMAI_CONSOLE_SPECS_SPACE() ##TYPE Get##NAME() { return s_instance ? s_instance->InstanceGet##NAME() : ##DEFAULT_VALUE ; } \
	protected: virtual RAMAI_CONSOLE_SPECS_SPACE() ##TYPE InstanceGet##NAME() const = 0;


namespace RamAi
{
	//Singleton class for the specifications of the game console currently in use.
	//Settings for a specific console should derive from this and then be passed into SetInstance().
	class ConsoleSpecs
	{
	public:
		ConsoleSpecs() = default;
		~ConsoleSpecs() = default;

	public:
		static const std::unique_ptr<ConsoleSpecs> &GetInstance()			{ return s_instance; }
		static void SetInstance(std::unique_ptr<ConsoleSpecs> &&console)	{ s_instance = std::move(console); }

	protected:
		RAMAI_CONSOLE_SPECS_ACCESSOR(const size_t, RamSize, 0);

	private:
		static std::unique_ptr<ConsoleSpecs> s_instance;
	};


	std::unique_ptr<ConsoleSpecs> ConsoleSpecs::s_instance = nullptr;
};

#undef RAMAI_CONSOLE_SPECS_ACCESSOR
#undef RAMAI_CONSOLE_SPECS_SPACE