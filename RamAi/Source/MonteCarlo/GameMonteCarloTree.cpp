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

#include <cassert>

#include "Action/ButtonSet.h"
#include "Settings/ConsoleSettings.h"


RamAi::GameMonteCarloTree::GameMonteCarloTree()
	: UctMonteCarloTree(0.5f)
{
}

RamAi::GameMonteCarloTree::~GameMonteCarloTree()
{
}

bool RamAi::GameMonteCarloTree::NodeNeedsExpanding(const TreeNode &node) const
{
	//TODO: This needs to be changed to allow the tree to eventually expand into all allowed button combinations.
	return node.GetNumberOfChildren() < 2;
}

void RamAi::GameMonteCarloTree::PerformExpansion(TreeNode &nodeToBeExpanded)
{
	std::vector<ButtonSet> allInputCombinations = std::move(ConsoleSettings::GetSpecs().GetAllInputCombinations());

	//Select and add one child at random.
	while (allInputCombinations.size() > 0)
	{
		const size_t chosenIndex = rand() % allInputCombinations.size();

		//Add the action to the tree if it is unique.
		if (!nodeToBeExpanded.ContainsAction(allInputCombinations[chosenIndex]))
		{
			nodeToBeExpanded.AddChild(allInputCombinations[chosenIndex]);
			break;
		}
		//Otherwise, discard it.
		else
		{
			allInputCombinations.erase(allInputCombinations.begin() + chosenIndex);
		}
	}
}

RamAi::TreeNode *RamAi::GameMonteCarloTree::SelectExpandedChild(const TreeNode &parent, const GameSettings &gameSettings) const
{
	//We should only be expanding one child at a time, so we should return our newly expanded child here.
	//Thus, we just need to select the node that hasn't been given a score yet.
	for (auto it = parent.GetIteratorBegin(); it != parent.GetIteratorEnd(); ++it)
	{
		if (it->second.GetScore().GetVisits() == 0)
		{
			assert(!it->second.HasSavestate());

			//TODO: Fix constness here by making the base function non-const.
			return const_cast<TreeNode*>(&(it->second));
		}
	}

	//Didn't find it?
	assert(false);
	return nullptr;
}
