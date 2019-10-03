#pragma once

#include "common/assert.h"

namespace prev {

	//Doubly linked list
	template<class T>
	class LinkedList final {
	public:
		LinkedList();
		LinkedList(const LinkedList & list) = delete;
		~LinkedList();
	public:
		void PushFront(const T & elem);
		void PushBack(const T & elem);

		void PopFront();
		void PopBack();

		inline T Front() { ASSERT(m_Size); return (m_Head->Data); }
		inline T Back() { ASSERT(m_Size); return (m_Tail->Data); }

		inline size_t Size() { return m_Size; }

	private:
		struct Node {
			T Data;
			Node * Next = nullptr;
			Node * Prev = nullptr;
		};

		Node * m_Head;
		Node * m_Tail;
		size_t m_Size;
	};

	template<class T>
	void prev::LinkedList<T>::PopBack() {
		ASSERT(m_Size);
		if (m_Size > 1) {
			m_Tail = m_Tail->Next;
			delete m_Tail->Prev;
			m_Tail->Prev = nullptr;
		} else {
			delete m_Tail;
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
			delete m_Head->Next;
			m_Head->Next = nullptr;
		} else {
			delete m_Head;
			m_Tail = nullptr;
			m_Head = nullptr;
		}

		m_Size--;
	}

	template<class T>
	void prev::LinkedList<T>::PushBack(const T & elem) {
		Node * node = new Node;
		ASSERT(node);
		node->Data = elem;
		
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
		Node * node = new Node;
		ASSERT(node);
		node->Data = elem;

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
		Node * temp = m_Tail;
		if (temp != nullptr) {
			if (temp->Next != nullptr) {
				temp = temp->Next;
				delete temp->Prev;
			} else {
				delete temp;
			}
		}
		m_Head = nullptr;
		m_Tail = nullptr;
		m_Size = 0;
	}

	template<class T>
	prev::LinkedList<T>::LinkedList() : m_Head(nullptr), m_Tail(nullptr), m_Size(0) {}

}
