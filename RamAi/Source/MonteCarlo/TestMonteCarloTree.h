#pragma once

#include "MonteCarlo\TreeNode.h"
#include "MonteCarlo\UctMonteCarloTree.h"

namespace RamAi
{
	class TestMonteCarloTree : public UctMonteCarloTree
	{
	public:
		TestMonteCarloTree(const float bias);
		~TestMonteCarloTree();

	protected:
		virtual void PerformExpansion(TreeNode &nodeToBeExpanded) override;
	};
};