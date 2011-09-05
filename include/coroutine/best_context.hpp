/*
 * best_context.hpp
 * Copyright © 2011 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef BEST_CONTEXT_H
#define BEST_CONTEXT_H

#include <coroutine/context.hpp>

#if defined(__linux__)
#	include <coroutine/impl/context_linux.hpp>

namespace coroutine {
	namespace context {

		struct best: context_tag {
			typedef linux alias;
		};

	} // namespace context
} // namespace coroutine

#elif defined(__USE_POSIX)
#	include <coroutine/impl/context_posix.hpp>

namespace coroutine {
	namespace context {
		struct best: context_tag {
			typedef posix alias;
		};

	} // namespace context
} // namespace coroutine

#else
#	error Failed to determind the best context for the current system.
#endif

#endif /* BEST_CONTEXT_H */
