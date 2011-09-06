/*
 * stack.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef STACK_H
#define STACK_H

namespace coroutine {
	namespace details {

		template <typename B, typename D>
			struct is_base_of {
				typedef char yes;
				struct no { yes _[2]; };

				static yes check(B*);
				static no check(...);

				static const bool value
					= sizeof check((D*)0) == sizeof (yes);
			};

	} // namespace details

	namespace stack {

		struct size_tag {};

		template <size_t V>
			struct size_in_b: size_tag {
				static const size_t value = V;
			};

		template <size_t V>
			struct size_in_kb: size_tag {
				static const size_t value = V * 1024;
			};

		template <size_t V>
			struct size_in_mb: size_tag {
				static const size_t value = V * 1024 * 1024;
			};

		template <typename T>
			struct is_size {
				static const bool value
					= details::is_base_of<size_tag, T>::value;
			};

		struct stack_tag {
			// Technically, stack object have to be moveable to allow retuning
			// coroutine from function, using a nice coroutine builder and so
			// on. However, at soon as the coroutine is started the stack
			// CANNOT move because some code is using it. An exception will be
			// threw if you try to move an already running one. However, if a
			// stack is allocated dynamically, it's possible to move the object
			// encapsulating it while keeping the same physical stack. For any
			// stack that can be really moved, set this attribute to true to
			// access the liberty of free moving running coroutines.
			static const bool really_moveable = false;
		};

		template <typename TAG, size_t SSIZE>
			class stack;

		template <typename T>
			struct is {
				static const bool value
					= details::is_base_of<stack_tag, T>::value;
			};

		namespace details {

			template <typename T>
				struct get_tag;

			template <typename TAG, size_t SSIZE>
				struct get_tag< stack<TAG, SSIZE> > {
					typedef TAG type;
				};

		} // namespace details

		template <typename T>
			struct is_really_moveable {
				static const bool value
					= details::get_tag<T>::type::really_moveable;
			};

	} // namespace stack
} // namespace coroutine

#endif /* STACK_H */
