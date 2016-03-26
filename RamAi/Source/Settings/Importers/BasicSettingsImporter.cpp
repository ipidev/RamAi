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

#include "BasicSettingsImporter.h"

#include <RapidXml/rapidxml.hpp>


RamAi::BasicSettingsImporter::BasicSettingsImporter(char *xmlString, const std::string &outerElementName)
{
	rapidxml::xml_document<char> xmlDocument;
	xmlDocument.parse<rapidxml::parse_default>(xmlString);

	if (rapidxml::xml_node<char> *rootNode = xmlDocument.first_node(outerElementName.c_str()))
	{
		//Iterate through all of the root node's children.
		rapidxml::xml_node<char> *currentNode = rootNode->first_node();

		while (currentNode)
		{
			std::string nodeName(currentNode->name());

			//If the key already exists in the map, just replace it.
			std::unordered_map<std::string, std::string>::iterator existingKey = m_map.find(nodeName);

			if (existingKey != m_map.end())
			{
				existingKey->second = std::string(currentNode->value());
			}
			//Otherwise, add it into the map.
			else
			{
				m_map.emplace(std::move(nodeName), currentNode->value());
			}
		}
	}
}

RamAi::BasicSettingsImporter::~BasicSettingsImporter()
{
}
