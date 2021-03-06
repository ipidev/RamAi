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
#include "Settings\GameSettings.h"
#include "ExpansionState.h"


RamAi::SimulationState::SimulationState(StateMachine &stateMachine)
	: State(stateMachine)
{
	m_simulatedNode = nullptr;
	m_numberOfFramesExecuted = 0;

	m_currentScore = 0;
}

RamAi::SimulationState::SimulationState(SimulationState &&other)
	: State(std::move(other))
{
	m_numberOfFramesExecuted = other.m_numberOfFramesExecuted;
	m_currentScore = other.m_currentScore;
}

RamAi::SimulationState::~SimulationState()
{
}

RamAi::SimulationState &RamAi::SimulationState::operator= (SimulationState &&other)
{
	Move(std::move(other));

	m_numberOfFramesExecuted = other.m_numberOfFramesExecuted;
	m_currentScore = other.m_currentScore;

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
	m_currentScore = 0;

	m_currentMacroAction.GetBitfield().Clear();
}

RamAi::ButtonSet RamAi::SimulationState::CalculateInput(const Ram &ram)
{
	const uint32_t simulationMacroActionLength = AiSettings::GetData().simulationMacroActionLength;

	//Get a new macro action if the macro action length has been reached.
	if (simulationMacroActionLength == 0 || (m_numberOfFramesExecuted % simulationMacroActionLength) == 0)
	{
		m_currentMacroAction = ConsoleSettings::GetSpecs().GenerateRandomInput();
	}

	++m_numberOfFramesExecuted;

	return m_currentMacroAction;
}

RamAi::StateMachine::State::Type RamAi::SimulationState::GetDesiredStateType(const Ram &ram)
{
	UpdateCurrentScore(ram);

	//Calculate hard cut-off time.
	const size_t frameRate = ConsoleSettings::GetSpecs().frameRate;
	const size_t targetNumberOfFrames = AiSettings::GetData().GetMaximumSimulationFrames(frameRate);

	//Check if we need to record a playback movie.
	bool needsToRecordPlaybackMovie = false;

	if (m_stateMachine)
	{
		const uint32_t currentIteration = m_stateMachine->GetScoreLog().GetCurrentIteration();
		const uint32_t movieFileSaveFrequency = AiSettings::GetData().movieFileSaveFrequency;
		needsToRecordPlaybackMovie = currentIteration > 0 && (currentIteration % movieFileSaveFrequency) == 0;
	}

	const Type nextStateType = needsToRecordPlaybackMovie ? Type::Initialisation : Type::Expansion;
	return m_numberOfFramesExecuted >= targetNumberOfFrames ? nextStateType : Type::Simulation;
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

			tree.Backpropagate(*m_simulatedNode, m_currentScore);
		}

		//Update the log.
		m_stateMachine->UpdateScoreLog(*m_simulatedNode);
	}
}

void RamAi::SimulationState::UpdateCurrentScore(const Ram &ram)
{
	assert(m_stateMachine);

	if (m_stateMachine)
	{
		m_currentScore = ram.GetCurrentScore(GameSettings::GetInstance());
	}
}
