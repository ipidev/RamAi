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

#include "GameMonteCarloTree.h"

#include "Action/ButtonSet.h"


RamAi::GameMonteCarloTree::GameMonteCarloTree()
	: UctMonteCarloTree(0.5f)
{
}

RamAi::GameMonteCarloTree::~GameMonteCarloTree()
{
}

bool RamAi::GameMonteCarloTree::NodeNeedsExpanding(const TreeNode &node) const
{
	return MonteCarloTreeBase::NodeNeedsExpanding(node);
}

void RamAi::GameMonteCarloTree::PerformExpansion(TreeNode &nodeToBeExpanded)
{
	//TODO: Magic numbers that currently return left/right/A for Nestopia. Fix!!
	std::vector<ButtonSet> leftRightAButtonSet = ButtonSet::GetAllCombinations(Bitfield<uint32_t>(0), Bitfield<uint32_t>(193));

	for (size_t i = 0; i < leftRightAButtonSet.size(); ++i)
	{
		nodeToBeExpanded.AddChild(leftRightAButtonSet[i]);
	}
}