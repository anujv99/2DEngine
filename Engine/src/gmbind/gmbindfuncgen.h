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

/*
	GET/SET DOT OPERATORS
*/

// SETDOT FUNCTION GEN
#define GM_SETDOT_FUNC_BEGIN(TYPE)																			\
	void GM_CDECL gmfSetDot(gmThread * a_thread, gmVariable * a_operands) {									\
		GM_OP_USER_PTR(TYPE, 0, ptr);																		\
		GM_OP_STR_PTR(memStr, 2);																			\
		if (false) {}
#define GM_SETDOT_FUNC_END(TYPE)																			\
		else { a_thread->GetMachine()->GetLog().LogEntry( "Setting invalid property %s", memStr ); }		\
	}

// GETDOT FUNCTION GEN
#define GM_GETDOT_FUNC_BEGIN(TYPE)																			\
	void GM_CDECL gmfGetDot(gmThread * a_thread, gmVariable * a_operands) {									\
		GM_OP_USER_PTR(TYPE, 0, ptr);																		\
		GM_OP_STR_PTR(memStr, 1);																			\
		if (false) {}
#define GM_GETDOT_FUNC_END(TYPE)																			\
		else { a_operands[0].Nullify(); }																	\
	}
		

//SETDOT PARAMS
#define GM_SETDOT_PARAM_USER_PTR(NAME, PARAM)		else if (strcmp(memStr, NAME) == 0) { GM_OP_USER_PTR(result, 1); ptr->PARAM = result;	}
#define GM_SETDOT_PARAM_STR(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { GM_OP_STR_PTR(result, 1); ptr->PARAM = result;	}
#define GM_SETDOT_PARAM_INT(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { GM_OP_INT(result, 1); ptr->PARAM = result;		}
#define GM_SETDOT_PARAM_FLOAT(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { GM_OP_FLOAT(result, 1); ptr->PARAM = result;		}
#define GM_SETDOT_PARAM_VEC4(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { GM_OP_VEC4(result, 1); ptr->PARAM = result;		}
#define GM_SETDOT_PARAM_VEC3(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { GM_OP_VEC3(result, 1); ptr->PARAM = result;		}
#define GM_SETDOT_PARAM_VEC2(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { GM_OP_VEC2(result, 1); ptr->PARAM = result;		}
#define GM_SETDOT_PARAM_VEC2I(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { GM_OP_VEC2I(result, 1); ptr->PARAM = result;		}

#define GM_SETDOT_PARAM_CUSTOM(NAME, OPERATION)		else if (strcmp(memStr, NAME) == 0) { OPERATION; }

//GETDOT PARAMS
#define GM_GETDOT_PARAM_USER_PTR(NAME, PARAM)		else if (strcmp(memStr, NAME) == 0) { a_operands[0].SetUser(ptr->PARAM);	}
#define GM_GETDOT_PARAM_STR(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { a_operands[0].SetString(ptr->PARAM);	}
#define GM_GETDOT_PARAM_INT(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { a_operands[0].SetInt(ptr->PARAM);		}
#define GM_GETDOT_PARAM_FLOAT(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { a_operands[0].SetFloat(ptr->PARAM);	}
#define GM_GETDOT_PARAM_VEC4(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { a_operands[0].SetVec4(ptr->PARAM);	}
#define GM_GETDOT_PARAM_VEC3(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { a_operands[0].SetVec3(ptr->PARAM);	}
#define GM_GETDOT_PARAM_VEC2(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { a_operands[0].SetVec2(ptr->PARAM);	}
#define GM_GETDOT_PARAM_VEC2I(NAME, PARAM)			else if (strcmp(memStr, NAME) == 0) { a_operands[0].SetVec2i(ptr->PARAM);	}

#define GM_GETDOT_PARAM_CUSTOM(NAME, OPERATION)		else if (strcmp(memStr, NAME) == 0) { OPERATION; }

// REGISTER GET/SET DOT FUNCTIONS

#define _GM_REG_MEMOP_HELPER(FUNC, OPERATOR) a_machine->RegisterTypeOperator(gmTypeId, OPERATOR, nullptr, FUNC);
#define GM_REG_SETDOT_FUNC(CLASS) _GM_REG_MEMOP_HELPER(gmf##CLASS::gmfSetDot, O_SETDOT)
#define GM_REG_GETDOT_FUNC(CLASS) _GM_REG_MEMOP_HELPER(gmf##CLASS::gmfGetDot, O_GETDOT)
#define GM_REG_SET_GET_DOT_FUNC(CLASS) GM_REG_SETDOT_FUNC(CLASS) GM_REG_GETDOT_FUNC(CLASS)