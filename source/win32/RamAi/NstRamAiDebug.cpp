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

void Nestopia::RamAiDebug::InstanceOut(const std::string &string, const RamAi::Colour colour)
{
	if (m_outputHandle != INVALID_HANDLE_VALUE)
	{
		SetConsoleTextAttribute(m_outputHandle, RamAiColourToWord(colour));
		WriteConsoleA(m_outputHandle, string.c_str(), string.length(), nullptr, NULL);
	}
}

void Nestopia::RamAiDebug::InstanceClearScreen()
{
	system("cls");
}

WORD Nestopia::RamAiDebug::RamAiColourToWord(const RamAi::Colour colour) const
{
	WORD returnValue = 0;

	if ((colour & RamAi::Colour::Red) != 0)		{ returnValue |= FOREGROUND_RED; }
	if ((colour & RamAi::Colour::Green) != 0)	{ returnValue |= FOREGROUND_GREEN; }
	if ((colour & RamAi::Colour::Blue) != 0)	{ returnValue |= FOREGROUND_BLUE; }
	if (colour != RamAi::Colour::White)			{ returnValue |= FOREGROUND_INTENSITY; }

	return returnValue;
}
