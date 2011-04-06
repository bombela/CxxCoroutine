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

namespace details {
	
template <typename T, typename R>
struct cb_type { typedef R (*type)(void*, T); };

template <typename R>
struct cb_type<void, R> { typedef R (*type)(void*); };

} // namespace details

template <typename T, typename R>
class YielderBase
{
	public:
		typedef typename details::cb_type<T, R>::type cb_t;

#ifndef    YIELDER_TEST_MODE
	private:
#endif //! YIELDER_TEST_MODE
		cb_t  _cb;
		void* _ptr;

		YielderBase(const YielderBase& from); // disabled
		YielderBase& operator=(const YielderBase& from); // disabled
	
		YielderBase(cb_t cb, void* ptr): _cb(cb), _ptr(ptr) { }
};

template <typename T = void, typename R = void>
class Yielder: public YielderBase<T, R>
{
	typedef YielderBase<T, R>     base_t;
	typedef typename base_t::cb_t cb_t;

	public:
		R operator()(T value) const {
			return this->_cb(this->_ptr, value);
		}
		
#ifndef    YIELDER_TEST_MODE
	private:
#endif //! YIELDER_TEST_MODE
		Yielder(cb_t cb, void* ptr): base_t(cb, ptr) { }
};

template <typename R>
class Yielder<void, R>: public YielderBase<void, R>
{
	typedef YielderBase<void, R>  base_t;
	typedef typename base_t::cb_t cb_t;

	public:
		R operator()() const {
			return this->_cb(this->_ptr);
		}
		
#ifndef    YIELDER_TEST_MODE
	private:
#endif //! YIELDER_TEST_MODE
		Yielder(cb_t cb, void* ptr): base_t(cb, ptr) { }
};


class Coroutine
{
	public:
		
	private:
};

} // namespace coroutine

#endif /* COROUTINE_H */
