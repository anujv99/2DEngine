#include "pch.h"
#include "gmbinddata.h"

#include "gmbind.h"
#include "gmbindgame.h"
#include "utils/linkedlist.h"

using namespace prev;

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

	using linkedlist = LinkedList<gmVariable>;

	GM_MEMFUNC_DECL(Createlinkedlist) {
		GM_CHECK_NUM_PARAMS(0);
		GM_PUSH_USER(linkedlist, new linkedlist);
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

	GM_REG_MEM_BEGIN(linkedlist)
		GM_REG_DESTRUCTOR(linkedlist)
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