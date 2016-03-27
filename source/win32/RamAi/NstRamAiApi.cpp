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

#include "NstRamAiApi.h"

#include "../../core/api/NstApiInput.hpp"
#include "../NstIoStream.hpp"
#include "../NstIoFile.hpp"
#include "../NstApplicationInstance.hpp"
#include "NstRamAiDebug.h"


Nestopia::RamAiApi::RamAiApi(Managers::Emulator &emulator)
	: RamAi::Api(s_specsContainer.specs, std::make_unique<RamAiDebug>())
	, m_emulator(emulator)
	, m_allowHumanOverride(true)
{
}

Nestopia::RamAiApi::~RamAiApi()
{
}

void Nestopia::RamAiApi::InitialiseGame(const RamAi::GameSettings &gameDetails)
{
	//Bind functions to this specific instance and pass them to the base class.
	RamAi::StateMachine::SaveStateHandleSignature saveStateHandle = std::bind(&RamAiApi::SaveState, this);

	//std::placeholders are used for extra parameters that are filled in later.
	RamAi::StateMachine::LoadStateHandleSignature loadStateHandle = std::bind(&RamAiApi::LoadState, this, std::placeholders::_1); 

	//Call the base.
	RamAi::Api::InitialiseGame(gameDetails, saveStateHandle, loadStateHandle);

	EnableTurbo(true);
}

void Nestopia::RamAiApi::CalculateInput(const Nes::byte *ramBytes, Nes::Core::Input::Controllers *const input)
{
	if (input)
	{
		//Get desired input from RamAi.
		RamAi::Ram ram = std::move(NstRamToRamAiRam(ramBytes));
		RamAi::ButtonSet buttonSet = RamAi::Api::CalculateInput(ram);

		//Set the bitfield on the controller inputs.
		RamAi::Bitfield<RamAi::ButtonSet::BitfieldType> buttonBitfield = buttonSet.GetBitfield();

		//Never press start or select for now!!
		//buttonBitfield.UnsetAll(Nes::Core::Input::Controllers::Pad::START | Nes::Core::Input::Controllers::Pad::SELECT);

		input->pad[0].buttons = buttonBitfield.GetValue();
	}
}

void Nestopia::RamAiApi::ImportAiSettings()
{
	//Here begins the string buggery.
	//STL wide string to Nestopia wide string.
	String::Generic<wchar_t> aiSettingsFileNameString(s_aiSettingsFileName.c_str(), s_aiSettingsFileName.length());

	//Nestopia file path to Nestopia wide string.
	Path aiSettingsPath = Application::Instance::GetExePath(aiSettingsFileNameString);
	String::Generic<wchar_t> aiSettingsPathString(aiSettingsPath.Ptr(), aiSettingsPath.Length());

	try
	{
		//Open the file.
		Io::File file(aiSettingsPathString, Io::File::READ | Io::File::EXISTING);

		//Copy the bytes from the file into a buffer, then add a null-terminator on the end...
		static const size_t fileDataSize = 2048;
		char fileData[fileDataSize];

		size_t numberOfCharsRead = file.ReadSome(fileData, fileDataSize);
		fileData[numberOfCharsRead] = '\0';

		//Finally import it!
		Api::ImportAiSettings(fileData);
	}
	catch (...)
	{
		RamAi::Debug::OutLine("Couldn't import AI settings.", RamAi::Colour::Red);
	}
}

void Nestopia::RamAiApi::ImportGameSettings(RamAi::GameSettings &gameSettings, const Path &path, const std::wstring &gameName)
{
	std::wstring gamePathStlString(path.Ptr(), path.Length());
	std::wstring fullStlString = gamePathStlString + gameName + s_settingsExtension;
	String::Generic<wchar_t> fullNstString(fullStlString.c_str(), fullStlString.length());

	try
	{
		//Open the file.
		Io::File file(fullNstString, Io::File::READ | Io::File::EXISTING);

		//Copy the bytes from the file into a buffer, then add a null-terminator on the end...
		static const size_t fileDataSize = 2048;
		char fileData[fileDataSize];

		size_t numberOfCharsRead = file.ReadSome(fileData, fileDataSize);
		fileData[numberOfCharsRead] = '\0';

		//Finally import it!
		gameSettings.Import(fileData);
	}
	catch (...)
	{
	}
}

RamAi::Savestate Nestopia::RamAiApi::SaveState()
{
	//Most of this is derived from Nestopia::Managers::Emulator::SaveState().
	Collection::Buffer buffer;
	Io::Stream::Out stream(buffer);

	Nes::Result result = Nes::Machine(m_emulator).SaveState(stream, s_compressSavestates ? Nes::Machine::USE_COMPRESSION : Nes::Machine::NO_COMPRESSION);

	if (NES_SUCCEEDED(result))
	{
		return std::move(BufferToSavestate(buffer));
	}
	else
	{
		return RamAi::Savestate();
	}
}

RamAi::Savestate Nestopia::RamAiApi::BufferToSavestate(const Collection::Buffer &buffer)
{
	//Get byte array from buffer.
	const uint8_t *bytePtr = reinterpret_cast<const uint8_t*>(buffer.Ptr());
	const size_t numberOfBytes = buffer.Size();

	return RamAi::Savestate(bytePtr, numberOfBytes);
}

void Nestopia::RamAiApi::LoadState(const RamAi::Savestate &savestate)
{
	//Most of this is derived from Nestopia::Managers::Emulator::LoadState().
	Collection::Buffer buffer = std::move(SavestateToBuffer(savestate));
	Io::Stream::In stream(buffer);

	Nes::Result result = Nes::Machine(m_emulator).LoadState(stream);

	assert(NES_SUCCEEDED(result));
}

Nestopia::Collection::Buffer Nestopia::RamAiApi::SavestateToBuffer(const RamAi::Savestate &savestate)
{
	const char *charPtr = reinterpret_cast<const char*>(savestate.GetData().get());
	Collection::Buffer buffer(charPtr, savestate.GetSize());

	return buffer;
}

void Nestopia::RamAiApi::EnableTurbo(const bool turboOn)
{
	if (turboOn)
	{
		m_emulator.ToggleSpeed(true);
	}
	else
	{
		m_emulator.ToggleSpeed(false);
	}
}

RamAi::Ram Nestopia::RamAiApi::NstRamToRamAiRam(const Nes::byte *ramBytes)
{
	return RamAi::Ram(ramBytes, Nes::Core::Cpu::RAM_SIZE);
}

Nestopia::RamAiApi::SpecsContainer::SpecsContainer()
{
	specs.ramSize = Nes::Core::Cpu::RAM_SIZE;
	specs.numberOfGamePadButtons = 8;
	specs.initialisationButtonSet = RamAi::ButtonSet(Nes::Core::Input::Controllers::Pad::START);
}

Nestopia::RamAiApi::SpecsContainer Nestopia::RamAiApi::s_specsContainer = Nestopia::RamAiApi::SpecsContainer();

bool Nestopia::RamAiApi::s_compressSavestates = false;

const std::wstring Nestopia::RamAiApi::s_aiSettingsFileName = L"aiSettings.xml";

const std::wstring Nestopia::RamAiApi::s_settingsExtension = L".xml";