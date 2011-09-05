/*
 * best_context.hpp
 * Copyright © 2011 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef BEST_CONTEXT_H
#define BEST_CONTEXT_H

#include <coroutine/context.hpp>

namespace coroutine {
	namespace context {

#if defined(__linux__)

		struct best: context_tag {
			typedef linux alias;
		};

#elif defined(__USE_POSIX)

		struct best: context_tag {
			typedef posix alias;
		};

#else
#	error Failed to determind the best context for the current system.
#endif

	} // namespace context
} // namespace coroutine

#endif /* BEST_CONTEXT_H */
