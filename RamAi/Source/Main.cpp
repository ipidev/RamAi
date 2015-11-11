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

#include <iostream>

#include "MonteCarlo\TreeNode.h"


int main(int argc, char **argv)
{
	RamAi::TreeNode<int> treeNode;
	treeNode.AddChild(1);
	treeNode.AddChild(2);
	treeNode.AddChild(3);

	for (size_t i = 0; i < treeNode.GetNumberOfChildren(); ++i)
	{
		std::cout << "Child " << i << ": " << treeNode.GetChild(i).GetValue() << std::endl;
	}

	std::cin.ignore();

	return 0;
}