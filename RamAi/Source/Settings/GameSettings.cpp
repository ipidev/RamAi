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

#include "GameSettings.h"

#include <algorithm>
#include <cassert>
#include <ctime>

#include "Importers\BasicSettingsImporter.h"


RamAi::GameSettings::GameSettings()
{
	gameName = "None";
	time(&initialisationTime);
	assert(initialisationTime != -1);

	initialisationStartButtonFrames = 60;
	initialisationTotalFrames = 180;

	scoreOffset = 0;
	scoreSize = 0;
	scoreEndianness = BinaryCodedDecimal::Endianness::Big;
	scoreTwoDigitsPerByte = false;
	scoreUpperDigitInHighNibble = true;
}

void RamAi::GameSettings::Import(char *settingsFile)
{
	BasicSettingsImporter settingsImporter(settingsFile, "GameSettings");

	if (settingsImporter.ContainsKey("InitialisationStartButtonFrames"))
	{
		initialisationStartButtonFrames = static_cast<size_t>(std::stoi(settingsImporter["InitialisationStartButtonFrames"]));
	}

	if (settingsImporter.ContainsKey("InitialisationTotalFrames"))
	{
		initialisationTotalFrames = static_cast<size_t>(std::stoi(settingsImporter["InitialisationTotalFrames"]));
	}

	if (settingsImporter.ContainsKey("ScoreOffset"))
	{
		scoreOffset = static_cast<size_t>(std::stoi(settingsImporter["ScoreOffset"]));
	}

	if (settingsImporter.ContainsKey("ScoreSize"))
	{
		scoreSize = static_cast<size_t>(std::stoi(settingsImporter["ScoreSize"]));
	}

	if (settingsImporter.ContainsKey("ScoreEndianness"))
	{
		const std::string endiannessString = settingsImporter["ScoreEndianness"];

		scoreEndianness = (endiannessString == "Big") ? BinaryCodedDecimal::Endianness::Big : BinaryCodedDecimal::Endianness::Little;
	}

	if (settingsImporter.ContainsKey("ScoreTwoDigitsPerByte"))
	{
		const std::string twoDigitsPerByteString = settingsImporter["ScoreTwoDigitsPerByte"];

		scoreTwoDigitsPerByte = (twoDigitsPerByteString == "True");
	}

	if (settingsImporter.ContainsKey("ScoreUpperDigitInHighNibble"))
	{
		const std::string upperDigitInHighNibbleString = settingsImporter["ScoreUpperDigitInHighNibble"];

		scoreUpperDigitInHighNibble = (upperDigitInHighNibbleString == "True");
	}
}

size_t RamAi::GameSettings::GetMaximumInitialisationFrames() const
{
	return std::max(initialisationStartButtonFrames, initialisationTotalFrames);
}

uint32_t RamAi::GameSettings::GetMaximumScore() const
{
	return BinaryCodedDecimal::Power(10, static_cast<uint32_t>(scoreSize)) - 1;
}

RamAi::GameSettings RamAi::GameSettings::s_instance = GameSettings();