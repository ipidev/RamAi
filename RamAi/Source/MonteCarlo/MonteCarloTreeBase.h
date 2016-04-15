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

#include "Settings/GameSettings.h"
#include "TreeNode.h"


namespace RamAi
{
	class MonteCarloTreeException : std::exception
	{
	public:
		MonteCarloTreeException();
		MonteCarloTreeException(const char *message);
		~MonteCarloTreeException();
	};

	//The base class for a Monte Carlo Tree Search (MCTS) implementation.
	//It uses Upper Confidence Bounds for Trees (UCT) as a tree policy.
	class MonteCarloTreeBase
	{
	public:
		typedef uint64_t ScoreType;

	public:
		MonteCarloTreeBase(const double bias);
		MonteCarloTreeBase(const MonteCarloTreeBase &other);
		MonteCarloTreeBase(MonteCarloTreeBase &&other);
		virtual ~MonteCarloTreeBase();

	public:
		MonteCarloTreeBase &operator= (const MonteCarloTreeBase &other);
		MonteCarloTreeBase &operator= (MonteCarloTreeBase &&other);

	public:
		const TreeNode &GetRoot() const				{ return m_root; }
		TreeNode &GetRoot()							{ return m_root; }

		const double GetBias() const				{ return m_bias; }
		void SetBias(const double bias)				{ m_bias = bias; }

		const TreeNode *GetBestScoringNode() const	{ return m_bestScoringNode; }

	public:
		TreeNode &Select();

	protected:
		//Returns true if the given node should be expanded.
		//By default, returns true if the given node has no children.
		virtual bool NodeNeedsExpanding(const TreeNode &node) const			{ return node.IsLeaf(); }

		//Returns the most urgent child from the parent, or nullptr if the parent is a leaf node.
		virtual TreeNode *SelectChild(const TreeNode &parent) const;

	public:
		double CalculateUcbScore(const TreeNode &child) const;
		double CalculateUcbScore(const TreeNode &parent, const TreeNode &child) const;

	public:
		virtual TreeNode &Expand(TreeNode &nodeToBeExpanded);

	protected:
		//Performs tree expansion by generating children for the given root.
		virtual void PerformExpansion(TreeNode &nodeToBeExpanded) = 0;

		//Returns the most urgent child from the parent, or nullptr if the parent is a leaf node.
		//By default, this is a synonym for SelectChild().
		virtual TreeNode *SelectExpandedChild(const TreeNode &parent) const	{ return SelectChild(parent); }

	public:
		void Backpropagate(TreeNode &nodeToBackpropagateFrom, const ScoreType score);

	protected:
		void BackpropagateUpdatingBestScoringNode(const TreeNode &nodeToBackpropagateFrom);
		virtual const TreeNode *UpdateBestScoringNode(const TreeNode &newNode) const;

	public:
		virtual std::string GetLogDetails() const;

	protected:
		static std::string DoubleToString(const double value, const int precision);

	private:
		void Copy(const MonteCarloTreeBase &other);
		void Move(MonteCarloTreeBase &&other);

	private:
		TreeNode m_root;
		double m_bias;

		const TreeNode *m_bestScoringNode;
	};
};
