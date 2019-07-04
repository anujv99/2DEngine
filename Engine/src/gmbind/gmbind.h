#pragma once

#include "gm/gmMachine.h"
#include "gm/gmThread.h"


#define GM_BIND_INIT(TYPE, MACHINE) gmf##TYPE::gmBind##TYPE##Lib(MACHINE);

#define GM_TYPE_CLASS gmfUserType

//HEADER
#define GM_BIND_TYPEID(TYPE) class GM_TYPE_CLASS { public: static int s_gmUserTypeId; };
#define GM_BIND_DECL(TYPE) void gmBind##TYPE##Lib(gmMachine * a_machine);
//CPP
#define GM_TYPEID(TYPE) (gmf##TYPE::GM_TYPE_CLASS::s_gmUserTypeId)

#define GM_REG_NAMESPACE(TYPE) namespace gmf##TYPE
#define GM_MEMFUNC_DECL(FUNC_NAME) static int GM_CDECL gmf##FUNC_NAME(gmThread * a_thread)
#define GM_REG_MEMFUNC(CLASS, FUNC_NAME) a_machine->RegisterTypeLibraryFunction(gmTypeId, #FUNC_NAME, gmf##CLASS::gmf##FUNC_NAME);

#define GM_PUSH_USER(TYPE, PTR) { TYPE * _ptr = (PTR); GM_THREAD_ARG->PushNewUser(_ptr, GM_TYPEID(TYPE)); }
#define GM_PUSH_USER_HANDLED(TYPE, PTR) { TYPE * _ptr = (PTR); _ptr->AddRef(); GM_THREAD_ARG->PushNewUser(_ptr, GM_TYPEID(TYPE)); }

#define GM_BIND_DEFINE( TYPE )							\
	void gmf##TYPE::gmBind##TYPE##Lib( gmMachine * a_machine ) {	\
		gmBindUser##TYPE##Type(a_machine);				\
		gmBindUser##TYPE##LibFuncs(a_machine);			\
	}

//default member begin
#define GM_REG_MEM_BEGIN(TYPE)																			\
	void gmBindUser##TYPE##Type(gmMachine * a_machine) {												\
		gmFunctionEntry gm##TYPE##ConstructorEntry[] = { #TYPE, gmf##TYPE::gmfCreate##TYPE, 0 };		\
		a_machine->RegisterLibrary(gm##TYPE##ConstructorEntry, 1);										\
		GM_TYPEID(TYPE) = a_machine->CreateUserType(#TYPE);												\
	}																									\
	int gmf##TYPE::GM_TYPE_CLASS::s_gmUserTypeId = 0;													\
	void gmBindUser##TYPE##LibFuncs(gmMachine * a_machine) {											\
		gmType gmTypeId = GM_TYPEID(TYPE);

#define GM_REG_MEM_END(TYPE) }

//destructor
#define GM_REG_HANDLED_DESTRUCTOR(TYPE) a_machine->RegisterUserCallbacks(GM_TYPEID(TYPE), GM_DEFAULT_TRACE_FUNC(TYPE), GM_DESTRUCT_STRONG_HANDLED(TYPE), nullptr);

template<class T>
void GM_CDECL gmDestructStrongHandled(gmMachine * a_machine, gmUserObject * a_object) {
	GM_ASSERT(a_object->m_userType == GM_TYPEID(T));
	T * p = (T *)(a_object->m_user);
	p->ReleaseRef();
	if (p->RefCount() == 0) delete p;
}

#define GM_DESTRUCT_STRONG_HANDLED(TYPE)\
	[](gmMachine * a_machine, gmUserObject * a_object) {\
		GM_ASSERT(a_object->m_userType == GM_TYPEID(TYPE));\
		TYPE * p = (TYPE *)(a_object->m_user);\
		p->ReleaseRef();\
		if (p->RefCount() == 0) delete p;\
	}

#define GM_DEFAULT_TRACE_FUNC(TYPE)\
	[](gmMachine * a_machine, gmUserObject * a_object, gmGarbageCollector * a_gc,\
	const int a_workLeftToGo, int & a_workDone) -> bool {\
		/*Make sure this is the correct type*/\
		GM_ASSERT(a_object->m_userType == GM_TYPEID(TYPE));\
		return true;\
	}


#define GM_GET_THIS_PTR(TYPE, PTR) TYPE * PTR = (TYPE *)a_thread->ThisUserCheckType(GM_TYPEID(TYPE)); 