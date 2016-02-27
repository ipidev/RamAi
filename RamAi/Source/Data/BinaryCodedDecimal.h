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

#include <cstdint>
#include <limits>


namespace RamAi
{
	//A bitfield with a given underlying type.
	class BinaryCodedDecimal
	{
	public:
		enum Endianness
		{
			Big,
			Little
		};

		static const uint32_t ErrorValue = std::numeric_limits<uint32_t>::max();

	public:
		BinaryCodedDecimal() = delete;
		~BinaryCodedDecimal() = delete;

	public:
		//TODO: Mkae this work for cases where two decimal digits are stored per byte.
		static uint32_t ToInt(const uint8_t *bytes, const size_t numberOfBytes, const Endianness endianness);

		static uint32_t Power(uint32_t base, uint32_t exponent);
	};
};