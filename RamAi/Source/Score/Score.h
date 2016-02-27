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


namespace RamAi
{
	//The score for a particular node in the search tree.
	class Score
	{
	public:
		Score();
		Score(const Score &other);
		Score(Score &&other);
		~Score();

	public:
		Score &operator= (const Score &other);
		Score &operator= (Score &&other);

	public:
		uint64_t GetTotalScore() const	{ return m_totalScore; }
		uint64_t GetVisits() const		{ return m_visits; }

	public:
		void AddScore(const uint32_t score);
		double GetAverageScore() const;

	private:
		void Copy(const Score &other);
		void Move(Score &&other);

	private:
		uint64_t m_totalScore;
		uint64_t m_visits;
	};
};