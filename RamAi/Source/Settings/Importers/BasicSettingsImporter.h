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

#include <string>
#include <unordered_map>


namespace RamAi
{
	//The base class for the settings XML document importers.
	//It takes in a simple document (containing only leaf elements with no attributes) and
	//arranges them into a map.
	class BasicSettingsImporter
	{
	public:
		BasicSettingsImporter(char *xmlString, const std::string &outerElementName);
		~BasicSettingsImporter();

	public:
		const std::string &operator[] (const std::string &key) const		{ return m_map.at(key); }

		const std::unordered_map<std::string, std::string> &GetMap() const	{ return m_map; }

		bool ContainsKey(const std::string &key) const						{ return m_map.find(key) != m_map.cend(); }

	private:
		std::unordered_map<std::string, std::string> m_map;
	};
};