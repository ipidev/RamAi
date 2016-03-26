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

#pragma once

#include "StateMachine.h"


namespace RamAi
{
	//A state responsible for pressing the start button repeatedly until the game has definitely begun.
	class InitialisationState : public StateMachine::State
	{
	public:
		InitialisationState(StateMachine &stateMachine);
		InitialisationState(const InitialisationState &other) = delete;
		InitialisationState(InitialisationState &&other);
		~InitialisationState();

	public:
		InitialisationState &operator= (const InitialisationState &other) = delete;
		InitialisationState &operator= (InitialisationState &&other);

	public:
		virtual void OnStateEntered(const std::weak_ptr<State> &oldState, const Type oldStateType) override;

		virtual ButtonSet CalculateInput(const Ram &ram) override;

		virtual Type GetDesiredStateType(const Ram &ram) override;

		virtual void OnStateExited(const std::weak_ptr<State> &newState, const Type newStateType) override;

	private:
		size_t m_numberOfFramesExecuted;
	};
};