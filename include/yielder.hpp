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

/*
 * FV stand for Feed Value, the value feeded to the coroutine for each iteration.
 * RV stand for Return Value, the value that the coroutine return for each iteration.
 */

namespace details {

template <typename RV, typename FV>
struct cb_type { typedef FV (*type)(void*, RV); };

template <typename FV>
struct cb_type<void, FV> { typedef FV (*type)(void*); };

} // namespace details

template <typename RV, typename FV>
class YielderBase
{
	public:
		typedef typename details::cb_type<RV, FV>::type cb_t;

		YielderBase(const YielderBase& from):
			_cb(from._cb), _ptr(from._ptr) {}

	protected:
		cb_t  _cb;
		void* _ptr;
		
		YielderBase(cb_t cb, void* ptr):
			_cb(cb), _ptr(ptr) { }
	
	private:
		YielderBase& operator=(const YielderBase& from); // disabled
};

// RV f(FV feedValue)
template <typename RV = void, typename FV = void>
class Yielder: public YielderBase<RV, FV>
{
	typedef YielderBase<RV, FV>   base_t;
	typedef typename base_t::cb_t cb_t;

	public:
		FV operator()(RV value) const {
			return this->_cb(this->_ptr, value);
		}
		
#ifndef    YIELDER_TEST_MODE
	protected:
#endif //! YIELDER_TEST_MODE
		Yielder(cb_t cb, void* ptr): base_t(cb, ptr) { }
};

// RV f()
template <typename RV>
class Yielder<RV, void>: public YielderBase<RV, void>
{
	typedef YielderBase<RV, void> base_t;
	typedef typename base_t::cb_t cb_t;

	public:
		void operator()(RV value) const {
			this->_cb(this->_ptr, value);
		}
		
#ifndef    YIELDER_TEST_MODE
	protected:
#endif //! YIELDER_TEST_MODE
		Yielder(cb_t cb, void* ptr): base_t(cb, ptr) { }
};

// void f(FV feedValue)
template <typename FV>
class Yielder<void, FV>: public YielderBase<void, FV>
{
	typedef YielderBase<void, FV> base_t;
	typedef typename base_t::cb_t cb_t;

	public:
		FV operator()() const {
			return this->_cb(this->_ptr);
		}
		
#ifndef    YIELDER_TEST_MODE
	protected:
#endif //! YIELDER_TEST_MODE
		Yielder(cb_t cb, void* ptr): base_t(cb, ptr) { }
};

// void f()
template <>
class Yielder<void, void>: public YielderBase<void, void>
{
	typedef YielderBase<void, void> base_t;
	typedef typename base_t::cb_t   cb_t;

	public:
		void operator()() const {
			this->_cb(this->_ptr);
		}
		
#ifndef    YIELDER_TEST_MODE
	protected:
#endif //! YIELDER_TEST_MODE
		Yielder(cb_t cb, void* ptr): base_t(cb, ptr) { }
};

} // namespace coroutines

#endif /* YIELDER_H */
