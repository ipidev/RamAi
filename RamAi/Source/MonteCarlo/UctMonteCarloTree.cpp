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

#include "UctMonteCarloTree.h"


RamAi::UctMonteCarloTree::UctMonteCarloTree(const double bias)
	: MonteCarloTreeBase()
{
	m_bias = bias;
}

RamAi::UctMonteCarloTree::UctMonteCarloTree(const UctMonteCarloTree &other)
	: MonteCarloTreeBase(other)
{
	Copy(other);
}

RamAi::UctMonteCarloTree::UctMonteCarloTree(UctMonteCarloTree &&other)
	: MonteCarloTreeBase(std::move(other))
{
	Move(std::move(other));
}

RamAi::UctMonteCarloTree::~UctMonteCarloTree()
{
}

RamAi::UctMonteCarloTree &RamAi::UctMonteCarloTree::operator= (const UctMonteCarloTree &other)
{
	Copy(other);
	return *this;
}

RamAi::UctMonteCarloTree &RamAi::UctMonteCarloTree::operator= (UctMonteCarloTree &&other)
{
	Move(std::move(other));
	return *this;
}

RamAi::TreeNode *RamAi::UctMonteCarloTree::SelectChild(const TreeNode &parent) const
{
	if (parent.IsLeaf())
	{
		return nullptr;
	}
	else
	{
		BestScoreCollection<const TreeNode*, double> bestNodes(-std::numeric_limits<double>::infinity());

		for (auto it = parent.GetIteratorBegin(); it != parent.GetIteratorEnd(); ++it)
		{
			const TreeNode &child = it->second;

			//TODO: Check if the state is terminal, and skip add terminal states.
			const double ucbScore = CalculateUcbScore(parent, child);
			bestNodes.Add(&child, ucbScore);
		}

		auto highestValueItem = bestNodes.GetItem();
		return highestValueItem ? const_cast<TreeNode*>(*highestValueItem) : nullptr; //TODO: FIX!!
	}
}

double RamAi::UctMonteCarloTree::CalculateUcbScore(const TreeNode &parent, const TreeNode &child) const
{
	const uint64_t rootVisits = parent.GetScore().GetVisits();
	const uint64_t childVisits = child.GetScore().GetVisits();

	if (rootVisits > 0 && childVisits > 0)
	{
		const double rootVisitsFloat = static_cast<double>(rootVisits);
		const double childVisitsFloat = static_cast<double>(childVisits);

		const double visitsRadical = sqrt((2.0 * log(rootVisitsFloat)) / childVisitsFloat);
		const double childScoreMean = child.GetScore().GetNormalisedScore(GameSettings::GetInstance());

		const double ucb = childScoreMean + (m_bias * visitsRadical);
		return ucb;
	}
	else
	{
		return std::numeric_limits<double>::infinity();
	}
}

void RamAi::UctMonteCarloTree::Copy(const UctMonteCarloTree &other)
{
	m_bias = other.m_bias;
}

void RamAi::UctMonteCarloTree::Move(UctMonteCarloTree &&other)
{
	m_bias = std::move(other.m_bias);
}