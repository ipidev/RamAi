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

#include <cstdlib>

#include "BestScoreCollection.h"
#include "MonteCarloBase.h"


namespace RamAi
{
	//Interface for the score of a particular tree node.
	class UctScore : public MonteCarloScore
	{
	public:
		UctScore();
		~UctScore() = default;

	public:
		const float GetTotalScore() const	{ return m_totalScore; }
		const size_t GetVisits() const		{ return m_visits; }

	public:
		virtual void AddScore(const float score) override;
		virtual const float GetScore() const override;

	private:
		float m_totalScore;
		size_t m_visits;
	};

	//A Monte Carlo Tree Search (MCTS) implementation using Upper Confidence Bounds for Trees (UCT).
	template <typename StateType>
	class UctMonteCarlo : public MonteCarloBase<StateType, UctScore>
	{
	public:
		UctMonteCarlo(const StateType &initialState, const float bias)
			: MonteCarloBase(initialState)
		{
			m_bias = bias;
		}

		UctMonteCarlo(StateType &&initialState, const float bias)
			: MonteCarloBase(std::move(initialState))
		{
			m_bias = bias;
		}

		UctMonteCarlo(const UctMonteCarlo &other)
			: MonteCarloBase(other)
		{
			m_bias = other.m_bias;
		}

		UctMonteCarlo(UctMonteCarlo &&other)
			: MonteCarloBase(std::move(other))
		{
			m_bias = other.m_bias;
		}

		virtual ~UctMonteCarlo() override
		{
		}

	public:
		UctMonteCarlo &operator= (const UctMonteCarlo &other)
		{
			m_root = other.m_root;
			m_bias = other.m_bias;
			return *this;
		}

		UctMonteCarlo &operator= (UctMonteCarlo &&other)
		{
			m_root = std::move(other.m_root);
			m_bias = std::move(other.m_bias);
			return *this;
		}

	public:
		const float GetBias() const		{ return m_bias; }
		void SetBias(const float bias)	{ m_bias = bias; }

	protected:
		virtual TreeNode<NodeType> *SelectChild(TreeNode<NodeType> &root) override
		{
			if (root.IsLeaf())
			{
				return nullptr;
			}
			else
			{
				BestScoreCollection<TreeNode<NodeType>*, float> bestNodes(-std::numeric_limits<float>::infinity());

				for (size_t i = 0; i < root.GetNumberOfChildren(); ++i)
				{
					TreeNode<NodeType> &child = root.GetChild(i);

					//TODO: Check if the state is terminal, and skip add terminal states.
					const float ucbScore = CalculateUcbScore(root, child);
					bestNodes.Add(&child, ucbScore);
				}

				auto highestValueItem = bestNodes.GetItem();
				return highestValueItem ? *highestValueItem : nullptr;
			}
		}

		virtual const float CalculateUcbScore(const TreeNode<NodeType> &root, const TreeNode<NodeType> &child) const
		{
			assert(root.ContainsChild(child));

			const size_t rootVisits = root.GetValue().score.GetVisits();
			const size_t childVisits = child.GetValue().score.GetVisits();

			if (rootVisits > 0 && childVisits > 0)
			{
				const float rootVisitsFloat = static_cast<float>(rootVisits);
				const float childVisitsFloat = static_cast<float>(childVisits);

				const float visitsRadical = sqrt(log(rootVisitsFloat) / childVisitsFloat);
				const float childScoreMean = child.GetValue().score.GetTotalScore() / childVisitsFloat;

				const float ucb = childScoreMean + (m_bias * visitsRadical);
				return ucb;
			}
			else
			{
				return std::numeric_limits<float>::infinity();
			}
		}

	protected:


	protected:
		float m_bias;
	};
};