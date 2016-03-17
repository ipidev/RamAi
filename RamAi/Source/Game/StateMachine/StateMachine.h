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

#include <functional>
#include <memory>

#include "Game/GameDetails.h"
#include "MonteCarlo/GameMonteCarloTree.h"
#include "State/Ram.h"
#include "State/Savestate.h"


namespace RamAi
{
	//A state machine that is ultimately responsible for making actions in the game.
	class StateMachine
	{
	public:
		//The base class for a single state within the state machine.
		class State
		{
		public:
			//Holds all possible types of state that exist.
			enum Type
			{
				Initialisation,
				Selection,
				Expansion,
				Simulation,
				Backpropagation,
				Max
			};

		public:
			State(StateMachine &stateMachine);
			State(const State &other) = delete;
			State(State &&other);
			virtual ~State();

		public:
			State &operator= (const State &other) = delete;
			State &operator= (State &&other);

		public:
			virtual void OnStateEntered(const std::weak_ptr<State> &oldState, const Type oldStateType);

			//Returns the relevant button press for the current frame of gameplay.
			virtual ButtonSet CalculateInput(const Ram &ram) = 0;

			//Returns the state that the state machine should now be in.
			//Returns its own state if no change is needed.
			virtual Type GetDesiredStateType(const Ram &ram) = 0;

			virtual void OnStateExited(const std::weak_ptr<State> &newState, const Type newStateType);

		protected:
			void Move(State &&other);

		protected:
			StateMachine *m_stateMachine;
		};

	public:
		StateMachine(const GameDetails &gameDetails);
		~StateMachine();

	public:
		const GameDetails &GetGameDetails() const	{ return m_gameDetails; }

		const GameMonteCarloTree &GetTree() const	{ return m_tree; }
		GameMonteCarloTree &GetTree()				{ return m_tree; }

		const std::weak_ptr<State> GetState(const State::Type state) const	{ return m_states[state]; }
		std::weak_ptr<State> GetState(const State::Type state)				{ return m_states[state]; }

		State::Type GetCurrentStateType() const								{ return m_currentStateType; }

		const std::weak_ptr<State> GetCurrentState() const					{ return m_states[m_currentStateType]; }
		std::weak_ptr<State> GetCurrentState()								{ return m_states[m_currentStateType]; }

	public:
		std::function<Savestate()> &GetSaveStateHandle()					{ return m_saveStateHandle; }
		std::function<void(const Savestate&)> &GetLoadStateHandle()			{ return m_loadStateHandle; }

	public:
		bool IsCurrentStateValid() const									{ return IsStateValid(m_currentStateType); }
		bool IsStateValid(const State::Type stateType) const				{ return m_states[stateType].get() != nullptr; }

	public:
		//The main interface with the state machine.
		ButtonSet CalculateInput(const Ram &ram);

	protected:
		void InitialiseStates();

		const std::shared_ptr<State> &GetCurrentStateInternal() const		{ return m_states[m_currentStateType]; }
		std::shared_ptr<State> &GetCurrentStateInternal()					{ return m_states[m_currentStateType]; }

		void ChangeState(const State::Type newStateType);

	protected:
		GameDetails m_gameDetails;
		GameMonteCarloTree m_tree;

		std::shared_ptr<State> m_states[State::Type::Max];
		State::Type m_currentStateType;

	protected:
		std::function<Savestate()> m_saveStateHandle;
		std::function<void(const Savestate &)> m_loadStateHandle;
	};
};