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


#include "InitialisationState.h"

#include <cassert>

#include "Console.h"


RamAi::InitialisationState::InitialisationState(StateMachine &stateMachine)
	: State(stateMachine)
{
	m_numberOfFramesExecuted = 0;
}

RamAi::InitialisationState::InitialisationState(InitialisationState &&other)
	: State(std::move(other))
{
	m_numberOfFramesExecuted = other.m_numberOfFramesExecuted;
}

RamAi::InitialisationState::~InitialisationState()
{
}

RamAi::InitialisationState &RamAi::InitialisationState::operator= (InitialisationState &&other)
{
	Move(std::move(other));

	m_numberOfFramesExecuted = other.m_numberOfFramesExecuted;

	return *this;
}

void RamAi::InitialisationState::OnStateEntered(const std::weak_ptr<State>& oldState, const Type oldStateType)
{
	State::OnStateEntered(oldState, oldStateType);

	m_numberOfFramesExecuted = 0;
}

RamAi::ButtonSet RamAi::InitialisationState::CalculateInput(const Ram &ram)
{
	ButtonSet returnValue;

	assert(m_stateMachine);

	if (m_stateMachine)
	{
		if (m_numberOfFramesExecuted < m_stateMachine->GetGameDetails().initialisationStartButtonFrames)
		{
			//Return the pause button on even frames, and the empty button set on odd frames.
			//This will cause the pause button to be mashed as fast as possible!
			if ((m_numberOfFramesExecuted % 2) == 0)
			{
				//TODO: Get actual pause value.
				returnValue = Console::GetSpecs().initialisationButtonSet;
			}
		}
	}

	++m_numberOfFramesExecuted;
	return returnValue;
}

RamAi::StateMachine::State::Type RamAi::InitialisationState::GetDesiredStateType(const Ram &ram)
{
	assert(m_stateMachine);

	if (m_stateMachine)
	{
		//Go to the selection state once we've executed enough frames to skip the title screen.
		const size_t initialisationFrames = m_stateMachine->GetGameDetails().GetMaximumInitialisationFrames();

		//We shouldn't ever go over the target number of frames - that means we must have skipped one!
		assert(m_numberOfFramesExecuted <= initialisationFrames);

		return m_numberOfFramesExecuted >= initialisationFrames ? Type::Expansion : Type::Initialisation;
	}
	else
	{
		return Type::Initialisation;
	}
}

void RamAi::InitialisationState::OnStateExited(const std::weak_ptr<State> &newState, const Type newStateType)
{
	State::OnStateExited(newState, newStateType);

	//Save the current state into the root of the tree.
	assert(m_stateMachine);

	if (m_stateMachine)
	{
		//TODO: Improve interface here? Should there be a common function on the state machine that throws exceptions, etc?
		assert(m_stateMachine->GetSaveStateHandle());

		if (m_stateMachine->GetSaveStateHandle())
		{
			Savestate savestate = std::move(m_stateMachine->GetSaveStateHandle()());

			TreeNode &treeRoot = m_stateMachine->GetTree().GetRoot();
			treeRoot.SetSavestate(std::move(savestate));
		}
	}
}
