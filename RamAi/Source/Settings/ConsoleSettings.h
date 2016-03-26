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

#include "Action\ButtonSet.h"


namespace RamAi
{
	//Static class that holds information of the game console currently in use.
	class ConsoleSettings
	{
	public:
		//A plain-old-data struct that holds all of the specifications.
		struct Specs
		{
			Specs() = default;

			size_t ramSize;
			size_t numberOfGamePadButtons;
			ButtonSet initialisationButtonSet;		//The button(s) needed to press to skip any title screen/menus.
		};

	public:
		//Static class - construction is forbidden.
		ConsoleSettings() = delete;
		~ConsoleSettings() = delete;

	public:
		static const Specs &GetSpecs()				{ return s_specs; }
		static void SetSpecs(const Specs &specs)	{ s_specs = specs; }

	private:
		static Specs s_specs;
	};
};
