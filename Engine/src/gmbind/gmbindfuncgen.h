#pragma once

#define GM_LIBFUNC_ENTRY(FUNC, CLASS) { #FUNC, gmf##CLASS##Lib::gmf##FUNC },

#define GM_LIBFUNC_SIZE(ENTRY) sizeof(ENTRY) / sizeof(ENTRY[0])