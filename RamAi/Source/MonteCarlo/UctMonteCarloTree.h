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

#pragma once

#include "MonteCarloTreeBase.h"
#include "BestScoreCollection.h"


namespace RamAi
{
	//A Monte Carlo Tree using Upper Confidence Bounds for Trees (UCT) as a default policy.
	class UctMonteCarloTree : public MonteCarloTreeBase
	{
	public:
		UctMonteCarloTree(const float bias);
		UctMonteCarloTree(const UctMonteCarloTree &other);
		UctMonteCarloTree(UctMonteCarloTree &&other);
		virtual ~UctMonteCarloTree();

	public:
		UctMonteCarloTree &operator= (const UctMonteCarloTree &other);
		UctMonteCarloTree &operator= (UctMonteCarloTree &&other);

	public:
		const float GetBias() const		{ return m_bias; }
		void SetBias(const float bias)	{ m_bias = bias; }

	protected:
		virtual TreeNode *SelectChild(const TreeNode &parent) const override;

		float CalculateUcbScore(const TreeNode &parent, const TreeNode &child) const;

	protected:
		void Copy(const UctMonteCarloTree &other);
		void Move(UctMonteCarloTree &&other);

	protected:
		float m_bias;
	};
};