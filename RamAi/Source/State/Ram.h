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

#include <memory>


namespace RamAi
{
	//An individual RAM state.
	class Ram
	{
	public:
		Ram();
		Ram(const size_t size);
		Ram(const uint8_t *data, const size_t size);
		Ram(const Ram &other);
		Ram(Ram &&other);
		~Ram() = default;

	public:
		Ram &operator= (const Ram &other);
		Ram &operator= (Ram &&other);

		//TODO: Implement operator[] and range checks.
		const uint8_t &operator[] (const size_t index) const;
		uint8_t &operator[] (const size_t index);
		
	public:
		const bool HasData() const							{ return m_data.get() != nullptr; }
		const std::unique_ptr<uint8_t[]> &GetData()	const	{ return m_data; }
		std::unique_ptr<uint8_t[]> &GetData()				{ return m_data; }

		const size_t GetSize() const	{ return m_size; }

	public:
		void Copy(const uint8_t *data, const size_t size);
		void Clear(const uint8_t defaultValue = 0);

	private:
		std::unique_ptr<uint8_t[]> m_data;
		size_t m_size;
	};
};