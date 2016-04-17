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

#include <cassert>
#include <memory>

#include "Action\ButtonSet.h"
#include "Settings\ConsoleSettings.h"
#include "Settings\GameSettings.h"
#include "StateMachine\StateMachine.h"
#include "Debug.h"


namespace RamAi
{
	//The main interface with RamAi.
	//TODO: Will I need to create derived types to interface with specific consoles?
	class Api
	{
	public:
		Api(const ConsoleSettings::Specs &consoleSpecs);
		Api(const ConsoleSettings::Specs &consoleSpecs, std::unique_ptr<Debug> &&debugInstance);
		Api(const Api &other) = delete;
		Api(Api &&other);
		~Api();

	public:
		Api &operator= (const Api &other) = delete;
		Api &operator= (Api &&other);

	public:
		//Initialises the AI with a new game's settings.
		void InitialiseGame(const GameSettings &gameSettings,
			const StateMachine::SaveStateHandleSignature &saveStateHandle,
			const StateMachine::LoadStateHandleSignature &loadStateHandle,
			const ScoreLog::SaveLogToFileSignature &saveLogToFileHandle,
			const StateMachine::StartRecordingHandleSignature &startRecordingHandle,
			const StateMachine::FinishRecordingHandleSignature &finishRecordingHandle);

		void ImportAiSettings(char *settingsFile);

	public:
		ButtonSet CalculateInput(const Ram &ram);

	private:
		void PrintBootMessage();

	private:
		void Move(Api &&other);

	private:
		std::unique_ptr<StateMachine> m_stateMachine;
	};
};