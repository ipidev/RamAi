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

#include "StateMachine.h"

#include <cassert>

#include "InitialisationState.h"
#include "ExpansionState.h"
#include "SimulationState.h"


RamAi::StateMachine::State::State(StateMachine &stateMachine)
{
	m_stateMachine = &stateMachine;
}

RamAi::StateMachine::State::State(State &&other)
{
	Move(std::move(other));
}

RamAi::StateMachine::State::~State()
{
}

RamAi::StateMachine::State &RamAi::StateMachine::State::operator= (State &&other)
{
	Move(std::move(other));
	return *this;
}

void RamAi::StateMachine::State::OnStateEntered(const std::weak_ptr<State> &oldState, const Type oldStateType)
{
}

void RamAi::StateMachine::State::OnStateExited(const std::weak_ptr<State> &newState, const Type newStateType)
{
}

void RamAi::StateMachine::State::Move(State &&other)
{
	m_stateMachine = other.m_stateMachine;
	other.m_stateMachine = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

RamAi::StateMachine::StateMachine(const ScoreLog::SaveLogToFileSignature &saveLogToFileHandle)
	: m_tree()
	, m_currentStateType(State::Type::Initialisation)
	, m_scoreLog(GameSettings::GetInstance(), saveLogToFileHandle)
{
	InitialiseStates();
}

RamAi::StateMachine::~StateMachine()
{
}

RamAi::ButtonSet RamAi::StateMachine::CalculateInput(const Ram &ram)
{
	ButtonSet returnValue;

	{
		State *currentState = nullptr;
		State::Type desiredStateType = m_currentStateType;
		State::Type previousStateType = desiredStateType;

		//Get the current state. This is a bit complicated, as our current state may want to change.
		//In this case, we need to repeatedly change state until no more change is desired (or we change to a null state)!
		do
		{
			currentState = GetCurrentStateInternal().get();
			desiredStateType = currentState ? currentState->GetDesiredStateType(ram) : m_currentStateType;

			//Store the current state type before changing it.
			previousStateType = m_currentStateType;

			if (desiredStateType != m_currentStateType)
			{
				ChangeState(desiredStateType);
			}
		}
		while (currentState && desiredStateType != previousStateType);

		//Use the current state to calculate input.
		if (currentState)
		{
			returnValue = currentState->CalculateInput(ram);
		}
	}

	return returnValue;
}

void RamAi::StateMachine::UpdateScoreLog()
{
	m_scoreLog.UpdateLog(m_tree);
}

void RamAi::StateMachine::InitialiseStates()
{
	m_states[State::Type::Initialisation] = std::make_shared<InitialisationState>(*this);
	m_states[State::Type::Expansion] = std::make_shared<ExpansionState>(*this);
	m_states[State::Type::Simulation] = std::make_shared<SimulationState>(*this);
}

void RamAi::StateMachine::ChangeState(const State::Type newStateType)
{
	assert(newStateType != m_currentStateType);

	if (newStateType != m_currentStateType)
	{
		//Tell the old state that it's being left.
		if (std::shared_ptr<State> oldState = GetCurrentState().lock())
		{
			oldState->OnStateExited(GetState(newStateType), newStateType);
		}

		const State::Type oldStateType = m_currentStateType;
		m_currentStateType = newStateType;

		//Tell the new state that it's being entered.
		if (std::shared_ptr<State> newState = GetCurrentState().lock())
		{
			newState->OnStateEntered(GetState(oldStateType), oldStateType);
		}
	}
}