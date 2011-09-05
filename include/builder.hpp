/*
 * builder.hpp
 * Copyright © 2011 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef BUILDER_H
#define BUILDER_H

#include <coroutine.hpp>
#include <yielder.hpp>

namespace coroutine {

	namespace details {

		template <typename B, typename D>
			struct is_base_of {
				typedef char yes;
				struct no { yes _[2]; };

				static yes check(B*);
				static no check(...);

				static const bool value = sizeof check((D*)0) == sizeof (yes);
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

		struct stack_tag {};

		template <typename TAG, size_t SSIZE>
			struct stack;

		struct dynamic: stack_tag {};
		template <size_t SSIZE>
			struct stack<dynamic, SSIZE> {
				static const size_t size = SSIZE;
			};

		struct static_: stack_tag {};
		template <size_t SSIZE>
			struct stack<static_, SSIZE> {
				static const size_t size = SSIZE;
			};

		template <typename T>
			struct is {
				static const bool value
					= details::is_base_of<stack_tag, T>::value;
			};

	} // namespace stack

	namespace context {
		struct context_tag {
			typedef stack::dynamic default_stack;
		};

		template <typename TAG, class STACK>
			struct context;

		struct posix: context_tag {
			template <typename STACK>
				struct default_stack_size: stack::size_in_mb<24> {};
		};

		template <class STACK>
			struct context<posix, STACK> {
			};

		struct windows: context_tag {
			template <typename STACK>
				struct default_stack_size: stack::size_in_mb<64> {};
		};

		template <class STACK>
			struct context<windows, STACK> {
			};

		struct linux_x86_64: context_tag {
			typedef stack::static_ default_stack;
			template <typename STACK>
				struct default_stack_size: stack::size_in_mb<16> {};
		};

		template <>
			struct linux_x86_64::default_stack_size<stack::static_>
			: stack::size_in_mb<8> {};

		template <class STACK>
			struct context<linux_x86_64, STACK> {
			};

		struct linux: context_tag {
			typedef linux_x86_64 alias;
		};

		struct best: context_tag {
			typedef linux alias;
		};

		template <typename T>
			struct is {
				static const bool value
					= details::is_base_of<context_tag, T>::value;
			};

		namespace details {

			template <typename TAG>
				struct is_alias_tag {
					typedef char yes;
					struct no { yes _[2]; };

					template <typename T>
						static yes check(T*, typename T::alias* = 0);
					static no check(...);

					static const bool value
						= sizeof check((TAG*)0) == sizeof (yes);
				};

		} // namespace details

		template <typename TAG, bool = details::is_alias_tag<TAG>::value>
			struct resolve_alias {
				typedef TAG type;
			};

		template <typename TAG>
			struct resolve_alias<TAG, true> {
				typedef typename
					resolve_alias<typename TAG::alias>::type
					type;
			};

	} // namespace context


	namespace details {

		template <template <typename> class PREDICATE,
			bool B, typename HEAD, typename... TAIL>
			struct find_if_iter;

		template <template <typename> class PREDICATE,
				 typename HEAD, typename... TAIL>
			struct find_if: find_if_iter<PREDICATE,
				PREDICATE<HEAD>::value, HEAD, TAIL...> {};

		template <template <typename> class PREDICATE,
			typename HEAD, typename... TAIL>
			struct find_if_iter<PREDICATE, true, HEAD, TAIL...> {
				typedef HEAD type;
			};

		template <template <typename> class PREDICATE,
			typename HEAD, typename... TAIL>
			struct find_if_iter<PREDICATE, false, HEAD, TAIL...>
				: find_if<PREDICATE, TAIL...> {};

	} // namespace details

	template <typename T>
	struct some_coroutine {
		template <typename F>
			some_coroutine(F) {};

		int s() { return T(); }
	};

	template <typename S, typename F, typename... CONFIGS>
		struct builder {

			typedef typename
				details::find_if<context::is, CONFIGS..., context::best>::type
				config_context_tag;

			typedef typename context::resolve_alias<config_context_tag>::type
				context_tag;

			typedef typename
				details::find_if<stack::is, CONFIGS...,
					typename context_tag::default_stack>::type
				stack_tag;

			typedef
				typename context_tag::template default_stack_size<stack_tag>
				default_stack_size_type;

			typedef typename
				details::find_if<stack::is_size, CONFIGS...,
					default_stack_size_type>::type
				stack_size_type;

			static const size_t stack_size = stack_size_type::value;
			typedef stack::stack<stack_tag, stack_size> stack_type;
			typedef context::context<context_tag, stack_type> context_type;

			typedef some_coroutine< context_type > coro;
		};

	template <typename S,
			 typename C0 = void,
			 typename C1 = void,
			 typename C2 = void,
			 typename C3 = void,
			 typename C4 = void,
			 typename C5 = void,
			 typename C6 = void,
			 typename C7 = void,
			 typename C8 = void,
			 typename C9 = void,
			 typename F>
		 auto build(F f) -> typename
				 builder<S, F, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9>::coro
		 {
			 return typename
				 builder<S, F, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9>::coro(f);
		 }

} // namespace coroutine

#endif /* BUILDER_H */
