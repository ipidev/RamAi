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

#include <unordered_map>

#include "Action/ButtonSet.h"
#include "Score/Score.h"


namespace RamAi
{
	class TreeNode
	{
	public:
		TreeNode();
		TreeNode(const TreeNode &other);
		TreeNode(TreeNode &&other);
		~TreeNode();

	private:
		TreeNode(TreeNode *parent);

	public:
		TreeNode &operator= (const TreeNode &other);
		TreeNode &operator= (TreeNode &&other);

	public:
		const TreeNode *GetChild(const ButtonSet &buttonSet) const	{ return const_cast<TreeNode*>(this)->GetChild(buttonSet); }
		TreeNode *GetChild(const ButtonSet &buttonSet);
		bool ContainsAction(const ButtonSet &buttonSet) const		{ return GetChild(buttonSet) != nullptr; }

		size_t GetNumberOfChildren() const							{ return m_children.size(); }
		bool IsLeaf() const											{ return GetNumberOfChildren() == 0; }

		std::unordered_map<ButtonSet, TreeNode>::const_iterator GetIteratorBegin() const	{ return m_children.cbegin(); }
		std::unordered_map<ButtonSet, TreeNode>::const_iterator GetIteratorEnd() const		{ return m_children.cend(); }

		const TreeNode *GetParent() const							{ return m_parent; }
		TreeNode *GetParent()										{ return m_parent; }
		bool IsRoot() const											{ return m_parent == nullptr; }

		const Score &GetScore() const								{ return m_score; }
		Score &GetScore()											{ return m_score; }

	public:
		TreeNode *AddChild(const ButtonSet &buttonSet);
		TreeNode *AddChild(ButtonSet &&buttonSet);

	private:
		void Copy(const TreeNode &other);
		void Move(TreeNode &&other);

	private:
		std::unordered_map<ButtonSet, TreeNode> m_children;
		TreeNode *m_parent;
		Score m_score;
	};
};