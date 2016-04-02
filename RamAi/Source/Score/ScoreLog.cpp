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
#include <ctime>


RamAi::ScoreLog::Item::Node::Node()
{
	uctScore = 0.0;
	averageScore = 0.0;
	depth = 0;
}

RamAi::ScoreLog::Item::Node::Node(const TreeNode &node)
	: Node()
{
	//TODO: UCT score
	averageScore = node.GetScore().GetAverageScore();
	depth = node.CalculateDepth();
}

////////////////////////////////////////////////////////////////////////////////

RamAi::ScoreLog::Item::Item()
{
	iterationNumber = 0;
}

std::string RamAi::ScoreLog::Item::GetItemHeadings() const
{
	return "Iteration" + s_delimiter + "Best Node Score" + s_delimiter + "Best Node Depth" + s_delimiter + "Simulated Node Score" + s_delimiter + "Simulated Node Score" + s_lineTerminator;
}

std::string RamAi::ScoreLog::Item::GetItemValues() const
{
	return std::to_string(iterationNumber) + s_delimiter + std::to_string(bestNode.averageScore) + s_delimiter + std::to_string(bestNode.depth) + s_delimiter + std::to_string(simulatedNode.averageScore) + s_delimiter + std::to_string(simulatedNode.depth) +s_lineTerminator;
}

const std::string RamAi::ScoreLog::Item::s_delimiter = "\t";
const std::string RamAi::ScoreLog::Item::s_lineTerminator = "\r\n";

////////////////////////////////////////////////////////////////////////////////

RamAi::ScoreLog::ScoreLog(const GameSettings &gameSettings, const SaveLogToFileSignature &saveLogToFileHandle)
{
	m_fileName = std::move(ConstructFileName(gameSettings));

	m_saveLogToFileHandle = saveLogToFileHandle;

	m_items.reserve(100);
	m_currentIteration = 0;
}

RamAi::ScoreLog::~ScoreLog()
{
	SaveLogToFile();
}

void RamAi::ScoreLog::UpdateLog(const GameMonteCarloTree &tree, const TreeNode &simulatedNode)
{
	++m_currentIteration;

	AddItem(tree.GetBestScoringNode(), simulatedNode);

	if (ShouldSaveLogToFile(AiSettings::GetData()))
	{
		SaveLogToFile();
	}
}

std::string RamAi::ScoreLog::ConstructFileName(const GameSettings &gameSettings) const
{
	//Convert the timestamp to a string.
	static const size_t timestampBufferSize = 30;
	char timestampBuffer[timestampBufferSize];
	tm timestamp;
	localtime_s(&timestamp, &gameSettings.initialisationTime);
	strftime(timestampBuffer, timestampBufferSize, "%Y-%m-%d %H.%M.%S ", &timestamp);

	return timestampBuffer + gameSettings.gameName;
}

void RamAi::ScoreLog::AddItem(const TreeNode *bestNode, const TreeNode &simulatedNode)
{
	Item item;
	item.iterationNumber = m_currentIteration;

	if (bestNode)
	{
		item.bestNode = Item::Node(*bestNode);
	}

	item.simulatedNode = Item::Node(simulatedNode);

	m_items.push_back(std::move(item));
}

bool RamAi::ScoreLog::ShouldSaveLogToFile(const AiSettings::Data &aiSettings) const
{
	if (aiSettings.scoreLogSaveFrequency > 0)
	{
		return ((m_currentIteration - 1) % aiSettings.scoreLogSaveFrequency) == 0;
	}
	else
	{
		return false;
	}
}

void RamAi::ScoreLog::SaveLogToFile()
{
	if (m_saveLogToFileHandle)
	{
		m_saveLogToFileHandle(*this);
	}
}