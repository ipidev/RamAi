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


Nestopia::RamAiApi::RamAiApi(Managers::Emulator &emulator)
	: RamAi::Api(s_specsContainer.specs)
	, m_emulator(emulator)
	, m_allowHumanOverride(true)
{
}

Nestopia::RamAiApi::~RamAiApi()
{
}

void Nestopia::RamAiApi::CalculateInput(Nes::Core::Input::Controllers *const input)
{
	if (input)
	{
		//Get random input for now.
		RamAi::ButtonSet randomButtons = RamAi::Api::CalculateInput();
		RamAi::Bitfield<RamAi::ButtonSet::BitfieldType> buttonBitfield = randomButtons.GetBitfield();

		//Never press start or select for now!!
		//buttonBitfield.UnsetAll(Nes::Core::Input::Controllers::Pad::START | Nes::Core::Input::Controllers::Pad::SELECT);

		input->pad[0].buttons = buttonBitfield.GetValue();
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
}

Nestopia::RamAiApi::SpecsContainer::SpecsContainer()
{
	specs.ramSize = 2048;
	specs.numberOfGamePadButtons = 8;
	specs.initialisationButtonSet = RamAi::ButtonSet(Nes::Core::Input::Controllers::Pad::START);
}

Nestopia::RamAiApi::SpecsContainer Nestopia::RamAiApi::s_specsContainer = Nestopia::RamAiApi::SpecsContainer();

bool Nestopia::RamAiApi::s_compressSavestates = false;