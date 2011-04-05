/*
 * include/coroutine.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef COROUTINE_H
#define COROUTINE_H

#include <context.hpp>

namespace coroutine {

template <typename T, typename R = void>
class Yielder
{
	public:
		R operator()(T value)
		{
		}

	private:
		typedef R (cb_t)(void*, T);

		Yielder(const Yielder& from); // disabled
		Yielder& operator=(const Yielder& from); // disabled
	
		Yielder(cb_t* cb, void* ptr): _cb(cb), _ptr(ptr) {}
};

class Coroutine
{
	public:
		
	private:
};

} // namespace coroutine

#endif /* COROUTINE_H */
