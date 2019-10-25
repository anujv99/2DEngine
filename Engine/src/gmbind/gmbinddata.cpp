#include "pch.h"
#include "gmbinddata.h"

#include "gmbind.h"
#include "gmbindgame.h"
#include "utils/linkedlist.h"

using namespace prev;

class gmLinkedList : public LinkedList<gmVariable> {
public:
	gmLinkedList(gmMachine * a_machine) : m_Machine(a_machine) {
		ASSERT(a_machine);
	}

	bool Trace(gmMachine * a_machine, gmGarbageCollector * a_gc, const int a_workLeftToGo, int & a_workDone) {
		int i;
		Node * node = m_Tail;
		while (node != nullptr) {
			if (node->Data->IsReference()) {
				gmObject * object = GM_MOBJECT(a_machine, node->Data->m_value.m_ref);
				a_gc->GetNextObject(object);
				++a_workDone;
			}
			node = node->Next;
		}

		++a_workDone;
		return true;
	}
protected:
	virtual gmVariable * Allocate() override {
		return (gmVariable *)m_Machine->Sys_Alloc((int)sizeof(gmVariable));
	}

	virtual void Free(gmVariable * elem) override {
		ASSERT(elem);
		m_Machine->Sys_Free(elem);
	}
private:
	gmMachine * m_Machine;
};

GM_REG_NAMESPACE(queue) {

	using queue = std::queue<gmVariable>;

	GM_MEMFUNC_DECL(Createqueue) {
		GM_CHECK_NUM_PARAMS(0);
		GM_PUSH_USER(queue, new queue);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Push) {
		GM_CHECK_NUM_PARAMS(1);
		gmVariable & var = a_thread->Param(0);
		GM_GET_THIS_PTR(queue, ptr);
		ptr->push(var);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Pop) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(queue, ptr);
		gmVariable var = ptr->front();
		ptr->pop();
		a_thread->Push(var);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Size) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(queue, ptr);
		a_thread->PushInt((int)ptr->size());
		return GM_OK;
	}

	GM_REG_MEM_BEGIN(queue)
		GM_REG_DESTRUCTOR(queue)
		GM_REG_MEMFUNC(queue, Push)
		GM_REG_MEMFUNC(queue, Pop)
		GM_REG_MEMFUNC(queue, Size)
	GM_REG_MEM_END(queue)

	GM_BIND_DEFINE(queue)
}

GM_REG_NAMESPACE(linkedlist) {

	using linkedlist = gmLinkedList;

	GM_MEMFUNC_DECL(Createlinkedlist) {
		GM_CHECK_NUM_PARAMS(0);
		GM_PUSH_USER(linkedlist, new linkedlist(a_thread->GetMachine()));
		return GM_OK;
	}

	GM_MEMFUNC_DECL(PushFront) {
		GM_CHECK_NUM_PARAMS(1);
		gmVariable & var = a_thread->Param(0);
		GM_GET_THIS_PTR(linkedlist, ptr);
		ptr->PushFront(var);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(PushBack) {
		GM_CHECK_NUM_PARAMS(1);
		gmVariable & var = a_thread->Param(0);
		GM_GET_THIS_PTR(linkedlist, ptr);
		ptr->PushBack(var);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(PopFront) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(linkedlist, ptr);
		gmVariable var = ptr->Front();
		ptr->PopFront();
		a_thread->Push(var);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(PopBack) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(linkedlist, ptr);
		gmVariable var = ptr->Back();
		ptr->PopBack();
		a_thread->Push(var);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Front) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(linkedlist, ptr);
		gmVariable var = ptr->Front();
		a_thread->Push(var);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Back) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(linkedlist, ptr);
		gmVariable var = ptr->Back();
		a_thread->Push(var);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Size) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(linkedlist, ptr);
		a_thread->PushInt((int)ptr->Size());
		return GM_OK;
	}


	static bool GM_CDECL gmGCTraceLinkedListUserType(gmMachine * a_machine, gmUserObject * a_object, gmGarbageCollector * a_gc, const int a_workLeftToGo, int & a_workDone) {
		if (a_object->m_user) {
			linkedlist * list = (linkedlist *)a_object->m_user;
			return list->Trace(a_machine, a_gc, a_workLeftToGo, a_workDone);
		}
		return true;
	}

	static void GM_CDECL gmGCDestructLinkedListUserType(gmMachine * a_machine, gmUserObject * a_object) {
		if (a_object->m_user) {
			linkedlist * list = (linkedlist *)a_object->m_user;
			list->Destruct();
			delete list;
		}
		a_object->m_user = NULL;
	}

	GM_REG_MEM_BEGIN(linkedlist)
		a_machine->RegisterUserCallbacks(GM_TYPEID(linkedlist), gmGCTraceLinkedListUserType, gmGCDestructLinkedListUserType);
		GM_REG_MEMFUNC(linkedlist, PushFront)
		GM_REG_MEMFUNC(linkedlist, PushBack)
		GM_REG_MEMFUNC(linkedlist, PopFront)
		GM_REG_MEMFUNC(linkedlist, PopBack)
		GM_REG_MEMFUNC(linkedlist, Front)
		GM_REG_MEMFUNC(linkedlist, Back)
		GM_REG_MEMFUNC(linkedlist, Size)
	GM_REG_MEM_END(linkedlist)

	GM_BIND_DEFINE(linkedlist)

}
