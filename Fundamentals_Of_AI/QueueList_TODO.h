//! \file QueueList_TODO.h
//! \brief Defines the <code>fullsail_ai::data_structures::QueueList</code> class template
//! implementation.
//! \author Cromwell D. Enage
#ifndef _FULLSAIL_AI_FUNDAMENTALS_QUEUELIST_TODO_H_
#define _FULLSAIL_AI_FUNDAMENTALS_QUEUELIST_TODO_H_

namespace fullsail_ai { namespace fundamentals {

	//! \TODO
	//!   - Complete the definition of the <code>QueueList::Iterator</code> pre-increment operator.
	//!   - Complete the definition of the <code>QueueList::Iterator</code> dereference operator.
	//!   - Define the <code>QueueList</code> constructor.
	//!   - Define the <code>QueueList</code> destructor.
	//!   - Define the <code>QueueList</code> methods.

	template<typename T>
	QueueList<T>::QueueList()
	{
		front = back = nullptr;
	}

	template<typename T>
	QueueList<T>::~QueueList()
	{
		removeAll();
	}


	template <typename T>
	typename QueueList<T>::Iterator& QueueList<T>::Iterator::operator++()
	{
		// TODO: Complete the definition of this operator.

		if (currentNode == nullptr)
			return *this;
		else
			currentNode = currentNode->next;

		return *this;
	}

	template <typename T>
	T QueueList<T>::Iterator::operator*() const
	{
		// TODO: Complete the definition of this operator.
		return currentNode->data;
	}

	template <typename T>
	bool QueueList<T>::isEmpty() const
	{
		return (front == nullptr && back == nullptr);
	}

	template<typename T>
	void QueueList<T>::enqueue(T element)
	{
		Node* newNode = new Node(element);
		newNode->data = element;
		newNode->next = nullptr;

		if (isEmpty())
		{
			front = newNode;
			back = newNode;
		}
		else
			back->next = newNode;

		back = newNode;
	}

	template<typename T>
	T QueueList<T>::getFront() const
	{
		return front->data;
	}

	template<typename T>
	void QueueList<T>::dequeue()
	{
		Node* newNode;
		if (front == nullptr)
			return;
		if (front == back)
		{
			delete front;
			front = nullptr;
			back = nullptr;
		}
		else
		{
			newNode = front;
			front = front->next;
			delete newNode;
		}
	}

	template<typename T>
	void QueueList<T>::removeAll()
	{
		Node* newNode = front;
		while (newNode)
		{
			Node* temp = newNode;
			newNode = newNode->next;
			delete temp;
		}
		front = back = nullptr;
	}

	template<typename T>
	bool QueueList<T>::contains(T element) const
	{
		Node* walkingNode = front;
		while (walkingNode)
		{
			if (walkingNode->data == element)
				return true;
			else
				walkingNode = walkingNode->next;
		}
		return false;
	}

	template<typename T>
	void QueueList<T>::remove(T element)
	{
		if (contains(element))
		{
			if (front == back)
			{
				delete front;
				front = back = nullptr;
				return;
			}
			if (front->data == element)
			{
				Node* newNode = front;
				front = front->next;
				delete newNode;
				return;
			}

			Node* walkingNode = front->next;
			Node* prev = front;

			while (walkingNode)
			{
				if (walkingNode->data == element)
				{
					if (walkingNode == back)
					{
						prev->next = nullptr;
						back = prev;
						delete walkingNode;
						return;
					}
					else
					{
						Node* temp = walkingNode;
						prev->next = walkingNode->next;
						delete temp;
						return;
					}
				}
				prev = walkingNode;
				walkingNode = walkingNode->next;
			}
		}
	}

}}  // namespace fullsail_ai::fundamentals

#endif  // _FULLSAIL_AI_FUNDAMENTALS_QUEUELIST_TODO_H_

