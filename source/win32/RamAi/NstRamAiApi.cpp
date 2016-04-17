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
#include "../../core/api/NstApiMovie.hpp"
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

	RamAi::ScoreLog::SaveLogToFileSignature saveLogToFileHandle = std::bind(&RamAiApi::SaveLogToFile, this, std::placeholders::_1, std::placeholders::_2); 

	RamAi::StateMachine::StartRecordingHandleSignature startRecordingHandle = std::bind(&RamAiApi::StartRecording, this, std::placeholders::_1);
	RamAi::StateMachine::FinishRecordingHandleSignature finishRecordingHandle = std::bind(&RamAiApi::FinishRecording, this);

	//Call the base.
	RamAi::Api::InitialiseGame(gameDetails, saveStateHandle, loadStateHandle, saveLogToFileHandle, startRecordingHandle, finishRecordingHandle);

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

void Nestopia::RamAiApi::SaveLogToFile(const RamAi::ScoreLog &scoreLog, const RamAi::MonteCarloTreeBase &tree)
{
	//Create the directory to store logs first.
	{
		String::Generic<wchar_t> scoreLogDirectory(s_scoreLogDirectory.c_str(), s_scoreLogDirectory.length());
		Path scoreLogDirectoryPath = Application::Instance::GetExePath(scoreLogDirectory);

		BOOL createdDirectory = ::CreateDirectory(scoreLogDirectoryPath.Ptr(), NULL);
		
		//It will fail if the directory already exists, but that's okay. Assert on any other error.
		assert(createdDirectory || ::GetLastError() == ERROR_ALREADY_EXISTS);
	}

	//More string junk!
	std::wstring scoreLogFileNameWide = s_scoreLogDirectory;
	scoreLogFileNameWide += std::wstring(scoreLog.GetFileName().begin(), scoreLog.GetFileName().end()); //STL string to STL wide string.
	scoreLogFileNameWide += s_scoreLogExtension;

	//STL wide string to Nestopia wide string.
	String::Generic<wchar_t> scoreLogFileName(scoreLogFileNameWide.c_str(), scoreLogFileNameWide.length());

	//Nestopia file path to Nestopia wide string.
	Path scoreLogPath = Application::Instance::GetExePath(scoreLogFileName);
	String::Generic<wchar_t> scoreLogPathString(scoreLogPath.Ptr(), scoreLogPath.Length());

	try
	{
		//Open the file. We empty any existing file; this might be dangerous if it stops midway!
		Io::File file(scoreLogPathString, Io::File::WRITE | Io::File::EMPTY);

		static const size_t rowSize = 256;
		char rowData[rowSize];

		bool hasWrittenHeader = false;

		for (auto it = scoreLog.GetItems().cbegin(); it != scoreLog.GetItems().cend(); ++it)
		{
			//Write the header.
			if (!hasWrittenHeader)
			{
				const std::string itemHeadings = std::move(it->GetItemHeadings(tree));
				strcpy_s(rowData, rowSize, itemHeadings.c_str());

				file.WriteSome(rowData, itemHeadings.length());

				hasWrittenHeader = true;
			}

			//Write the item values.
			const std::string itemValues = std::move(it->GetItemValues());
			strcpy_s(rowData, rowSize, itemValues.c_str());

			file.WriteSome(rowData, itemValues.length());
		}
	}
	catch (...)
	{
		RamAi::Debug::OutLine("Error saving log file for iteration " + std::to_string(scoreLog.GetCurrentIteration()), RamAi::Colour::Red);
	}
}

void Nestopia::RamAiApi::StartRecording(const RamAi::ScoreLog &scoreLog)
{
	//Reset the emulator.
	Nes::Result result = Nes::Machine(m_emulator).Reset(false);
	assert(NES_SUCCEEDED(result));

	//Create the directory to store movie files first.
	{
		String::Generic<wchar_t> movieFileDirectory(s_movieFileDirectory.c_str(), s_movieFileDirectory.length());
		Path movieFileDirectoryPath = Application::Instance::GetExePath(movieFileDirectory);

		BOOL createdDirectory = ::CreateDirectory(movieFileDirectoryPath.Ptr(), NULL);

		//It will fail if the directory already exists, but that's okay. Assert on any other error.
		assert(createdDirectory || ::GetLastError() == ERROR_ALREADY_EXISTS);
	}

	//Get the path to save the file to.
	std::string movieFileNameShort = scoreLog.GetFileName() + " it" + std::to_string(scoreLog.GetCurrentIteration());

	std::wstring movieFileNameWide = s_movieFileDirectory;
	movieFileNameWide += std::wstring(movieFileNameShort.begin(), movieFileNameShort.end()); //STL string to STL wide string.
	movieFileNameWide += s_movieFileExtension;

	//STL wide string to Nestopia wide string.
	String::Generic<wchar_t> movieFileName(movieFileNameWide.c_str(), movieFileNameWide.length());

	//Nestopia file path to Nestopia wide string.
	Path movieFilePath = Application::Instance::GetExePath(movieFileName);
	String::Generic<wchar_t> movieFilePathString(movieFilePath.Ptr(), movieFilePath.Length());

	try
	{
		m_movieFile = std::make_unique<Io::File>(movieFilePathString, Io::File::WRITE | Io::File::EMPTY);
		m_movieFileStream = std::make_unique<Io::Stream::InOut>(*m_movieFile);

		//Start recording.
		Nes::Movie(m_emulator).Record(*m_movieFileStream, Nes::Movie::CLEAN);
	}
	catch (...)
	{
		RamAi::Debug::OutLine("Error in starting to record movie.", RamAi::Colour::Red);
		m_movieFile.reset();
		m_movieFileStream.reset();
	}
}

void Nestopia::RamAiApi::FinishRecording()
{
	//Finish recording.
	if (m_movieFile)
	{
		Nes::Movie(m_emulator).Stop();

		m_movieFileStream.reset();

		m_movieFile->Close();
		m_movieFile.reset();
	}
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
	using Nes::Core::Input::Controllers;

	specs.frameRate = 60;
	specs.initialisationButtonSet = RamAi::ButtonSet(Controllers::Pad::START);

	specs.directionalPadFields[RamAi::DirectionalPad::Up] = RamAi::ButtonSet(Controllers::Pad::UP);
	specs.directionalPadFields[RamAi::DirectionalPad::Down] = RamAi::ButtonSet(Controllers::Pad::DOWN);
	specs.directionalPadFields[RamAi::DirectionalPad::Left] = RamAi::ButtonSet(Controllers::Pad::LEFT);
	specs.directionalPadFields[RamAi::DirectionalPad::Right] = RamAi::ButtonSet(Controllers::Pad::RIGHT);

	specs.buttonsField = RamAi::ButtonSet(Controllers::Pad::A | Controllers::Pad::B);
}

Nestopia::RamAiApi::SpecsContainer Nestopia::RamAiApi::s_specsContainer = Nestopia::RamAiApi::SpecsContainer();

bool Nestopia::RamAiApi::s_compressSavestates = false;

const std::wstring Nestopia::RamAiApi::s_aiSettingsFileName = L"aiSettings.xml";
const std::wstring Nestopia::RamAiApi::s_settingsExtension = L".xml";
const std::wstring Nestopia::RamAiApi::s_scoreLogDirectory = L"RamAiLogs\\";
const std::wstring Nestopia::RamAiApi::s_scoreLogExtension = L".csv";
const std::wstring Nestopia::RamAiApi::s_movieFileDirectory = L"RamAiMovies\\";
const std::wstring Nestopia::RamAiApi::s_movieFileExtension = L".nsv";