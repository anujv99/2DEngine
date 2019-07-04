#pragma once

#include "gm/gmThread.h"

/*
	LIBRARY FUNCTIONS
*/
#define GM_LIBFUNC_ENTRY(FUNC, CLASS) { #FUNC, gmf##CLASS##Lib::gmf##FUNC },
#define GM_LIBFUNC_SIZE(ENTRY) sizeof(ENTRY) / sizeof(ENTRY[0])

/*
	GENERATE FUNCTIONS
*/

#define _GM_GEN_MEMFUNC_BEGIN(FUNC_NAME) int GM_CDECL gmf##FUNC_NAME(gmThread * a_thread) {
#define _GM_GEN_MEMFUNC_END(FUNC_NAME) }


// void function(void)
#define GM_GEN_MEMFUNC_VOID_VOID(TYPE, MEMFUNC)				\
	_GM_GEN_MEMFUNC_BEGIN(MEMFUNC)							\
	GM_CHECK_NUM_PARAMS(0);									\
	GM_GET_THIS_PTR(TYPE, ptr);								\
	ptr->MEMFUNC();											\
	return GM_OK;											\
	_GM_GEN_MEMFUNC_END(MEMFUNC)

// void function(const char *)
#define GM_GEN_MEMFUNC_VOID_STRING(TYPE, MEMFUNC)			\
	_GM_GEN_MEMFUNC_BEGIN(MEMFUNC)							\
	GM_CHECK_NUM_PARAMS(1);									\
	GM_CHECK_STRING_PARAM(val, 0);							\
	GM_GET_THIS_PTR(TYPE, ptr);								\
	ptr->MEMFUNC(val);										\
	return GM_OK;											\
	_GM_GEN_MEMFUNC_END(MEMFUNC)

// const char * function(void)
#define GM_GEN_MEMFUNC_STRING_VOID(TYPE, MEMFUNC)			\
	_GM_GEN_MEMFUNC_BEGIN(MEMFUNC)							\
	GM_CHECK_NUM_PARAMS(0);									\
	GM_GET_THIS_PTR(TYPE, ptr);								\
	std::string str = ptr->MEMFUNC();						\
	a_thread->PushNewString(str.c_str());					\
	return GM_OK;											\
	_GM_GEN_MEMFUNC_END(MEMFUNC)