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

#include "Api.h"

#include <cstdlib>
#include <ctime>

#include "MonteCarlo\TestMonteCarloTree.h"


RamAi::Api::Api(const Console::Specs &consoleSpecs)
{
	Console::SetSpecs(consoleSpecs);

	srand(static_cast<unsigned int>(time(NULL)));

	TestMonteCarloTree mcts(0.5f);

	for (int i = 0; i < 100; ++i)
	{
		mcts.PerformSearch();
	}
}

RamAi::Api::Api(Api &&other)
{
	Move(std::move(other));
}

RamAi::Api::~Api()
{
}

RamAi::Api &RamAi::Api::operator= (Api &&other)
{
	Move(std::move(other));
	return *this;
}

RamAi::ButtonSet RamAi::Api::CalculateInput()
{
	//For now just generate random button presses.
	const size_t buttonSetSize = 1 << Console::GetSpecs().numberOfGamePadButtons;
	return ButtonSet(rand() % 256);
}

void RamAi::Api::Move(Api &&other)
{
}