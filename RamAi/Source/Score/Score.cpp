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

#include "Score.h"

#include <limits>
#include <utility>

#include "Settings/ConsoleSettings.h"


RamAi::Score::Score()
{
	m_totalScore = 0;
	m_visits = 0;
}

RamAi::Score::Score(const Score &other)
{
	Copy(other);
}

RamAi::Score::Score(Score &&other)
{
	Move(std::move(other));
}

RamAi::Score::~Score()
{
}

RamAi::Score &RamAi::Score::operator= (const Score &other)
{
	Copy(other);
	return *this;
}

RamAi::Score &RamAi::Score::operator= (Score &&other)
{
	Move(std::move(other));
	return *this;
}

void RamAi::Score::AddScore(const uint32_t score)
{
	m_totalScore += static_cast<uint64_t>(score);
	++m_visits;
}

double RamAi::Score::GetNormalisedScore(const GameSettings &gameSettings) const
{
	const double maximumScore = static_cast<double>(gameSettings.GetMaximumScore());

	return maximumScore > 0.0f ? GetAverageScore() / maximumScore : std::numeric_limits<double>::infinity();
}

double RamAi::Score::GetAverageScore() const
{
	if (m_visits > 0)
	{
		return static_cast<double>(m_totalScore) / static_cast<double>(m_visits);
	}
	else
	{
		return std::numeric_limits<double>::infinity();
	}
}

void RamAi::Score::Copy(const Score &other)
{
	m_totalScore = other.m_totalScore;
	m_visits = other.m_visits;
}

void RamAi::Score::Move(Score &&other)
{
	m_totalScore = std::move(other.m_totalScore);
	m_visits = std::move(other.m_visits);
}