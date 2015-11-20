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

#include <limits>
#include <type_traits>


namespace RamAi
{
	//A bitfield that represents the buttons that are being pressed during a
	//single frame. This is the primary output of the AI.
	template <typename T>
	class Bitfield
	{
		static_assert(std::is_arithmetic<T>::value,
			"The underlying type must be arithmetic.");

		static_assert(std::is_integral<T>::value,
			"The underlying type must be integral.");

		static_assert(std::is_fundamental<T>::value,
			"The underlying type must be fundamental.");

		static_assert(!std::is_const<T>::value,
			"The underlying type must be non-const.");

	public:
		Bitfield()					: Bitfield(0) {}
		Bitfield(const T value)		: m_value(value) {}
		Bitfield(const Bitfield &other) = default;
		Bitfield(Bitfield &&other) = default;
		~Bitfield() = default;

	public:
		Bitfield &operator= (const Bitfield &other) = default;
		Bitfield &operator= (Bitfield &&other) = default;

	public:
		const size_t NumberOfBits = sizeof(T) * 8;

	public:
		const bool IsBitSet(const size_t position) const	{ return (m_value & static_cast<T>(1 << position)) != 0; }

		void SetBit(const size_t position)					{ m_value |= static_cast<T>(1 << position); }
		void UnsetBit(const size_t position)				{ m_value &= static_cast<T>(~(1 << position)); }
		void InvertBit(const size_t position)				{ m_value ^= static_cast<T>(1 << position); }

	public:
		const bool AreAnySet(const T bitmask) const			{ return (m_value & bitmask) != 0; }
		const bool AreAllSet(const T bitmask) const			{ return (m_value & bitmask) == bitmask; }

		void SetAll(const T bitmask)						{ m_value |= bitmask; }
		void UnsetAll(const T bitmask)						{ m_value &= ~bitmask; }
		void InvertAll(const T bitmask)						{ m_value ^= bitmask; }

	public:
		const bool IsFull() const							{ return m_value == std::numeric_limits<T>::max(); }
		const bool IsEmpty() const							{ return m_value == 0; }

		void Fill()											{ m_value = std::numeric_limits<T>::max(); }
		void Clear()										{ m_value = 0; }
		void Invert()										{ m_value = ~m_value; }

	public:
		const T GetValue() const							{ return m_value; }
		void SetValue(const T value)						{ m_value = value; }

	private:
		T m_value;
	};
};