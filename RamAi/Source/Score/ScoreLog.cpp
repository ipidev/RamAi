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

#include <cassert>
#include "Debug.h"


std::string RamAi::ScoreLog::Item::GetItemHeadings() const
{
	return "Iteration" + s_delimiter + "Best Score" + s_delimiter + "Depth" + s_lineTerminator;
}

std::string RamAi::ScoreLog::Item::GetItemValues() const
{
	return std::to_string(iterationNumber) + s_delimiter + std::to_string(bestNodeScore) + s_delimiter + std::to_string(bestNodeDepth) + s_lineTerminator;
}

const std::string RamAi::ScoreLog::Item::s_delimiter = "\t";
const std::string RamAi::ScoreLog::Item::s_lineTerminator = "\r\n";

////////////////////////////////////////////////////////////////////////////////

RamAi::ScoreLog::ScoreLog(const SaveLogToFileSignature &saveLogToFileHandle)
{
	m_saveLogToFileHandle = saveLogToFileHandle;

	m_items.reserve(100);
	m_currentIteration = 0;
}

RamAi::ScoreLog::~ScoreLog()
{
	SaveLogToFile();
}

void RamAi::ScoreLog::UpdateLog(const GameMonteCarloTree &tree)
{
	++m_currentIteration;

	//TODO: Selectively add the best scoring node so the log doesn't become too big?
	const TreeNode &bestNode = tree.GetBestScoringNode();

	AddItem(bestNode);

	//TODO: Only occasionally save to file.
	SaveLogToFile();
}

void RamAi::ScoreLog::AddItem(const TreeNode &bestNode)
{
	Item item;
	item.iterationNumber = m_currentIteration;
	item.bestNodeScore = bestNode.GetScore().GetAverageScore();
	item.bestNodeDepth = bestNode.CalculateDepth();

	m_items.push_back(std::move(item));
}

void RamAi::ScoreLog::SaveLogToFile()
{
	if (m_saveLogToFileHandle)
	{
		m_saveLogToFileHandle(*this);
	}
}