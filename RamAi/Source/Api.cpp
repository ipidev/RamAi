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

#include <cassert>
#include <cstdlib>
#include <ctime>

#include "MonteCarlo\TestMonteCarloTree.h"


RamAi::Api::Api(const Console::Specs &consoleSpecs)
{
	Console::SetSpecs(consoleSpecs);

	srand(static_cast<unsigned int>(time(NULL)));
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

void RamAi::Api::InitialiseGame(const GameDetails &gameDetails)
{
	//Create a new state machine.
	m_stateMachine = std::make_unique<StateMachine>(gameDetails);
}

RamAi::ButtonSet RamAi::Api::CalculateInput()
{
	ButtonSet returnValue;

	assert(m_stateMachine);

	if (m_stateMachine)
	{
		//TODO: Still uses a random input when there is no valid current state.
		if (m_stateMachine->IsCurrentStateValid())
		{
			//TODO: Need to provide actual RAM.
			Ram ram(Console::GetSpecs().ramSize);

			returnValue = m_stateMachine->CalculateInput(ram);
		}
		else
		{
			//For now just generate random button presses.
			const size_t buttonSetSize = 1 << Console::GetSpecs().numberOfGamePadButtons;
			returnValue = ButtonSet(rand() % 256);
		}
	}

	return returnValue;
}

void RamAi::Api::Move(Api &&other)
{
	m_stateMachine = std::move(other.m_stateMachine);
}