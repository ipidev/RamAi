#include "TestMonteCarloTree.h"


RamAi::TestMonteCarloTree::TestMonteCarloTree(const float bias)
	: UctMonteCarloTree(bias)
{
}

RamAi::TestMonteCarloTree::~TestMonteCarloTree()
{
}

void RamAi::TestMonteCarloTree::PerformExpansion(TreeNode &nodeToBeExpanded)
{
	nodeToBeExpanded.AddChild(ButtonSet(0));
	nodeToBeExpanded.AddChild(ButtonSet(1));
}
