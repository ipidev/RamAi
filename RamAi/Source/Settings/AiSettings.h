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

#include <cstdint>


namespace RamAi
{
	//Static class that contains the console-independent settings for the agent.
	class AiSettings
	{
	public:
		//Plain-old data type that holds all of the neccessary parameters.
		struct Data
		{
		public:
			Data();
			~Data() = default;

			//The bias towards exploring the search tree versus exploiting known good areas.
			double explorationBias;

			//The base value for unexpanded, unexplored nodes during partial expansion.
			double partialExpansionBase;

			//The number of times to repeat the action taken during expansion. 
			uint32_t macroActionLength;

			//The number of times to repeat the actions taken during simulation. 
			uint32_t simulationMacroActionLength;

			//The maximum length of time to simulate for, in seconds.
			float maximumSimulationTime;

			//How often the score log is saved to disk.
			uint32_t scoreLogSaveFrequency;

		public:
			size_t GetMaximumSimulationFrames(const size_t frameRate) const;
		};

	public:
		AiSettings() = delete;
		~AiSettings() = delete;

	public:
		static const Data &GetData()			{ return s_data; }
		static void SetData(const Data &data)	{ s_data = data; }

		static Data Import(char *settingsFile);

	private:
		static Data s_data;
	};
};