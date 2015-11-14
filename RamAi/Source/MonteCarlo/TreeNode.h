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
	template <typename T>
	class TreeNode
	{
		static_assert(!std::is_abstract<T>::value,
			"T cannot be an abstract class.");

		static_assert(std::is_default_constructible<T>::value,
			"T must be default constructible.");

	public:
		TreeNode()
		{
		}

		TreeNode(const T &value)
			: m_value(value)
		{
		}

		TreeNode(T &&value)
			: m_value(std::move(value))
		{
		}

		TreeNode(const TreeNode<T> &other)
		{
			Copy(other);
		}

		TreeNode(TreeNode<T> &&other)
		{
			Move(std::move(other));
		}

		~TreeNode()
		{
		}

	public:
		TreeNode<T> &operator= (const TreeNode<T> &other)
		{
			Copy(other);
			return *this;
		}

		TreeNode<T> &operator= (TreeNode<T> &&other)
		{
			Move(std::move(other));
			return *this;
		}

	public:
		const T &GetValue() const		{ return m_value; }
		T &GetValue()					{ return m_value; }

		void SetValue(const T &value)	{ m_value = value; }

		const std::vector<TreeNode<T>> &GetChildren() const	{ return m_children; }
		std::vector<TreeNode<T>> &GetChildren()				{ return m_children; }

		const size_t GetNumberOfChildren() const			{ return m_children.size(); }
		const bool IsLeaf() const							{ return GetNumberOfChildren() == 0; }

		const TreeNode<T> &GetChild(const size_t index) const	{ return m_children.at(index); }
		TreeNode<T> &GetChild(const size_t index)				{ return m_children.at(index); }

		const TreeNode<T> *GetChildChecked(const size_t index) const
		{
			if (index < m_children.size())
			{
				return &m_children.at(index);
			}

			return nullptr;
		}

		TreeNode<T> *GetChildChecked(const size_t index)
		{
			return const_cast<TreeNode<T>*>(GetChildChecked(index));
		}

	public:
		void AddChild(const T &value)
		{
			AddChild(std::move(TreeNode<T>(value)));
		}

		void AddChild(const TreeNode<T> &child)
		{
			m_children.push_back(child);
		}

		void AddChild(TreeNode<T> &&child)
		{
			m_children.push_back(std::move(child));
		}

		const bool AddChildUnique(const T &value)
		{
			return AddChildUnique(std::move(TreeNode<T>(value)));
		}

		const bool AddChildUnique(const TreeNode<T> &child)
		{
			if (!ContainsChild(child.GetValue()))
			{
				AddChild(child);
				return true;
			}
			else
			{
				return false;
			}
		}

		const bool AddChildUnique(TreeNode<T> &&child)
		{
			if (!ContainsChild(child.GetValue()))
			{
				AddChild(std::move(child));
				return true;
			}
			else
			{
				return false;
			}
		}

	public:
		TreeNode<T> *FindChild(const T &value)
		{
			for (auto it = m_children.begin(); it != m_children.end(); ++it)
			{
				if (it->GetValue() == value)
				{
					return &(*it);
				}
			}

			return nullptr;
		}

		const TreeNode<T> *FindChild(const T &value) const
		{
			return const_cast<TreeNode<T>*>(this)->FindChild(value);
		}

		const bool ContainsChild(const T &value) const
		{
			return (FindChild(value) != nullptr);
		}

		const bool ContainsChild(const TreeNode<T> &node) const
		{
			for (auto it = m_children.begin(); it != m_children.end(); ++it)
			{
				if (&(*it) == &node)
				{
					return true;
				}
			}

			return false;
		}

	private:
		void Copy(const TreeNode<T> &other)
		{
			m_value = other.m_value;
			m_children = other.m_children;
		}

		void Move(TreeNode<T> &&other)
		{
			m_value = std::move(other.m_value);
			m_children = std::move(other.m_children);
		}

	private:
		T m_value;
		std::vector<TreeNode<T>> m_children;
	};
};