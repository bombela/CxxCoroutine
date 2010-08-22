/*
 * posixContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#ifndef INCLUDED

#define CAT(a, b) a##b

#define INCLUDED _static_
#define __BENCH(n, p) _BENCH(staticStack_##n, p)
#define CONTEXT Context<stack::Static>
#include "context.cpp.inc"
#undef CONTEXT
#undef __BENCH
#undef INCLUDED

#define INCLUDED _dynamic
#define __BENCH(n, p) _BENCH(dynamicStack_##n, p)
#define CONTEXT Context<stack::Dynamic>
#include "context.cpp.inc"
#undef CONTEXT
#undef __BENCH
#undef INCLUDED

#else

#include <benchmark.hpp>

namespace INCLUDED {

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
