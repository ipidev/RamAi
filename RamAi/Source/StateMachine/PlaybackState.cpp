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


#include "PlaybackState.h"

#include <cassert>


RamAi::PlaybackState::PlaybackState(StateMachine &stateMachine)
	: State(stateMachine)
{
	m_numberOfFramesExecuted = 0;
}

RamAi::PlaybackState::PlaybackState(PlaybackState &&other)
	: State(std::move(other))
{
	m_numberOfFramesExecuted = other.m_numberOfFramesExecuted;
	m_actionSequence = std::move(other.m_actionSequence);
}

RamAi::PlaybackState::~PlaybackState()
{
}

RamAi::PlaybackState &RamAi::PlaybackState::operator= (PlaybackState &&other)
{
	Move(std::move(other));

	m_numberOfFramesExecuted = other.m_numberOfFramesExecuted;
	m_actionSequence = std::move(other.m_actionSequence);

	return *this;
}

void RamAi::PlaybackState::OnStateEntered(const std::weak_ptr<State>& oldState, const Type oldStateType)
{
	State::OnStateEntered(oldState, oldStateType);

	m_numberOfFramesExecuted = 0;

	//Construct action sequence.
	m_actionSequence.clear();

	assert(m_stateMachine);

	if (m_stateMachine)
	{
		if (const TreeNode *bestNode = m_stateMachine->GetTree().GetBestScoringNode())
		{
			const TreeNode *childNode = bestNode;
			const TreeNode *parentNode = bestNode->GetParent();

			//Iterate up the tree and build the sequence of actions.
			while (parentNode && childNode)
			{
				assert(childNode);

				ButtonSet parentToChildAction;
				const bool gotAction = parentNode->GetActionLeadingToChild(*childNode, parentToChildAction);
				assert(gotAction);

				//The actions will be in last-to-first order, so push them to the front (which reverses them).
				m_actionSequence.push_front(parentToChildAction);

				//Progress up the tree.
				childNode = parentNode;
				parentNode = childNode->GetParent();
			}
		}
	}
}

RamAi::ButtonSet RamAi::PlaybackState::CalculateInput(const Ram &ram)
{
	ButtonSet returnValue;

	//Return the action at the current index. 
	const size_t currentActionSequenceIndex = GetCurrentActionSequenceIndex();

	if (currentActionSequenceIndex < m_actionSequence.size())
	{
		returnValue = m_actionSequence[currentActionSequenceIndex];
	}

	++m_numberOfFramesExecuted;
	return returnValue;
}

RamAi::StateMachine::State::Type RamAi::PlaybackState::GetDesiredStateType(const Ram &ram)
{
	//Go to the selection/expansion state once we've executed the entire sequence.
	const size_t currentActionSequenceIndex = GetCurrentActionSequenceIndex();

	return (currentActionSequenceIndex >= m_actionSequence.size()) ? Type::Expansion : Type::Playback;
}

void RamAi::PlaybackState::OnStateExited(const std::weak_ptr<State> &newState, const Type newStateType)
{
	State::OnStateExited(newState, newStateType);

	assert(m_stateMachine);

	if (m_stateMachine)
	{
		//Finish recording.
		assert(m_stateMachine->GetFinishRecordingHandle());

		if (m_stateMachine->GetFinishRecordingHandle())
		{
			m_stateMachine->GetFinishRecordingHandle()();
		}
	}
}

size_t RamAi::PlaybackState::GetCurrentActionSequenceIndex() const
{
	const uint32_t macroActionLength = AiSettings::GetData().macroActionLength;

	return (macroActionLength > 0) ? m_numberOfFramesExecuted / macroActionLength : m_numberOfFramesExecuted;
}
