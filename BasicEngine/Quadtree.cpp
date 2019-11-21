#include "Quadtree.h"
#include "Collision.h"
#include "Renderer.h"

Quadtree::Quadtree(const int maxObjects,
	const int maxLevels,
	const int level,
	const Rectangle& bounds,
	Quadtree *parent)
	:m_maxObjects(maxObjects),
	m_maxLevels(maxLevels),
	m_level(level),
	m_bounds(bounds),
	m_parent(parent)
{
	
}

Quadtree::~Quadtree()
{
	cleanUp();
}

//=============================================================================
// Function: const int getWidth() const
// Description:
// Gets the width of the Quadtree.
// Output:
// const int
// Returns width.
//=============================================================================
const int Quadtree::getWidth() const
{
	return m_bounds.getWidth();
}

//=============================================================================
// Function: const int getHeight() const
// Description:
// Gets the height of the Quadtree.
// Output:
// const int
// Returns height.
//=============================================================================
const int Quadtree::getHeight() const
{
	return m_bounds.getHeight();
}

//=============================================================================
// Function: vector<EntityData> search(const Rectangle&) const
// Description:
// Searches the tree for any entities inside the specified rectangle.
// Parameters:
// const Rectangle& searchArea - The area to search inside.
// Output:
// vector<EntityData> Returns a vector filled with all of the
// entities inside the search area.
//=============================================================================
std::vector<EntityData> Quadtree::search(const Rectangle& searchArea) const
{
	std::vector<EntityData> data;

	if (rectIntersectRect(m_bounds, searchArea))
	{
		if (m_children[0] != nullptr)
		{
			for (int i = 0; i <= CHILD_SW; i++)
			{
				m_children[i]->search(searchArea, data);
			}
		}

		for (unsigned int i = 0; i < m_data.size(); i++)
		{
			if (rectIntersectRect(m_data[i].m_size, searchArea))
			{
				data.emplace_back(m_data[i]);
			}
		}
	}

	return data;
}

//=============================================================================
// Function: void search(const Line&, vector<EntityData>&) const
// Description:
// Searches along the search line and adds any entities that collide
// with it to the data vector.
// Parameters:
// const Line& searchLine - The line to search along.
// vector<EntityData>& data - The data vector to fill.
//=============================================================================
void Quadtree::search(const Line& searchLine,
	std::unordered_map<int, EntityData>& data) const
{
	if (lineInRect(m_bounds, searchLine))
	{
		if (m_children[0] != nullptr)
		{
			m_children[CHILD_NW]->search(searchLine, data);
			m_children[CHILD_NE]->search(searchLine, data);
			m_children[CHILD_SE]->search(searchLine, data);
			m_children[CHILD_SW]->search(searchLine, data);
		}

		for (unsigned int i = 0; i < m_data.size(); i++)
		{
			if (data.count(m_data[i].m_id) == 0)
			{
				if (lineInRect(m_data[i].m_size, searchLine))
				{
					data.insert(std::make_pair(m_data[i].m_id, m_data[i]));
				}
			}
		}
	}
}

//=============================================================================
// Function: void addEntity(const EntityData&)
// Description:
// Adds an entity to the tree.
// Parameters:
// const EntityData& entity - The entity to add to the quad tree.
//=============================================================================
void Quadtree::addEntity(const EntityData& entity)
{
	if (m_children[0] != nullptr)
	{
		int index = getChildIndex(entity.m_size.getCenter());

		if (index != THIS_TREE)
		{
			m_children[index]->addEntity(entity);
			return;
		}
	}

	m_data.emplace_back(entity);

	if (m_maxObjects < (int)m_data.size() && 
		m_children[0] == nullptr)
	{
		split();

		auto vit = m_data.begin();

		while (vit != m_data.end())
		{
			int index = getChildIndex(vit->m_size.getCenter());

			if (index != THIS_TREE)
			{
				m_children[index]->addEntity(*vit);
				
				vit = m_data.erase(vit);
			}
			else
			{
				vit++;
			}
		}
	}
}

//=============================================================================
// Function: void removeEntity(const EntityData&)
// Description:
// Finds and removes an entity from the quad tree.
// Parameters:
// const EntityData& entity - The entity to remove.
//=============================================================================
void Quadtree::removeEntity(const EntityData& entity)
{
	if (m_children[0] != nullptr)
	{
		int index = getChildIndex(entity.m_size.getCenter());

		if (index != THIS_TREE)
		{
			m_children[index]->removeEntity(entity);
			return;
		}
	}

	auto vit = m_data.begin();

	while (vit != m_data.end())
	{
		if (vit->m_id == entity.m_id)
		{
			vit = m_data.erase(vit);
			break;
		}
		else
		{
			vit++;
		}
	}
}

void Quadtree::renderTree(Renderer *renderer, 
	const Vector2D& offset,
	const float xScale,
	const float yScale)
{
	if (m_children[0] != nullptr)
	{
		m_children[CHILD_NW]->renderTree(renderer, offset, xScale, yScale);
		m_children[CHILD_NE]->renderTree(renderer, offset, xScale, yScale);
		m_children[CHILD_SE]->renderTree(renderer, offset, xScale, yScale);
		m_children[CHILD_SW]->renderTree(renderer, offset, xScale, yScale);
	}
	else
	{
		SDL_Color red{ 255, 0, 0, 255 };

		Vector2D center = m_bounds.getCenter();

		center.m_x *= xScale;
		center.m_y *= yScale;

		center -= offset;

		Rectangle scaled(center, m_bounds.getWidth() * xScale, m_bounds.getHeight() * yScale);
		renderer->drawRect(scaled, red, false);
	}
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up all of the nodes.
//=============================================================================
void Quadtree::cleanUp()
{
	if (m_children[0] != nullptr)
	{
		for (int i = 0; i <= CHILD_SW; i++)
		{
			m_children[i].reset();
		}
	}
}

//=============================================================================
// Function: void search(const Rectangle&, 
// vector<EntityData>&) const
// Description:
// Adds all of the items which are inside the search area.
// NOTE: This search function adds everything in the node
// If the node is inside the node.
// Parameters:
// const Rectangle& searchArea - The area to search.
// vector<EntityData>& data - The data to fill.
//=============================================================================
void Quadtree::search(const Rectangle& searchArea,
	std::vector<EntityData>& data) const
{
	if (rectIntersectRect(m_bounds, searchArea))
	{
		if (rectInsideRect(m_bounds, searchArea))
		{
			getData(data);
		}
		else
		{
			if (m_children[0] != nullptr)
			{
				for (int i = 0; i <= CHILD_SW; i++)
				{
					m_children[i]->search(searchArea, data);
				}
			}

			for (unsigned int i = 0; i < m_data.size(); i++)
			{
				if (rectIntersectRect(searchArea, m_data[i].m_size))
				{
					data.emplace_back(m_data[i]);
				}
			}
		}
	}
}

//=============================================================================
// Function: void getData(vector<EntityData>&) const
// Description:
// Gets all of the data from the node it's children.
// Parameters:
// vector<EntityData>& data - The data vector to fill up.
//=============================================================================
void Quadtree::getData(std::vector<EntityData>& data) const
{
	if (m_children[0] != nullptr)
	{
		m_children[CHILD_NW]->getData(data);
		m_children[CHILD_NE]->getData(data);
		m_children[CHILD_SE]->getData(data);
		m_children[CHILD_SW]->getData(data);
	}

	for (unsigned int i = 0; i < m_data.size(); i++)
	{
		data.emplace_back(m_data[i]);
	}
}

//=============================================================================
// Function: void split()
// Description:
// Splits the quad tree into 4 leafs if it's possible.
//=============================================================================
void Quadtree::split()
{
	if (m_children[0] == nullptr)
	{
		if (m_level < m_maxLevels)
		{
			int halfWidth = m_bounds.getWidth() / 2;
			int halfHeight = m_bounds.getHeight() / 2;
			Vector2D center = m_bounds.getCenter();

			m_children[CHILD_NW] =
				std::make_shared<Quadtree>(m_maxObjects,
					m_maxLevels,
					m_level + 1,
					Rectangle(Vector2D(center.m_x - (float)(halfWidth / 2), center.m_y - (float)(halfHeight / 2)), halfWidth, halfHeight),
					this);

			m_children[CHILD_NE] =
				std::make_shared<Quadtree>
				(m_maxObjects,
					m_maxLevels,
					m_level + 1,
					Rectangle(Vector2D(center.m_x + (float)(halfWidth / 2), center.m_y - (float)(halfHeight / 2)), halfWidth, halfHeight),
					this);

			m_children[CHILD_SE] =
				std::make_shared<Quadtree>
				(m_maxObjects,
					m_maxLevels,
					m_level + 1,
					Rectangle(Vector2D(center.m_x + (float)(halfWidth / 2), center.m_y + (float)(halfHeight / 2)), halfWidth, halfHeight),
					this);

			m_children[CHILD_SW] =
				std::make_shared<Quadtree>
				(m_maxObjects,
					m_maxLevels,
					m_level + 1,
					Rectangle(Vector2D(center.m_x - (float)(halfWidth / 2), center.m_y + (float)(halfHeight / 2)), halfWidth, halfHeight),
					this);
		}
	}
}

//=============================================================================
// Function: const int getChildIndex(const Vector2D&) const
// Description:
// Checks all of the children nodes to see if a point is inside them.
// If there are no children, it returns the base node.
// Parameters:
// const Vector2D& point - The point to find.
// Output:
// const int
// Returns the child node index of the point.
//=============================================================================
const int Quadtree::getChildIndex(const Vector2D& point) const
{
	if (m_children[0] != nullptr)
	{
		for (int i = 0; i <= CHILD_SW; i++)
		{
			if (pointInRect(m_children[i]->m_bounds, point))
			{
				return i;
			}
		}
	}

	return -1;
}