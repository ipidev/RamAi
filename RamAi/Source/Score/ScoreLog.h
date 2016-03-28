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

#include <vector>

#include "MonteCarlo\GameMonteCarloTree.h"


namespace RamAi
{
	//A log of the best scores stored in the search tree over time.
	class ScoreLog
	{
	public:
		//A single item in the log.
		struct Item
		{
			uint32_t iterationNumber;
			double bestNodeScore;
			uint32_t bestNodeDepth;
		};

	public:
		ScoreLog();
		~ScoreLog();

	public:
		virtual void UpdateLog(const GameMonteCarloTree &tree);

	public:
		const std::vector<Item> &GetItems() const	{ return m_items; }

	protected:
		void AddItem(const TreeNode &bestNode);

	private:
		std::vector<Item> m_items;
		uint32_t m_currentIteration;
	};
};