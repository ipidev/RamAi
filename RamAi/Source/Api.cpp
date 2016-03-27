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
#include "Settings\AiSettings.h"


RamAi::Api::Api(const ConsoleSettings::Specs &consoleSpecs)
	: Api(consoleSpecs, nullptr)
{
}

RamAi::Api::Api(const ConsoleSettings::Specs &consoleSpecs, std::unique_ptr<Debug> &&debugInstance)
{
	ConsoleSettings::SetSpecs(consoleSpecs);
	Debug::SetInstance(std::move(debugInstance));

	srand(static_cast<unsigned int>(time(NULL)));

	Debug::Out("RamAi instantiated. Waiting for game...");
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

void RamAi::Api::InitialiseGame(const GameSettings &gameSettings, const StateMachine::SaveStateHandleSignature &saveStateHandle, const StateMachine::LoadStateHandleSignature &loadStateHandle)
{
	Debug::ClearScreen();

	//Create a new state machine.
	m_stateMachine = std::make_unique<StateMachine>(gameSettings);
	m_stateMachine->GetSaveStateHandle() = saveStateHandle;
	m_stateMachine->GetLoadStateHandle() = loadStateHandle;

	if (!gameSettings.IsValid())
	{
		Debug::OutLine("Current game settings are invalid.", Colour::Red);
	}
}

void RamAi::Api::ImportAiSettings(char *settingsFile)
{
	AiSettings::Import(settingsFile);
}

RamAi::ButtonSet RamAi::Api::CalculateInput(const Ram &ram)
{
	ButtonSet returnValue;

	assert(m_stateMachine);

	if (m_stateMachine)
	{
		//TODO: Still uses a random input when there is no valid current state.
		if (m_stateMachine->IsCurrentStateValid())
		{
			returnValue = m_stateMachine->CalculateInput(ram);
		}
		else
		{
			//For now just generate random button presses.
			const size_t buttonSetSize = 1 << ConsoleSettings::GetSpecs().numberOfGamePadButtons;
			returnValue = ButtonSet(rand() % 256);
			returnValue.GetBitfield().UnsetAll(ConsoleSettings::GetSpecs().initialisationButtonSet.GetBitfield().GetValue());
		}
	}

	return returnValue;
}

void RamAi::Api::Move(Api &&other)
{
	m_stateMachine = std::move(other.m_stateMachine);
}