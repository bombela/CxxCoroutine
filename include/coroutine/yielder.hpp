/*
 * yielder.hpp
 * Copyright © 2011, François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef YIELDER_H
#define YIELDER_H

#include <iostream>

/*
 * TODO
 *
 * yield of type yielder is used like that:
 *
 * feed_val = yield(ret_val);
 *
 * It's should possible to provide overload with move semantic.
 * It's maybe overkill because: each side (main code/coroutine code)
 * need to know if the value was yield/feed from an RVALUE
 * to be able to pass it on the other side as an RVALUE too.
 * Adding a simple boolean and a if could do the trick, but I feel
 * it will be slower than simply allowing only copy semantic.
 * After all, if you need to pass value without duplicating it (thats
 * the main purpose of move semantic), simply yield/feed a pointer.
 */

namespace coroutine {

	/*
	 * FV stand for Feed Value, the value feeded to the coroutine for each
	 * iteration.
	 * RV stand for Return Value, the value returned by the coroutine for
	 * each iteration.
	 */

	namespace details {

		template <typename RV, typename FV>
			struct cb_type { typedef FV (*type)(void*, RV); };

		template <typename FV>
			struct cb_type<void, FV> { typedef FV (*type)(void*); };

	} // namespace details

	template <typename RV, typename FV>
		class yielder_base
		{
		public:
			typedef typename details::cb_type<RV, FV>::type cb_t;

			yielder_base(const yielder_base& from):
				_cb(from._cb), _ptr(from._ptr) {
				}

		protected:
			cb_t  _cb;
			void* _ptr;

			yielder_base(cb_t cb, void* ptr):
				_cb(cb), _ptr(ptr) {
				}

		private:
			yielder_base& operator=(const yielder_base& from); // disabled
		};

	template <typename S = void ()>
		class yielder;

	// RV f(FV feedValue)
	template <typename RV, typename FV>
		class yielder<RV (FV)>: public yielder_base<RV, FV>
	{
		typedef yielder_base<RV, FV>  base_t;
		typedef typename base_t::cb_t cb_t;

		public:
		FV operator()(RV value) const {
			return this->_cb(this->_ptr, value);
		}

		yielder(cb_t cb, void* ptr): base_t(cb, ptr) { }
	};

	// RV f()
	template <typename RV>
		class yielder<RV ()>: public yielder_base<RV, void>
		{
			typedef yielder_base<RV, void> base_t;
			typedef typename base_t::cb_t cb_t;

			public:
			void operator()(RV value) const {
				this->_cb(this->_ptr, value);
			}

			yielder(cb_t cb, void* ptr): base_t(cb, ptr) { }
		};

	// void f(FV feedValue)
	template <typename FV>
		class yielder<void (FV)>: public yielder_base<void, FV>
		{
			typedef yielder_base<void, FV> base_t;
			typedef typename base_t::cb_t cb_t;

			public:
			FV operator()() const {
				return this->_cb(this->_ptr);
			}

			yielder(cb_t cb, void* ptr): base_t(cb, ptr) { }
		};

	// void f()
	template <>
		class yielder<void ()>: public yielder_base<void, void>
		{
			typedef yielder_base<void, void> base_t;
			typedef typename base_t::cb_t   cb_t;

			public:
			void operator()() const {
				this->_cb(this->_ptr);
			}

			yielder(cb_t cb, void* ptr): base_t(cb, ptr) { }
		};

} // namespace coroutines

#endif /* YIELDER_H */
