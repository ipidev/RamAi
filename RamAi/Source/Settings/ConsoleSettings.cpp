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

#include "ConsoleSettings.h"

#include <cassert>


uint32_t RamAi::ConsoleSettings::Specs::GetNumberOfInputCombinations() const
{
	//TODO: Currently assumes non-diagonals.
	return static_cast<uint32_t>(DirectionalPad::Max) * buttonsField.GetNumberOfCombinations();
}

std::vector<RamAi::ButtonSet> RamAi::ConsoleSettings::Specs::GetAllInputCombinations() const
{
	std::vector<ButtonSet> returnValue;
	returnValue.reserve(GetNumberOfInputCombinations());

	//For each possible direction, get all of the button sets that contain it and the other buttons, and add them to the end of the list.
	for (size_t i = 0; i < DirectionalPad::Max; ++i)
	{
		std::vector<ButtonSet> combinationsForCurrentDirection = ButtonSet::GetAllCombinations(directionalPadFields[i].GetBitfield(), buttonsField.GetBitfield());
		returnValue.insert(returnValue.cend(), combinationsForCurrentDirection.begin(), combinationsForCurrentDirection.end());
	}

	//Sanity check - this should be all of the combinations possible!
	assert(returnValue.size() == GetNumberOfInputCombinations());

	return returnValue;
}

RamAi::ButtonSet RamAi::ConsoleSettings::Specs::GetRandomDirection() const
{
	//Simply choose a random direction. No diagonals... yet!
	size_t randomIndex = rand() % static_cast<size_t>(DirectionalPad::Max);

	return directionalPadFields[randomIndex];
}

RamAi::ButtonSet RamAi::ConsoleSettings::Specs::GetRandomButton() const
{
	Bitfield<ButtonSet::BitfieldType> randomBitfield = rand();

	return ButtonSet(randomBitfield & buttonsField.GetBitfield());
}

RamAi::ConsoleSettings::Specs RamAi::ConsoleSettings::s_specs = RamAi::ConsoleSettings::Specs();