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
#include "Game\GameDetails.h"
#include "Game\StateMachine\StateMachine.h"
#include "Console.h"
#include "Debug.h"


namespace RamAi
{
	//The main interface with RamAi.
	//TODO: Will I need to create derived types to interface with specific consoles?
	class Api
	{
	public:
		Api(const Console::Specs &consoleSpecs);
		Api(const Console::Specs &consoleSpecs, std::unique_ptr<Debug> &&debugInstance);
		Api(const Api &other) = delete;
		Api(Api &&other);
		~Api();

	public:
		Api &operator= (const Api &other) = delete;
		Api &operator= (Api &&other);

	public:
		//Initialises the AI with a new game's settings.
		void InitialiseGame(const GameDetails &gameDetails, const StateMachine::SaveStateHandleSignature &saveStateHandle, const StateMachine::LoadStateHandleSignature &loadStateHandle);

	public:
		ButtonSet CalculateInput();

	private:
		void Move(Api &&other);

	private:
		std::unique_ptr<StateMachine> m_stateMachine;
	};
};