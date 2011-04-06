/*
 * yielder.hpp
 * Copyright © 2011, François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef YIELDER_H
#define YIELDER_H

namespace coroutine {

	/*
template<
	typename                R,
	typename                F,
	size_t                  SSIZE,
	template <size_t> class S,
	template <class>  class C
	>
class Coroutine; */

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

		YielderBase(const YielderBase& from): _cb(from._cb), _ptr(from._ptr) {}

	protected:
		cb_t  _cb;
		void* _ptr;

#ifndef    YIELDER_TEST_MODE
	private:
#endif //! YIELDER_TEST_MODE
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

} // namespace coroutines

#endif /* YIELDER_H */
