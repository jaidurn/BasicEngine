#pragma once
//==========================================================================================
// File Name: LinkedList.h
// Author: Brian Blackmon
// Date Created: 12/3/2012
// Purpose: 
// A bare bones doubly linked list class.
//==========================================================================================
#include "Node.h"

template <class Data>
class LinkedList
{
public:
	LinkedList()
		:m_head(nullptr),
		m_tail(nullptr),
		m_it(nullptr),
		m_count(0)
	{

	}

	~LinkedList()
	{
//		clear();
	}

//	void insert(const Data data);
//	void remove(const Data data);

	// Iterator functions
//	bool hasMore() const;

private:
	Node<Data>* m_head;
	Node<Data>* m_tail;
	Node<Data>* m_it;

	int m_count;

//	void clear();
};