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

#include <cassert>
#include <type_traits>
#include <vector>

#include "TreeNode.h"


namespace RamAi
{
	//Interface for the score of a particular tree node.
	class MonteCarloScore
	{
	public:
		MonteCarloScore() {}

	public:
		virtual void AddScore(const float score) = 0;
		virtual const float GetScore() const = 0;
	};

	//Base class for a simple Monte Carlo Tree Search algorithm.
	template <typename StateType, typename ScoreType>
	class MonteCarloBase
	{
		static_assert(!std::is_abstract<ScoreType>::value,
			"StateType cannot be an abstract class.");

		static_assert(std::is_constructible<StateType>::value,
			"StateType must be default constructible.");

		static_assert(!std::is_abstract<ScoreType>::value,
			"ScoreType cannot be an abstract class.");

		static_assert(std::is_constructible<ScoreType>::value,
			"ScoreType must be default constructible.");

		static_assert(std::is_base_of<MonteCarloScore, ScoreType>::value,
			"ScoreType must derive from MonteCarloScore.");

	public:
		MonteCarloBase(const StateType &initialState)
			: m_root(NodeType(initialState))
		{
		}		

		MonteCarloBase(StateType &&initialState)
			: m_root(NodeType(std::move(initialState)))
		{
		}

		MonteCarloBase(const MonteCarloBase &other)
			: m_root(other.m_root)
		{
		}

		MonteCarloBase(MonteCarloBase &&other)
			: m_root(std::move(other.m_root))
		{
		}

		virtual ~MonteCarloBase() = default;

	public:
		virtual MonteCarloBase &operator= (const MonteCarloBase &other)
		{
			m_root = other.m_root;
			return *this;
		}

		virtual MonteCarloBase &operator= (MonteCarloBase &&other)
		{
			m_root = std::move(other.m_root);
			return *this;
		}

	public:
		void PerformSearch()
		{
			std::vector<TreeNode<NodeType>*> traversedNodes;
			TreeNode<NodeType> &selectedNode = Select(traversedNodes);
			TreeNode<NodeType> &expandedNode = Expand(selectedNode, traversedNodes);

			const float score = Simulate(expandedNode.GetValue().state);

			Backpropagate(traversedNodes, score);
		}

	protected:
		//Datatype used by the tree structure.
		struct NodeType
		{
			NodeType() {}
			NodeType(const StateType &inState) : state(inState) {}
			NodeType(StateType &&inState) : state(std::move(inState)) {}
			NodeType(const NodeType &other) = default;
			NodeType(NodeType &&other) = default;
			~NodeType() = default;

			NodeType &operator= (const NodeType &other)
			{
				state = other.state;
				score = other.score;
				return *this;
			}

			NodeType &operator= (NodeType &&other)
			{
				state = std::move(other.state);
				score = std::move(other.score);
				return *this;
			}

			StateType state;
			ScoreType score;
		};

	protected:
		//Selects a node that needs to be expanded.
		TreeNode<NodeType> &Select(std::vector<TreeNode<NodeType>*> &outTraversedNodes)
		{
			assert(outTraversedNodes.empty());

			TreeNode<NodeType> *currentNode = &m_root;

			int attemptsRemaining = 100;
			while (attemptsRemaining-- > 0)
			{
				assert(currentNode);
				outTraversedNodes.push_back(currentNode);

				//Return the current node if it needs expanding (as per some external criteria)
				//or it has no children (even after expansion).
				if (NodeNeedsExpanding(*currentNode) || currentNode->IsLeaf())
				{
					return *currentNode;
				}
				//Select one of its children and try again.
				else
				{
					TreeNode<NodeType> *nextNode = SelectChild(*currentNode);

					if (nextNode)
					{
						assert(nextNode != currentNode);
						currentNode = nextNode;
					}
					else
					{
						//The only reason SelectChild() should return nullptr is if it's a leaf.
						assert(currentNode->IsLeaf());
						return *currentNode;
					}
				}
			}

			//Reached maximum iterations!
			assert(false);
			return *currentNode;
		}

		//Returns the desired child of the given root for this iteration. 
		//If the given root is a leaf, nullptr should be returned.
		virtual TreeNode<NodeType> *SelectChild(TreeNode<NodeType> &root) = 0;

		//Whether or not the given node's children should (or can) be created.
		//By default, all leaf nodes will be expanded.
		virtual const bool NodeNeedsExpanding(const TreeNode<NodeType> &node)
		{
			return node.IsLeaf();
		}

	protected:
		TreeNode<NodeType> &Expand(TreeNode<NodeType> &root, std::vector<TreeNode<NodeType>*> &outTraversedNodes)
		{
			PerformExpansion(root);

			//If expansion resulted in more children, select one of them.
			if (!root.IsLeaf())
			{
				TreeNode<NodeType> *nextNode = SelectExpandedChild(root);

				if (nextNode)
				{
					assert(nextNode != &root);

					outTraversedNodes.push_back(nextNode);
					return *nextNode;
				}
				else
				{
					//The only reason SelectExpandedChild() should return nullptr is if it's a leaf.
					assert(root.IsLeaf());
					return root;
				}
			}
			//The expansion step resulted in a leaf node: return the node itself.
			else
			{
				return root;
			}
		}

		//Performs tree expansion by generating children for the given root.
		virtual void PerformExpansion(TreeNode<NodeType> &root) = 0;

		//Returns the desired child of the given root that's been newly expanded.
		//By default, this is a synonym for SelectChild();
		//If the given root is a leaf, nullptr should be returned.
		virtual TreeNode<NodeType> *SelectExpandedChild(TreeNode<NodeType> &root)
		{
			return SelectChild(root);
		}

	protected:
		//Simulates the given state until a terminal state is reached and returns a score value.
		virtual const float Simulate(const StateType &state) = 0;

	protected:
		//Backpropagates and adds the score onto all of the nodes that were traversed this search iteration.
		//By default, calls MonteCarloScore::AddScore() on each node.
		virtual void Backpropagate(const std::vector<TreeNode<NodeType>*> &traversedNodes, const float score)
		{
			for (size_t i = 0; i < traversedNodes.size(); ++i)
			{
				assert(traversedNodes[i]);
				traversedNodes[i]->GetValue().score.AddScore(score);
			}
		}

	protected:
		TreeNode<NodeType> m_root;
	};
};
