#pragma once
//==========================================================================================
// File Name: Grid.h
// Author: Brian Blackmon
// Date Created: 10/25/2019
// Purpose: 
// A grid storage class. It holds items in a linked grid.
//==========================================================================================
#include <vector>
#include "Node.h"
#include "Rectangle.h"

template <class Data>
class Grid
{
public:
	Grid(const int width,
		const int height,
		const int cellWidth,
		const int cellHeight)
	{

	}

private:
	std::vector<Node<Data>*> m_grid;
};