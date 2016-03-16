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

#include "TreeNode.h"


RamAi::TreeNode::TreeNode()
{
	m_parent = nullptr;
}


RamAi::TreeNode::TreeNode(Savestate &&savestate)
	: TreeNode()
{
	m_savestate = std::make_unique<Savestate>(std::move(savestate));
}


RamAi::TreeNode::TreeNode(const TreeNode &other)
{
	Copy(other);
}

RamAi::TreeNode::TreeNode(TreeNode &&other)
{
	Move(std::move(other));
}

RamAi::TreeNode::~TreeNode()
{
}

RamAi::TreeNode::TreeNode(TreeNode *parent)
	: TreeNode()
{
	m_parent = parent;
}

RamAi::TreeNode &RamAi::TreeNode::operator= (const TreeNode &other)
{
	Copy(other);
	return *this;
}

RamAi::TreeNode &RamAi::TreeNode::operator= (TreeNode &&other)
{
	Move(std::move(other));
	return *this;
}

RamAi::TreeNode *RamAi::TreeNode::GetChild(const ButtonSet &buttonSet)
{
	auto it = m_children.find(buttonSet);
	return (it != m_children.end()) ? &it->second : nullptr;
}

RamAi::TreeNode *RamAi::TreeNode::AddChild(const ButtonSet &buttonSet)
{
	auto result = m_children.insert({buttonSet, std::move(TreeNode(this))});
	return result.second ? &(result.first->second) : nullptr;
}

RamAi::TreeNode *RamAi::TreeNode::AddChild(ButtonSet &&buttonSet)
{
	auto result = m_children.insert({std::move(buttonSet), std::move(TreeNode(this))});
	return result.second ? &(result.first->second) : nullptr;
}

void RamAi::TreeNode::Copy(const TreeNode &other)
{
	m_children = other.m_children;
	m_parent = other.m_parent;
	m_score = other.m_score;

	if (Savestate *otherSavestate = other.m_savestate.get())
	{
		m_savestate = std::make_unique<Savestate>(*otherSavestate);
	}
	else
	{
		m_savestate = nullptr;
	}
}

void RamAi::TreeNode::Move(TreeNode &&other)
{
	m_children = std::move(other.m_children);

	m_parent = other.m_parent;
	other.m_parent = nullptr;

	m_savestate = std::move(other.m_savestate);

	m_score = std::move(other.m_score);
}
