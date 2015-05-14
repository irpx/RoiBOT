#pragma once

#ifdef _DEBUG

#define LOG(fmt, ...) printf(" LOG  "); printf(fmt, __VA_ARGS__); printf("\n")
#include <intrin.h>
#define ASSERT(expr,fmt,...)if(!epxr){LOG("\n #### ASSERT ####");LOG(fmt,__VA_ARGS__);__debugbreak();}

#else

#define LOG(fmt,...)
#define ASSERT(expr,fmt,...)

#endif

