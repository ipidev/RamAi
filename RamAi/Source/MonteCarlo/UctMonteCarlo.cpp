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

#include "UctMonteCarlo.h"

#include <cstdint>
#include <limits>


RamAi::UctScore::UctScore()
	: m_totalScore(0.0f)
	, m_visits(0)
{
}

void RamAi::UctScore::AddScore(const float score)
{
	if (m_visits < std::numeric_limits<size_t>::max())
	{
		m_totalScore += score;
		++m_visits;
	}
}

const float RamAi::UctScore::GetScore() const
{
	return (m_visits > 0) ? m_totalScore / m_visits : std::numeric_limits<float>::infinity();
}