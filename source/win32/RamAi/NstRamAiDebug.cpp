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

#include "NstRamAiDebug.h"


Nestopia::RamAiDebug::RamAiDebug()
	: m_outputHandle(INVALID_HANDLE_VALUE)
{
	if (AllocConsole())
	{
		m_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		if (m_outputHandle != INVALID_HANDLE_VALUE)
		{
			//Resize the console window.
			CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

			if (GetConsoleScreenBufferInfo(m_outputHandle, &screenBufferInfo))
			{
				screenBufferInfo.dwSize.Y = s_consoleHeight;

				SetConsoleScreenBufferSize(m_outputHandle, screenBufferInfo.dwSize);
			}

			//Rename the console window.
			SetConsoleTitleA("RamAi Debug");
		}
	}
}

Nestopia::RamAiDebug::~RamAiDebug()
{
	if (m_outputHandle != INVALID_HANDLE_VALUE)
	{
		FreeConsole();
		m_outputHandle = INVALID_HANDLE_VALUE;
	}
}

void Nestopia::RamAiDebug::InstanceOut(const std::string &string)
{
	if (m_outputHandle != INVALID_HANDLE_VALUE)
	{
		WriteConsoleA(m_outputHandle, string.c_str(), string.length(), nullptr, NULL);
	}
}
