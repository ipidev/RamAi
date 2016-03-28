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

#include <string>

#include "UctMonteCarloTree.h"


namespace RamAi
{
	//The game tree that is ultimately responsible for making decisions in the game.
	//This class contains all the domain-specific knowledge needed to build the search tree.
	class GameMonteCarloTree : public UctMonteCarloTree
	{
	public:
		GameMonteCarloTree();
		~GameMonteCarloTree();

	protected:
		virtual bool NodeNeedsExpanding(const TreeNode &node) const;

		virtual void PerformExpansion(TreeNode &nodeToBeExpanded) override;

		virtual TreeNode *SelectExpandedChild(const TreeNode &parent, const GameSettings &gameSettings) const override;
	};
};