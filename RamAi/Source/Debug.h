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

#include <memory>
#include <string>


namespace RamAi
{
	//Primary and secondary colours of light.
	enum Colour
	{
		Black	= 0,
		Red		= 1 << 0,
		Green	= 1 << 1,
		Blue	= 1 << 2,

		Yellow	= Red | Green,
		Cyan	= Green | Blue,
		Magenta	= Blue | Red,
		White	= Red | Green | Blue
	};

	//Singleton class responsible for managing a debug output console.
	class Debug
	{
	public:
		Debug();
		virtual ~Debug();

	public:
		static Debug *GetInstance()									{ return s_instance.get(); }
		static void SetInstance(std::unique_ptr<Debug> &&instance)	{ s_instance = std::move(instance); }

		static void Out(const std::string &string, const Colour colour = Colour::White);
		static void OutLine(const std::string &string, const Colour colour = Colour::White)	{ Out(string + "\n", colour); }
		static void ClearScreen();

	public:
		virtual void InstanceOut(const std::string &string, const Colour colour) = 0;
		virtual void InstanceClearScreen() = 0;

	private:
		static std::unique_ptr<Debug> s_instance;
	};
};
