//! \file Behavior_TODO.cpp
//! \brief Implements the <code>fullsail_ai::fundamentals::Behavior</code> abstract class.
//! \author Jeremiah Blanchard with code from Cromwell D. Enage

// #include <queue>   NOTE: Only use if your QueueList is not working
#include <stack>
#include "Behavior.h"
#include "../QueueList//QueueList.h"

namespace fullsail_ai { namespace fundamentals {

	// DO NOT Edit or Use these two variables for any reason, it is for memory leak check purposes only.
	unsigned Behavior::createdCount = 0;
	unsigned Behavior::destroyedCount = 0;

	unsigned Behavior::getCreatedCount()
	{
		return createdCount;
	}

	unsigned Behavior::getDestroyedCount()
	{
		return destroyedCount;
	}

	Behavior::Behavior(char const* _description)
	{
		description = _description;
	}

	char const* Behavior::toString() const
	{
		return description;
	}

	// DO NOT edit this function, it is supposed to just return false
	bool Behavior::isLeaf() const
	{
		return false;
	}

	//! \TODO
	//!   - Implement the <code>Behavior</code> methods.
	DLLEXPORT size_t Behavior::getChildCount() const
	{
		return children.size();
	}

	DLLEXPORT Behavior * Behavior::getChild(size_t index)
	{
		return children[index];
	}

	DLLEXPORT Behavior const * Behavior::getChild(size_t index)const
	{
		return (children[index]);
	}

	DLLEXPORT void Behavior::addChild(Behavior *child)
	{
		child->parent = this;
		return children.push_back(child);
	}

	DLLEXPORT void Behavior::breadthFirstTraverse(void(*dataFunction)(Behavior const*)) const
	{
		QueueList<Behavior const*>queue;

		queue.enqueue(this);

		while (!queue.isEmpty())
		{
			Behavior const* node = queue.getFront();
			dataFunction(node);
			queue.remove(node);

			for (size_t i = 0; i < node->children.size(); i++)
			{
				queue.enqueue(node->children[i]);
			}

		}
	}
	DLLEXPORT void Behavior::preOrderTraverse(void(*dataFunction)(Behavior const*))const
	{
		preOrderHelper(dataFunction, this);
	}

	void Behavior::preOrderHelper(void(*dataFunction)(Behavior const*), Behavior const* node)const
	{
		dataFunction(node);
		for (size_t i = 0; i < node->children.size(); i++)
		{
			preOrderHelper(dataFunction, node->children[i]);
		}
	}
	DLLEXPORT void Behavior::postOrderTraverse(void(*dataFunction)(Behavior const*))const
	{
		postOrderHelper(dataFunction, this);
	}
	void Behavior::postOrderHelper(void(*dataFunction)(Behavior const *), Behavior const* node)const
	{
		for (size_t i = 0; i < node->children.size(); i++)
		{
			postOrderHelper(dataFunction, node->children[i]);
		}
		dataFunction(node);
	}

}}  // namespace fullsail_ai::fundamentals
