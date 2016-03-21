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

#include "Ram.h"

#include <algorithm>
#include <cassert>

#include "BinaryCodedDecimal.h"


RamAi::Ram::Ram()
	: m_data(nullptr)
	, m_size(0)
{
}

RamAi::Ram::Ram(const size_t size)
	: m_data(std::make_unique<uint8_t[]>(size))
	, m_size(size)
{
	Clear();
}

RamAi::Ram::Ram(const uint8_t *data, const size_t size)
	: Ram(size)
{
	Copy(data, size);
}

RamAi::Ram::Ram(const Ram &other)
	: Ram(other.m_size)
{
	Copy(other.m_data.get(), other.m_size);
}

RamAi::Ram::Ram(Ram &&other)
	: m_data(std::move(other.m_data))
	, m_size(std::move(other.m_size))
{
}

RamAi::Ram &RamAi::Ram::operator= (const Ram &other)
{
	m_data = std::make_unique<uint8_t[]>(other.m_size);
	m_size = other.m_size;
	Copy(other.m_data.get(), m_size);
	return *this;
}

RamAi::Ram &RamAi::Ram::operator= (Ram &&other)
{
	m_data = std::move(other.m_data);
	m_size = std::move(other.m_size);
	return *this;
}

const uint8_t &RamAi::Ram::operator[] (const size_t index) const
{
	return const_cast<Ram*>(this)->operator[](index);
}

uint8_t &RamAi::Ram::operator[] (const size_t index)
{
	assert(m_data);
	assert(index < m_size);
	return *(m_data.get() + index);
}

void RamAi::Ram::Copy(const uint8_t *data, const size_t size)
{
	uint8_t *ownData = m_data.get();

	if (ownData && data)
	{
		const size_t bytesToCopy = std::min<size_t>(size, m_size);
		memcpy_s(ownData, bytesToCopy, data, size);
	}
}

void RamAi::Ram::Clear(const uint8_t defaultValue)
{
	uint8_t *ownData = m_data.get();

	if (ownData)
	{
		memset(ownData, defaultValue, m_size);
	}
}

uint32_t RamAi::Ram::GetCurrentScore(const GameDetails &gameDetails) const
{
	const uint8_t *scoreAddress = m_data.get() + gameDetails.scoreOffset;

	return BinaryCodedDecimal::ToInt(scoreAddress, gameDetails.scoreSize, gameDetails.scoreEndianness);
}
