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

#include "BinaryCodedDecimal.h"


uint32_t RamAi::BinaryCodedDecimal::ToInt(const uint8_t *bytes, const size_t numberOfBytes, const Endianness endianness, const bool twoDigitsPerByte, const bool highDigitInUpperNibble)
{
	if (bytes && numberOfBytes > 0)
	{
		uint32_t output = 0;

		const uint8_t *currentLocation = (endianness == Endianness::Big) ? bytes : bytes + (numberOfBytes - 1);
		const int32_t direction = (endianness == Endianness::Big) ? 1 : -1;

		uint32_t digitExponent = 0;

		for (size_t i = 0; i < numberOfBytes; ++i)
		{
			//Increase the running total for the first half of the current byte.
			output += ConvertBcdDigit(*currentLocation, digitExponent, !highDigitInUpperNibble);

			//If the byte contains two digits, repeat this for the other nibble.
			if (twoDigitsPerByte)
			{
				output += ConvertBcdDigit(*currentLocation, digitExponent, highDigitInUpperNibble);
			}

			//Move to the next digit.
			currentLocation += direction;
		}

		return output;
	}
	else
	{
		return ErrorValue;
	}
}

uint32_t RamAi::BinaryCodedDecimal::Power(uint32_t base, uint32_t exponent)
{
	//Adapted from: http://stackoverflow.com/a/101613/2299850

	uint32_t result = 1;

	while (exponent != 0)
	{
		if (exponent & 1 != 0)
		{
			result *= base;
		}

		exponent >>= 1;
		base *= base;
	}

	return result;
}

uint32_t RamAi::BinaryCodedDecimal::ConvertBcdDigit(const uint8_t byte, uint32_t &inOutDigitExponent, const bool useHighNibble)
{
	const uint32_t columnValue = Power(10, inOutDigitExponent);
	++inOutDigitExponent;

	const uint8_t byteValue = useHighNibble ? (byte & 0xF0) >> 4 : (byte & 0x0F);
	return static_cast<uint32_t>(byteValue) * columnValue;
}