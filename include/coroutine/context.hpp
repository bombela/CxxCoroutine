/*
 * context.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H

#include <coroutine/impl/stack_dynamic.hpp>

namespace coroutine {
	namespace context {

		struct context_tag {
			// context implementation can choose
			// a different default stack.
			typedef stack::dynamic default_stack;


			/* context implementation HAVE TO define the following template
			 * to provide a default stack size. This template can the be
			 * specialized for any stack implementation if needed.

			 template
			 <typename STACK>
				struct default_stack_size: stack::size_in_mb<24> {};
			*/
		};

		template <typename TAG, class STACK>
			struct context;

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
} // namespace coroutine

#endif /* CONTEXT_H */
