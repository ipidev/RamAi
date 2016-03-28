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


#include "SimulationState.h"

#include <cassert>

#include "Settings\AiSettings.h"
#include "Settings\ConsoleSettings.h"
#include "ExpansionState.h"


RamAi::SimulationState::SimulationState(StateMachine &stateMachine)
	: State(stateMachine)
{
	m_simulatedNode = nullptr;
	m_numberOfFramesExecuted = 0;
}

RamAi::SimulationState::SimulationState(SimulationState &&other)
	: State(std::move(other))
{
	m_numberOfFramesExecuted = other.m_numberOfFramesExecuted;
}

RamAi::SimulationState::~SimulationState()
{
}

RamAi::SimulationState &RamAi::SimulationState::operator= (SimulationState &&other)
{
	Move(std::move(other));

	m_numberOfFramesExecuted = other.m_numberOfFramesExecuted;

	return *this;
}

void RamAi::SimulationState::OnStateEntered(const std::weak_ptr<State>& oldState, const Type oldStateType)
{
	State::OnStateEntered(oldState, oldStateType);

	//Get the node we're currently on from the previous state.
	if (oldStateType == Type::Expansion)
	{
		if (State *oldStateRaw = oldState.lock().get())
		{
			ExpansionState *expansionState = static_cast<ExpansionState*>(oldStateRaw);
			assert(expansionState);

			if (expansionState)
			{
				assert(expansionState->GetExpandedNode());
				m_simulatedNode = expansionState->GetExpandedNode();
			}
		}
	}

	m_numberOfFramesExecuted = 0;
}

RamAi::ButtonSet RamAi::SimulationState::CalculateInput(const Ram &ram)
{
	ButtonSet returnValue;

	//Get random buttons. TODO: Don't use these magic numbers!
	std::vector<ButtonSet> buttonsToChooseFrom = ButtonSet::GetAllCombinations(Bitfield<uint32_t>(0), Bitfield<uint32_t>(193));
	
	if (buttonsToChooseFrom.size() > 0)
	{
		size_t chosenIndex = rand() % buttonsToChooseFrom.size();
		returnValue = buttonsToChooseFrom[chosenIndex];
	}

	++m_numberOfFramesExecuted;
	return returnValue;
}

RamAi::StateMachine::State::Type RamAi::SimulationState::GetDesiredStateType(const Ram &ram)
{
	const size_t frameRate = ConsoleSettings::GetSpecs().frameRate;
	const size_t targetNumberOfFrames = AiSettings::GetData().GetMaximumSimulationFrames(frameRate);

	return m_numberOfFramesExecuted >= targetNumberOfFrames ? Type::Expansion : Type::Simulation;
}

void RamAi::SimulationState::OnStateExited(const std::weak_ptr<State> &newState, const Type newStateType)
{
	State::OnStateExited(newState, newStateType);

	//Backpropagate the result back up the tree.
	assert(m_stateMachine);

	if (m_stateMachine)
	{
		assert(m_simulatedNode);

		if (m_simulatedNode)
		{
			GameMonteCarloTree &tree = m_stateMachine->GetTree();

			tree.Backpropagate(*m_simulatedNode, 0);
		}
	}
}
