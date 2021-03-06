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

	PrintBootMessage();
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

void RamAi::Api::InitialiseGame(const GameSettings &gameSettings,
	const StateMachine::SaveStateHandleSignature &saveStateHandle,
	const StateMachine::LoadStateHandleSignature &loadStateHandle,
	const ScoreLog::SaveLogToFileSignature &saveLogToFileHandle,
	const StateMachine::StartRecordingHandleSignature &startRecordingHandle,
	const StateMachine::FinishRecordingHandleSignature &finishRecordingHandle)
{
	Debug::ClearScreen();

	GameSettings::SetInstance(gameSettings);

	//Create a new state machine.
	m_stateMachine = std::make_unique<StateMachine>(saveLogToFileHandle);
	m_stateMachine->GetSaveStateHandle() = saveStateHandle;
	m_stateMachine->GetLoadStateHandle() = loadStateHandle;
	m_stateMachine->GetStartRecordingHandle() = startRecordingHandle;
	m_stateMachine->GetFinishRecordingHandle() = finishRecordingHandle;

	//Log any errors.
	if (!gameSettings.IsValid())
	{
		Debug::OutLine("Current game settings are invalid.", Colour::Red);
	}

	if (!saveLogToFileHandle)
	{
		Debug::OutLine("No save log to file handle - score log will not be saved to disk.", Colour::Red);
	}
}

void RamAi::Api::ImportAiSettings(char *settingsFile)
{
	AiSettings::SetData(AiSettings::Import(settingsFile));
}

RamAi::ButtonSet RamAi::Api::CalculateInput(const Ram &ram)
{
	ButtonSet returnValue;

	assert(m_stateMachine);

	if (m_stateMachine)
	{
		assert(m_stateMachine->IsCurrentStateValid());

		if (m_stateMachine->IsCurrentStateValid())
		{
			returnValue = m_stateMachine->CalculateInput(ram);
		}
	}

	return returnValue;
}

void RamAi::Api::PrintBootMessage()
{
	Debug::OutLine("RamAi instantiated.");
	Debug::OutLine("Branching factor: " + std::to_string(ConsoleSettings::GetSpecs().GetNumberOfInputCombinations()));
	Debug::OutLine("Waiting for game...");
}

void RamAi::Api::Move(Api &&other)
{
	m_stateMachine = std::move(other.m_stateMachine);
}