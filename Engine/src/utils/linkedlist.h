#pragma once

#include "common/assert.h"

namespace prev {

	//Doubly linked list
	template<class T>
	class LinkedList {
	public:
		LinkedList();
		LinkedList(const LinkedList & list) = delete;
		~LinkedList();

		void Destruct();
	public:
		void PushFront(const T & elem);
		void PushBack(const T & elem);

		void PopFront();
		void PopBack();

		inline T Front() { ASSERT(m_Size); return (*(m_Head->Data)); }
		inline T Back() { ASSERT(m_Size); return (*(m_Tail->Data)); }

		inline size_t Size() { return m_Size; }
	protected:
		virtual T * Allocate();
		virtual void Free(T * elem);

		struct Node {
			T * Data;
			Node * Next = nullptr;
			Node * Prev = nullptr;
		};

		Node * m_Head;
		Node * m_Tail;
		size_t m_Size;
	private:
		Node * CreateNode();
		void DeleteNode(Node * node);
	};

	template<class T>
	void prev::LinkedList<T>::Destruct() {
		Node * temp = m_Tail;
		while (temp != nullptr) {
			if (temp->Next != nullptr) {
				temp = temp->Next;
				DeleteNode(temp->Prev);
			} else {
				DeleteNode(temp);
				break;
			}
		}
		m_Head = nullptr;
		m_Tail = nullptr;
		m_Size = 0;
	}

	template<class T>
	typename prev::LinkedList<T>::Node * prev::LinkedList<T>::CreateNode() {
		Node * node = new Node();
		ASSERT(node);
		node->Data = Allocate();
		return node;
	}

	template<class T>
	void prev::LinkedList<T>::DeleteNode(Node * node) {
		ASSERT(node);
		Free(node->Data);
		delete node;
	}

	template<class T>
	void prev::LinkedList<T>::Free(T * elem) {
		ASSERT(elem);
		delete elem;
	}

	template<class T>
	T * prev::LinkedList<T>::Allocate() {
		T * elem = new T;
		ASSERT(elem);
		return elem;
	}

	template<class T>
	void prev::LinkedList<T>::PopBack() {
		ASSERT(m_Size);
		if (m_Size > 1) {
			m_Tail = m_Tail->Next;
			DeleteNode(m_Tail->Prev);
			m_Tail->Prev = nullptr;
		} else {
			DeleteNode(m_Tail);
			m_Tail = nullptr;
			m_Head = nullptr;
		}

		m_Size--;
	}

	template<class T>
	void prev::LinkedList<T>::PopFront() {
		ASSERT(m_Size);
		if (m_Size > 1) {
			m_Head = m_Head->Prev;
			DeleteNode(m_Head->Next);
			m_Head->Next = nullptr;
		} else {
			DeleteNode(m_Head);
			m_Tail = nullptr;
			m_Head = nullptr;
		}

		m_Size--;
	}

	template<class T>
	void prev::LinkedList<T>::PushBack(const T & elem) {
		Node * node = CreateNode();
		ASSERT(node);
		*(node->Data) = elem;
		
		if (m_Tail == nullptr) {
			node->Next = nullptr;
			node->Prev = nullptr;
			m_Head = node;
			m_Tail = node;
			m_Size++;
			return;
		}

		m_Tail->Prev = node;
		node->Next = m_Tail;
		node->Prev = nullptr;
		m_Tail = node;
		m_Size++;
	}

	template<class T>
	void prev::LinkedList<T>::PushFront(const T & elem) {
		Node * node = CreateNode();
		ASSERT(node);
		*(node->Data) = elem;

		if (m_Head == nullptr) {
			node->Next = nullptr;
			node->Prev = nullptr;
			m_Head = node;
			m_Tail = node;
			m_Size++;
			return;
		}

		m_Head->Next = node;
		node->Next = nullptr;
		node->Prev = m_Head;
		m_Head = node;
		m_Size++;
	}

	template<class T>
	prev::LinkedList<T>::~LinkedList() {
		Destruct();
	}

	template<class T>
	prev::LinkedList<T>::LinkedList() : m_Head(nullptr), m_Tail(nullptr), m_Size(0) {}

}
