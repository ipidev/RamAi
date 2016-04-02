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

#include <functional>
#include <string>
#include <vector>

#include "MonteCarlo\GameMonteCarloTree.h"
#include "Settings\AiSettings.h"
#include "Settings\GameSettings.h"


namespace RamAi
{
	//A log of the best scores stored in the search tree over time.
	class ScoreLog
	{
	public:
		//A single item in the log.
		struct Item
		{
		public:
			Item();
			~Item() = default;

		public:
			uint32_t iterationNumber;
			double bestNodeScore;
			uint32_t bestNodeDepth;

			double simulatedNodeScore;
			uint32_t simulatedNodeDepth;

		public:
			virtual std::string GetItemHeadings() const;
			virtual std::string GetItemValues() const;

		protected:
			static const std::string s_delimiter;
			static const std::string s_lineTerminator;
		};

	public:
		typedef std::function<void(const ScoreLog&)> SaveLogToFileSignature;

	public:
		ScoreLog(const GameSettings &gameSettings, const SaveLogToFileSignature &saveLogToFileHandle);
		~ScoreLog();

	public:
		virtual void UpdateLog(const GameMonteCarloTree &tree, const TreeNode &simulatedNode);

	public:
		const std::string &GetFileName() const		{ return m_fileName; }

		const std::vector<Item> &GetItems() const	{ return m_items; }
		uint32_t GetCurrentIteration() const		{ return m_currentIteration; }

	protected:
		virtual std::string ConstructFileName(const GameSettings &gameSettings) const;

		void AddItem(const TreeNode *bestNode, const TreeNode &simulatedNode);

		bool ShouldSaveLogToFile(const AiSettings::Data &aiSettings) const;
		void SaveLogToFile();

	private:
		std::string m_fileName;

		std::vector<Item> m_items;
		uint32_t m_currentIteration;

		SaveLogToFileSignature m_saveLogToFileHandle;
	};
};