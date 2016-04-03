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

RamAi::ScoreLog::Item::Node::Node(const GameMonteCarloTree &tree, const TreeNode &node)
	: Node()
{
	uctScore = tree.CalculateUcbScore(node);
	averageScore = node.GetScore().GetAverageScore();
	depth = node.CalculateDepth();
}

std::string RamAi::ScoreLog::Item::Node::GetItemHeadings(const std::string &name, const std::string &delimiter) const
{
	return name + " UCT" + delimiter + name + " Average" + delimiter + name + " Depth";
}

std::string RamAi::ScoreLog::Item::Node::GetItemValues(const std::string &delimiter) const
{
	return std::to_string(uctScore) + delimiter + std::to_string(averageScore) + delimiter + std::to_string(depth);
}

////////////////////////////////////////////////////////////////////////////////

RamAi::ScoreLog::Item::Item()
{
	iterationNumber = 0;
}

std::string RamAi::ScoreLog::Item::GetItemHeadings() const
{
	return "Iteration" + s_delimiter + bestNode.GetItemHeadings("Best Node", s_delimiter) + s_delimiter + simulatedNode.GetItemHeadings("Simulated Node", s_delimiter);
}

std::string RamAi::ScoreLog::Item::GetItemHeadings(const MonteCarloTreeBase &tree) const
{
	return GetItemHeadings() + s_delimiter + tree.GetLogDetails() + s_lineTerminator;
}

std::string RamAi::ScoreLog::Item::GetItemValues() const
{
	return std::to_string(iterationNumber) + s_delimiter + bestNode.GetItemValues(s_delimiter) + s_delimiter + simulatedNode.GetItemValues(s_delimiter) + s_lineTerminator;
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
}

void RamAi::ScoreLog::UpdateLog(const GameMonteCarloTree &tree, const TreeNode &simulatedNode)
{
	++m_currentIteration;

	AddItem(tree, simulatedNode);

	if (ShouldSaveLogToFile(AiSettings::GetData()))
	{
		if (m_saveLogToFileHandle)
		{
			m_saveLogToFileHandle(*this, tree);
		}
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

void RamAi::ScoreLog::AddItem(const GameMonteCarloTree &tree, const TreeNode &simulatedNode)
{
	Item item;
	item.iterationNumber = m_currentIteration;

	if (const TreeNode *bestNode = tree.GetBestScoringNode())
	{
		item.bestNode = Item::Node(tree, *bestNode);
	}

	item.simulatedNode = Item::Node(tree, simulatedNode);

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