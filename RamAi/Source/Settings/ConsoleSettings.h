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
	//An enum storing the directions that are pressable on a d-pad.
	enum DirectionalPad
	{
		Up,
		Down,
		Left,
		Right,
		Max
	};

	//Static class that holds information of the game console currently in use.
	class ConsoleSettings
	{
	public:
		//A plain-old-data struct that holds all of the specifications.
		struct Specs
		{
			Specs() = default;

			//The number of frames executed per second.
			size_t frameRate;

			//The button(s) needed to press to skip any title screen/menus.
			ButtonSet initialisationButtonSet;

			//A button set for each direction on the d-pad.
			//This structure ensures we can't press left/right or up/down at the same time.
			ButtonSet directionalPadFields[DirectionalPad::Max];

			//A single button set that represents the other pressable inputs on the gamepad.
			//These may be pressed simultaneously.
			ButtonSet buttonsField;

		public:
			uint32_t GetNumberOfInputCombinations() const;

			std::vector<ButtonSet> GetAllInputCombinations() const;

		public:
			ButtonSet GenerateRandomInput() const	{ return GetRandomDirection() | GetRandomButton(); }

			ButtonSet GetRandomDirection() const;
			ButtonSet GetRandomButton() const;
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
