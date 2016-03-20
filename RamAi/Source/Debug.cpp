/*
	RamAi - A general game-playing AI that uses RAM states as input to a value function
	Copyright (C) 2016 Sean Latham

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

#include "Debug.h"

RamAi::Debug::Debug()
{
}

RamAi::Debug::~Debug()
{
}

void RamAi::Debug::Out(const std::string &string, const Colour colour)
{
	if (s_instance)
	{
		s_instance->InstanceOut(string, colour);
	}
}

void RamAi::Debug::ClearScreen()
{
	if (s_instance)
	{
		s_instance->InstanceClearScreen();
	}
}

std::unique_ptr<RamAi::Debug> RamAi::Debug::s_instance = nullptr;