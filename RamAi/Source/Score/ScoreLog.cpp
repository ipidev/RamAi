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

#include "ScoreLog.h"


RamAi::ScoreLog::ScoreLog()
{
	m_items.reserve(100);
	m_currentIteration = 0;
}

RamAi::ScoreLog::~ScoreLog()
{
}

void RamAi::ScoreLog::UpdateLog(const GameMonteCarloTree &tree)
{
	++m_currentIteration;

	//TODO: Selectively add the best scoring node so the log doesn't become too big?
	const TreeNode &bestNode = tree.GetBestScoringNode();

	AddItem(bestNode);
}

void RamAi::ScoreLog::AddItem(const TreeNode &bestNode)
{
	Item item;
	item.iterationNumber = m_currentIteration;
	item.bestNodeScore = bestNode.GetScore().GetAverageScore();
	item.bestNodeDepth = bestNode.CalculateDepth();

	m_items.push_back(std::move(item));
}
