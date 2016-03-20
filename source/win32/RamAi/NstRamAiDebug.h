////////////////////////////////////////////////////////////////////////////////////////
//
// Nestopia - NES/Famicom emulator written in C++
//
// Copyright (C) 2016 Sean Latham
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

#ifndef NST_RAM_AI_DEBUG_H
#define NST_RAM_AI_DEBUG_H

#pragma once

#include "../../../RamAi/Source/Debug.h"

#include <Windows.h>


namespace Nestopia
{
	class RamAiDebug : public RamAi::Debug
	{
	public:
		RamAiDebug();
		~RamAiDebug();

	public:
		virtual void InstanceOut(const std::string &string, const RamAi::Colour colour) override;
		virtual void InstanceClearScreen() override;

	protected:
		WORD RamAiColourToWord(const RamAi::Colour colour) const;

	private:
		HANDLE m_outputHandle;

		static const short s_consoleHeight = 500;
	};
};

#endif