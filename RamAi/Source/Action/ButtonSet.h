/*
	RamAi - A general game-playing AI that uses RAM states as input to a value function
	Copyright (C) 2015 Sean Latham

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
#include <vector>

#include "Data\Bitfield.h"


namespace RamAi
{
	//A bitfield that represents the buttons that are being pressed on the
	//gamepad during a single frame. This is the primary output of the AI.
	class ButtonSet
	{
	public:
		typedef uint32_t BitfieldType;

	public:
		ButtonSet();
		ButtonSet(const Bitfield<BitfieldType> bitfield);
		ButtonSet(const ButtonSet &other);
		ButtonSet(ButtonSet &&other);
		~ButtonSet();

	public:
		ButtonSet &operator= (const ButtonSet &other);
		ButtonSet &operator= (ButtonSet &&other);

		bool operator== (const ButtonSet &other) const	{ return m_bitfield == other.m_bitfield; }
		bool operator!= (const ButtonSet &other) const	{ return m_bitfield != other.m_bitfield; }

	public:
		//Returns a collection of all button combinations that contain every permutation of the bitmasks
		//that can be created by setting/unsetting the bits defined in additionalBitmask. The bits defined in
		//baseBitfield are always set.
		static std::vector<ButtonSet> GetAllCombinations(const Bitfield<BitfieldType> baseBitfield, const Bitfield<BitfieldType> additionalBitfield);

	public:
		const Bitfield<BitfieldType> GetBitfield() const		{ return m_bitfield; }
		void SetBitfield(const Bitfield<BitfieldType> bitfield)	{ m_bitfield = bitfield; }

	private:
		Bitfield<BitfieldType> m_bitfield;
	};
};

namespace std
{
	//Hash function for ButtonSet. Allows it to be used as a key in a map/set.
	template <>
	struct hash<RamAi::ButtonSet>
	{
		size_t operator()(const RamAi::ButtonSet &value) const noexcept
		{
			//TODO: Write actual hashing function.
			return value.GetBitfield().GetValue();
		}
	};
};
