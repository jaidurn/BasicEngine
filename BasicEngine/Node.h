#pragma once
//==========================================================================================
// File Name: Node.h
// Author: Brian Blackmon
// Date Created: 10/25/2019
// Purpose: 
// Node template class for storage classes.
//==========================================================================================
template <class Data>
class Node
{
public:
	Node(Data data)
		:m_data(data),
		m_next(nullptr),
		m_prev(nullptr)
	{

	}

	Node(const Node<Data>& node)
	{
		m_data = node.m_data;
		m_next = node.m_next;
		m_prev = node.m_prev;
	}

	~Node()
	{
		m_next = nullptr;
		m_prev = nullptr;
	}

	Node<Data> *m_next;
	Node<Data> *m_prev;
	Data m_data;
};

