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

#include "AiSettings.h"

#include "Importers\BasicSettingsImporter.h"


RamAi::AiSettings::Data::Data()
{
	explorationBias = 0.5f;
	macroActionLength = 1;
	simulationMacroActionLength = 1;
	maximumSimulationTime = 120.0f;
	scoreLogSaveFrequency = 10;
}

size_t RamAi::AiSettings::Data::GetMaximumSimulationFrames(const size_t frameRate) const
{
	return static_cast<size_t>(static_cast<float>(frameRate) * maximumSimulationTime);
}

RamAi::AiSettings::Data RamAi::AiSettings::Import(char *settingsFile)
{
	Data data;
	BasicSettingsImporter settingsImporter(settingsFile, "AiSettings");

	if (settingsImporter.ContainsKey("ExplorationBias"))
	{
		data.explorationBias = std::stod(settingsImporter["ExplorationBias"]);
	}

	if (settingsImporter.ContainsKey("MacroActionLength"))
	{
		data.macroActionLength = static_cast<uint32_t>(std::stoi(settingsImporter["MacroActionLength"]));
	}

	if (settingsImporter.ContainsKey("SimulationMacroActionLength"))
	{
		data.simulationMacroActionLength = static_cast<uint32_t>(std::stoi(settingsImporter["SimulationMacroActionLength"]));
	}

	if (settingsImporter.ContainsKey("MaximumSimulationTime"))
	{
		data.maximumSimulationTime = std::stof(settingsImporter["MaximumSimulationTime"]);
	}

	if (settingsImporter.ContainsKey("ScoreLogSaveFrequency"))
	{
		data.scoreLogSaveFrequency = static_cast<uint32_t>(std::stoi(settingsImporter["ScoreLogSaveFrequency"]));
	}

	return data;
}


RamAi::AiSettings::Data RamAi::AiSettings::s_data = Data();