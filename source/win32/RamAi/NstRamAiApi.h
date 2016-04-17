////////////////////////////////////////////////////////////////////////////////////////
//
// Nestopia - NES/Famicom emulator written in C++
//
// Copyright (C) 2015 Sean Latham
//
// This file is part of Nestopia.
//
// Nestopia is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Nestopia is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Nestopia; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef NST_RAM_AI_API_H
#define NST_RAM_AI_API_H

#pragma once

#include "../../../RamAi/Source/Api.h"
#include "../NstManagerEmulator.hpp"
#include "../../core/NstCpu.hpp"

namespace Nes
{
	namespace Core
	{
		namespace Input
		{
			class Controllers;
		}
	}
}

namespace Nestopia
{
	class RamAiApi : public RamAi::Api
	{
	public:
		RamAiApi(Managers::Emulator &emulator);
		~RamAiApi();

	public:
		const bool AllowHumanOverride() const		{ return m_allowHumanOverride; }
		void SetAllowHumanOverride(const bool b)	{ m_allowHumanOverride = b; }

	public:
		//Initialises the game, passing in the right callbacks.
		void InitialiseGame(const RamAi::GameSettings &gameDetails);

		//Takes the emulator's RAM state and sets the relevant inputs.
		void CalculateInput(const Nes::byte *ramBytes, Nes::Core::Input::Controllers *const input);

	public:
		void ImportAiSettings();

		//This interface isn't great, but whatever.
		void ImportGameSettings(RamAi::GameSettings &gameSettings, const Path &path, const std::wstring &gameName);

	private:
		//ISavestateInteractable implementation.
		RamAi::Savestate SaveState();
		RamAi::Savestate BufferToSavestate(const Collection::Buffer &buffer);

		void LoadState(const RamAi::Savestate &savestate);
		Collection::Buffer SavestateToBuffer(const RamAi::Savestate &savestate);

		void SaveLogToFile(const RamAi::ScoreLog &scoreLog, const RamAi::MonteCarloTreeBase &tree);

		void StartRecording();
		void FinishRecording();

	private:
		void EnableTurbo(const bool turboOn);

		static RamAi::Ram NstRamToRamAiRam(const Nes::byte *ramBytes);

	private:
		Managers::Emulator &m_emulator;

		bool m_allowHumanOverride;

	private:
		//Container used to initialise the specs with the right values.
		class SpecsContainer
		{
		public:
			SpecsContainer();
			~SpecsContainer() = default;

		public:
			RamAi::ConsoleSettings::Specs specs;
		};

		static SpecsContainer s_specsContainer;

	private:
		static bool s_compressSavestates;

		static const std::wstring s_aiSettingsFileName;
		static const std::wstring s_settingsExtension;
		static const std::wstring s_scoreLogDirectory;
		static const std::wstring s_scoreLogExtension;
	};
};

#endif