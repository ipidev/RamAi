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

#include "MonteCarloTreeBase.h"

#include <cassert>

#include "BestScoreCollection.h"


RamAi::MonteCarloTreeException::MonteCarloTreeException()
	: std::exception()
{
}

RamAi::MonteCarloTreeException::MonteCarloTreeException(const char *message)
	: std::exception(message)
{
}

RamAi::MonteCarloTreeException::~MonteCarloTreeException()
{
}

////////////////////////////////////////////////////////////////////////////////

RamAi::MonteCarloTreeBase::MonteCarloTreeBase()
{
}

RamAi::MonteCarloTreeBase::MonteCarloTreeBase(const MonteCarloTreeBase &other)
{
	Copy(other);
}

RamAi::MonteCarloTreeBase::MonteCarloTreeBase(MonteCarloTreeBase &&other)
{
	Move(std::move(other));
}

RamAi::MonteCarloTreeBase::~MonteCarloTreeBase()
{
}

RamAi::MonteCarloTreeBase &RamAi::MonteCarloTreeBase::operator= (const MonteCarloTreeBase &other)
{
	Copy(other);
	return *this;
}

RamAi::MonteCarloTreeBase &RamAi::MonteCarloTreeBase::operator= (MonteCarloTreeBase &&other)
{
	Move(std::move(other));
	return *this;
}

RamAi::TreeNode &RamAi::MonteCarloTreeBase::Select()
{
	TreeNode *currentNode = &m_root;

	int attemptsRemaining = 1000;
	while (attemptsRemaining-- > 0)
	{
		assert(currentNode);

#if _DEBUG
		//Sanity check - the only node without a parent should be the root.
		if (!currentNode->GetParent())
		{
			assert(currentNode = &m_root);
		}
#endif

		//Return the current node if it needs expanding.
		if (NodeNeedsExpanding(*currentNode))
		{
			return *currentNode;
		}
		//Select one of its children and try again.
		else
		{
			TreeNode *nextNode = SelectChild(*currentNode);

			if (nextNode)
			{
				//Change the current node and repeat the process.
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

	//Reached maximum number of iterations!
	assert(false);
	return *currentNode;
}

RamAi::TreeNode &RamAi::MonteCarloTreeBase::Expand(TreeNode &nodeToBeExpanded)
{
	PerformExpansion(nodeToBeExpanded);

	//If expansion resulted in more children, select one of them.
	if (!nodeToBeExpanded.IsLeaf())
	{
		TreeNode *nextNode = SelectExpandedChild(nodeToBeExpanded);

		if (nextNode)
		{
			assert(nextNode != &nodeToBeExpanded);
			return *nextNode;
		}
		else
		{
			//The only reason SelectExpandedChild() should return nullptr is if it's a leaf.
			assert(nodeToBeExpanded.IsLeaf());
			return nodeToBeExpanded;
		}
	}
	//The expansion step resulted in a leaf node: return the node itself.
	else
	{
		return nodeToBeExpanded;
	}
}

void RamAi::MonteCarloTreeBase::Backpropagate(TreeNode &nodeToBackpropagateFrom, const ScoreType score)
{
	//Traverse backwards through the tree, adding the score to each one.
	TreeNode *currentNode = &nodeToBackpropagateFrom;

	while (currentNode)
	{
#if _DEBUG
		//Sanity check - the only node without a parent should be the root.
		if (!currentNode->GetParent())
		{
			assert(currentNode = &m_root);
		}
#endif

		currentNode->GetScore().AddScore(score);
		currentNode = currentNode->GetParent();
	}
}

const RamAi::TreeNode &RamAi::MonteCarloTreeBase::GetBestScoringNode() const
{
	const TreeNode *currentNode = &m_root;

	while (!currentNode->IsLeaf())
	{
		//Find the highest scoring child node(s).
		BestScoreCollection<const TreeNode*, double> bestNodes(-std::numeric_limits<double>::infinity());

		for (auto it = currentNode->GetIteratorBegin(); it != currentNode->GetIteratorEnd(); ++it)
		{
			const double score = it->second.GetScore().GetAverageScore();

			bestNodes.Add(&(it->second), score);
		}

		const TreeNode **bestChildPtr = bestNodes.GetItem();

		assert(bestChildPtr);
		if (bestChildPtr)
		{
			//Continue down through the tree.
			currentNode = *bestChildPtr;
		}
		else
		{
			break;
		}
	}

	assert(currentNode);
	return currentNode ? *currentNode : m_root;
}

void RamAi::MonteCarloTreeBase::Copy(const MonteCarloTreeBase &other)
{
	m_root = other.m_root;
}

void RamAi::MonteCarloTreeBase::Move(MonteCarloTreeBase &&other)
{
	m_root = std::move(other.m_root);
}