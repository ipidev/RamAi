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
	//A state responsible for expanding the search tree and associating the newly expanded node with a savestate.
	//After this has completed, simulation can begin.
	class ExpansionState : public StateMachine::State
	{
	public:
		ExpansionState(StateMachine &stateMachine);
		ExpansionState(const ExpansionState &other) = delete;
		ExpansionState(ExpansionState &&other);
		~ExpansionState();

	public:
		ExpansionState &operator= (const ExpansionState &other) = delete;
		ExpansionState &operator= (ExpansionState &&other);

	public:
		virtual void OnStateEntered(const std::weak_ptr<State> &oldState, const Type oldStateType) override;

		virtual ButtonSet CalculateInput(const Ram &ram) override;

		virtual Type GetDesiredStateType(const Ram &ram) override;

		virtual void OnStateExited(const std::weak_ptr<State> &newState, const Type newStateType) override;

	public:
		TreeNode *GetExpandedNode()	{ return m_expandedNode; }

	protected:
		TreeNode *m_expandedNode;
		ButtonSet m_expansionAction;

		size_t m_actionsPerformed;
		static const size_t s_maximumNumberOfActions;
	};
};