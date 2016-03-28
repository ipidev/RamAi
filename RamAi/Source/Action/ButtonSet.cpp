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

#include "ButtonSet.h"

#include <algorithm>


RamAi::ButtonSet::ButtonSet()
{
}

RamAi::ButtonSet::ButtonSet(const Bitfield<BitfieldType> bitfield)
	: m_bitfield(bitfield)
{
}

RamAi::ButtonSet::ButtonSet(const ButtonSet &other)
	: m_bitfield(other.m_bitfield)
{
}

RamAi::ButtonSet::ButtonSet(ButtonSet &&other)
	: m_bitfield(std::move(other.m_bitfield))
{
}

RamAi::ButtonSet::~ButtonSet()
{
}

RamAi::ButtonSet &RamAi::ButtonSet::operator= (const ButtonSet &other)
{
	m_bitfield = other.m_bitfield;
	return *this;
}

RamAi::ButtonSet &RamAi::ButtonSet::operator= (ButtonSet &&other)
{
	m_bitfield = std::move(other.m_bitfield);
	return *this;
}

std::vector<RamAi::ButtonSet> RamAi::ButtonSet::GetAllCombinations(const Bitfield<BitfieldType> baseBitfield, const Bitfield<BitfieldType> additionalBitfield)
{
	std::vector<ButtonSet> combinations;

	combinations.push_back(baseBitfield);

	//If the additional bitmask is empty, then the only possible combination is the base.
	if (!additionalBitfield.IsEmpty())
	{
		//Get minimum and maximum values to iterate between.
		const BitfieldType maximumValue = additionalBitfield.GetValue();
		BitfieldType minimumValue;

		for (size_t i = 0; i < Bitfield<BitfieldType>::NumberOfBits; ++i)
		{
			if (additionalBitfield.IsBitSet(i))
			{
				minimumValue = 1 << i;
				break;
			}
		}

		//Quite a naive method for iterating through all possible bit combinations!
		for (BitfieldType i = minimumValue; i <= maximumValue; ++i)
		{
			//If any of the bits of i match the original bitfield, we may have an original permutation.
			Bitfield<BitfieldType> matchingBits = additionalBitfield & i;

			if (!matchingBits.IsEmpty())
			{
				ButtonSet newCombination(baseBitfield | matchingBits);

				if (std::find(combinations.cbegin(), combinations.cend(), newCombination) == combinations.cend())
				{
					combinations.push_back(newCombination);
				}
			}
		}
	}

	return combinations;
}

uint32_t RamAi::ButtonSet::GetNumberOfCombinations() const
{
	uint32_t returnValue = 1;

	for (size_t i = 0; i < Bitfield<BitfieldType>::NumberOfBits; ++i)
	{
		if (m_bitfield.IsBitSet(i))
		{
			returnValue *= 2;
		}
	}

	return returnValue;
}
