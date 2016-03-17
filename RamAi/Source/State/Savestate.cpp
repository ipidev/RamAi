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

#include "Savestate.h"

RamAi::Savestate::Savestate()
{
}

RamAi::Savestate::Savestate(std::unique_ptr<uint8_t[]> &&data, const size_t size)
	: m_data(std::move(data))
	, m_size(size)
{
}

RamAi::Savestate::Savestate(const uint8_t *data, const size_t size)
	: m_data(std::make_unique<uint8_t[]>(size))
	, m_size(size)
{
	CopyBytes(data, size);
}

RamAi::Savestate::Savestate(const Savestate &other)
{
	Copy(other);
}

RamAi::Savestate::Savestate(Savestate &&other)
{
	Move(std::move(other));
}

RamAi::Savestate::~Savestate()
{
}

RamAi::Savestate &RamAi::Savestate::operator= (const Savestate &other)
{
	Copy(other);
	return *this;
}

RamAi::Savestate &RamAi::Savestate::operator= (Savestate &&other)
{
	Move(std::move(other));
	return *this;
}

void RamAi::Savestate::CopyBytes(const uint8_t *bytes, const size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		m_data[i] = bytes[i];
	}
}

void RamAi::Savestate::Copy(const Savestate &other)
{
	m_size = other.m_size;

	m_data = std::make_unique<uint8_t[]>(m_size);
	CopyBytes(other.GetData().get(), m_size);
}

void RamAi::Savestate::Move(Savestate &&other)
{
	m_data = std::move(other.m_data);
	m_size = other.m_size;
}
