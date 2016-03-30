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

#include "Data/BinaryCodedDecimal.h"


namespace RamAi
{
	//A struct that holds information about the currently loaded game.
	struct GameSettings
	{
	public:
		GameSettings();
		~GameSettings() = default;

	public:
		std::string gameName;
		time_t initialisationTime;

	public:
		//The number of frames during initialisation in which to mash the start button.
		size_t initialisationStartButtonFrames;

		//The total number of frames to wait before creating the root state.
		size_t initialisationTotalFrames;

	public:
		//TODO: Should this be in its own struct?
		size_t scoreOffset;
		size_t scoreSize;
		BinaryCodedDecimal::Endianness scoreEndianness;

	public:
		void Import(char *settingsFile);

	public:
		bool IsValid() const				{ return HasValidScoreLocation(); }
		bool HasValidScoreLocation() const	{ return scoreOffset != 0 && scoreSize > 0; }

	public:
		size_t GetMaximumInitialisationFrames() const;

		//Returns the maximum score possible based off of the number of BCD digits.
		uint32_t GetMaximumScore() const;

	public:
		static const GameSettings &GetInstance()				{ return s_instance; }
		static void SetInstance(const GameSettings &instance)	{ s_instance = instance; }

	private:
		static GameSettings s_instance;
	};
};