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


#include "ExpansionState.h"

#include <cassert>


RamAi::ExpansionState::ExpansionState(StateMachine &stateMachine)
	: State(stateMachine)
{
	m_expandedNode = nullptr;
	m_actionsPerformed = 0;
}

RamAi::ExpansionState::ExpansionState(ExpansionState &&other)
	: State(std::move(other))
{
	m_expandedNode = other.m_expandedNode;
	m_expansionAction = std::move(other.m_expansionAction);
	m_actionsPerformed = other.m_actionsPerformed;
}

RamAi::ExpansionState::~ExpansionState()
{
}

RamAi::ExpansionState &RamAi::ExpansionState::operator= (ExpansionState &&other)
{
	Move(std::move(other));

	m_expandedNode = other.m_expandedNode;
	m_expansionAction = std::move(other.m_expansionAction);
	m_actionsPerformed = other.m_actionsPerformed;

	return *this;
}

void RamAi::ExpansionState::OnStateEntered(const std::weak_ptr<State>& oldState, const Type oldStateType)
{
	State::OnStateEntered(oldState, oldStateType);

	m_expandedNode = nullptr;
	m_expansionAction = ButtonSet();
	m_actionsPerformed = 0;

	assert(m_stateMachine);

	if (m_stateMachine)
	{
		//Select the most urgent node from the tree and load its state.
		GameMonteCarloTree &tree = m_stateMachine->GetTree();
		
		TreeNode &selectedNode = tree.Select(m_stateMachine->GetGameSettings());
		assert(selectedNode.HasSavestate());

		if (selectedNode.HasSavestate())
		{
			assert(m_stateMachine->GetLoadStateHandle());

			if (m_stateMachine->GetLoadStateHandle())
			{
				m_stateMachine->GetLoadStateHandle()(*selectedNode.GetSavestate());
			}

			//Expand the node and store it.
			m_expandedNode = &tree.Expand(selectedNode, m_stateMachine->GetGameSettings());
			assert(m_expandedNode);

			//Store the action(s) that are needed to reach the newly expanded state.
			if (TreeNode *parent = m_expandedNode->GetParent())
			{
				bool gotAction = parent->GetActionLeadingToChild(*m_expandedNode, m_expansionAction);
				assert(gotAction);
			}
		}
	}
}

RamAi::StateMachine::State::Type RamAi::ExpansionState::GetDesiredStateType(const Ram &ram)
{
	Type desiredStateType = Type::Expansion;

	if (m_expandedNode)
	{
		//If the maximum number of actions needed has been reached, save the current state.
		if (m_actionsPerformed >= s_maximumNumberOfActions)
		{
			assert(m_stateMachine);

			if (m_stateMachine)
			{
				assert(m_stateMachine->GetSaveStateHandle());

				if (m_stateMachine->GetSaveStateHandle())
				{
					Savestate savestate = m_stateMachine->GetSaveStateHandle()();
					m_expandedNode->SetSavestate(std::move(savestate));
				}
			}
		}

		//If the expanded node now has a savestate, we are ready to simulate from this position.
		//This may occur on the first execution if the selected node was a leaf.
		if (m_expandedNode->HasSavestate())
		{
			desiredStateType = Type::Simulation;
		}
	}

	return desiredStateType;
}

RamAi::ButtonSet RamAi::ExpansionState::CalculateInput(const Ram &ram)
{
	//Increment the number of inputs returned.
	++m_actionsPerformed;

	return m_expansionAction;
}

void RamAi::ExpansionState::OnStateExited(const std::weak_ptr<State> &newState, const Type newStateType)
{
	State::OnStateExited(newState, newStateType);

	//Check the node has a savestate before exiting.
	//TODO: Load it just in case?
	assert(m_expandedNode);

	if (m_expandedNode)
	{
		assert(m_expandedNode->HasSavestate());
	}
}

const size_t RamAi::ExpansionState::s_maximumNumberOfActions = 1;