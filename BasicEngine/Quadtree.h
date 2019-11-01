#pragma once
//==========================================================================================
// File Name: Quadtree.h
// Author: Brian Blackmon
// Date Created: 10/25/2019
// Purpose: 
// A quad tree storage object.
//==========================================================================================
#include "Rectangle.h"
#include "Line.h"
#include <vector>
#include <memory>

class Renderer;

struct EntityData
{
	EntityData(const int id,
		const Rectangle& size)
		:m_id(id),
		m_size(size)
	{
		
	}

	int m_id;
	Rectangle m_size;
};

class Quadtree
{
public:
	Quadtree(const int maxObjects,
		const int maxLevels,
		const int level,
		const Rectangle& bounds,
		Quadtree *parent);

	~Quadtree();

	const int getWidth() const;
	const int getHeight() const;

	std::vector<EntityData> search(const Rectangle& searchArea) const;
	std::vector<EntityData> search(const Line& searchLine) const;

	void addEntity(const EntityData& entity);
	void removeEntity(const EntityData& entity);

	void renderTree(Renderer *renderer, 
		const Vector2D& offset,
		const float xScale,
		const float yScale);

private:
	static const int THIS_TREE = -1;
	static const int CHILD_NW = 0;
	static const int CHILD_NE = 1;
	static const int CHILD_SE = 2;
	static const int CHILD_SW = 3;

	std::vector<EntityData> m_data;

	Quadtree *m_parent;
	std::shared_ptr<Quadtree> m_children[4];

	Rectangle m_bounds;

	int m_maxObjects;
	int m_maxLevels;
	int m_level;

	void cleanUp();

	void search(const Rectangle& searchArea,
		std::vector<EntityData>& data) const;

	void search(const Line& searchLine,
		std::vector<EntityData>& data) const;

	void getData(std::vector<EntityData>& data) const;

	void split();

	const int getChildIndex(const Vector2D& point) const;
};