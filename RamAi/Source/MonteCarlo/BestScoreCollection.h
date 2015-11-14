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

#include <type_traits>
#include <vector>


namespace RamAi
{
	//Container that only stores data with the highest "score". Chooses randomly to break ties. 
	template<typename ItemType, typename ScoreType>
	class BestScoreCollection
	{
		static_assert(std::is_arithmetic<ScoreType>::value,
			"ScoreType must be arithmetic.");

	public:
		BestScoreCollection(const ScoreType initialScore)
			: m_score(initialScore)
		{
		}

		BestScoreCollection(const BestScoreCollection &other)
			: m_items(other.m_items)
			, m_score(other.m_score)
		{
		}

		BestScoreCollection(BestScoreCollection &&other)
			: m_items(std::move(other.m_items))
			, m_score(std::move(other.m_score))
		{
		}

		~BestScoreCollection()
		{
		}

	public:
		BestScoreCollection &operator= (const BestScoreCollection &other)
		{
			m_items = other.m_items;
			m_score = other.m_score;
			return *this;
		}

		BestScoreCollection &operator= (BestScoreCollection &&other)
		{
			m_items = std::move(other.m_items);
			m_score = std::move(other.m_score);
			return *this;
		}

	public:
		const std::vector<ItemType> &GetItems() const	{ return m_items; }
		const ScoreType GetScore() const				{ return m_score; }

	public:
		void Add(const ItemType &item, const ScoreType score)
		{
			//If the new item's score is greater than our one, clear the items with lower score.
			if (score > m_score)
			{
				m_items.clear();
				m_items.push_back(item);

				m_score = score;
			}
			else if (score == m_score)
			{
				m_items.push_back(item);
			}
		}

		ItemType *GetItem()
		{
			const size_t numberOfItems = m_items.size();

			if (numberOfItems > 0)
			{
				const size_t index = static_cast<size_t>(rand()) % numberOfItems;

				return &m_items[index];
			}
			else
			{
				return nullptr;
			}
		}

		const ItemType *GetRandomItem() const
		{
			return const_cast<BestScoreCollection<ItemType, ScoreType>*>(this)->GetRandomItem();
		}

		void Clear(const ScoreType newScore)
		{
			m_items.clear();
			m_score = newScore;
		}

	private:
		std::vector<ItemType> m_items;
		ScoreType m_score;
	};
};