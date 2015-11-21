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
		RamAiApi();
		~RamAiApi();

	public:
		void CalculateInput(Nes::Core::Input::Controllers *const input);
	};
};

#endif