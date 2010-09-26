/*
 * context.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H

#if defined(__linux__)
#	include <details/context_oslinux.hpp>
	namespace coroutine {
		using details::oslinux::Context;
	} // namespace coroutine
#elif defined(__USE_POSIX)
#	include <details/context_posix.hpp>
namespace coroutine {
	namespace coroutine {
		using details::posix::Context;
	} // namespace coroutine
} // namespace coroutine
#else
#	error Non posix system not yet supported!
#endif

#endif /* CONTEXT_H */
