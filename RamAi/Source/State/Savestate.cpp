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

RamAi::Savestate::Savestate(std::unique_ptr<uint8_t[]> &&data)
	: m_data(std::move(data))
{
}

RamAi::Savestate::Savestate(void *data, const size_t size)
	: m_data(std::make_unique<uint8_t[]>(size))
{
	uint8_t *dataAsBytes = reinterpret_cast<uint8_t*>(data);

	for (size_t i = 0; i < size; ++i)
	{
		m_data[i] = dataAsBytes[i];
	}
}

RamAi::Savestate::Savestate(Savestate &&other)
	: m_data(std::move(other.m_data))
{
}

RamAi::Savestate::~Savestate()
{
}

RamAi::Savestate &RamAi::Savestate::operator= (Savestate &&other)
{
	m_data = std::move(other.m_data);
	return *this;
}
