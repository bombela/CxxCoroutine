/*
 * posixContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#ifndef INCLUDED

#define CAT(a, b) a##b

#define INCLUDED static_
#define __BENCH(n, p) _BENCH(static_##n, p)
#define CONTEXT Context<stack::Static>
#include "context.cpp.inc"
#undef CONTEXT
#undef __BENCH
#undef INCLUDED

#define INCLUDED dynamic_
#define __BENCH(n, p) _BENCH(dynamic_##n, p)
#define CONTEXT Context<stack::Dynamic>
#include "context.cpp.inc"
#undef CONTEXT
#undef __BENCH
#undef INCLUDED

#else

#include <benchmark.hpp>

namespace INCLUDED {

__BENCH(nothing, 23)
{
}

void function() {}

__BENCH(createContext, 23)
{
	UNUSED CONTEXT context(function);
}

__BENCH(execContext, 23)
{
	UNUSED CONTEXT context(function);
	context.run();
}

} // namespace 

#endif
