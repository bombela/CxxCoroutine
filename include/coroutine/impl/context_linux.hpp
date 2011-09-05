/*
 * context_linux.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_LINUX_H
#define CONTEXT_LINUX_H


#if   __SIZEOF_POINTER__ == 8
#	include <coroutine/impl/context_oslinux_x86_64.hpp>

namespace coroutine {
	namespace context {
		struct linux: context_tag {
			typedef linux_x86_64 alias;
		};
	} // namespace context
} // namespace coroutine

#elif __SIZEOF_POINTER__ == 4
#	include <coroutine/impl/context_oslinux_x86_32.hpp>

namespace coroutine {
	namespace context {
		struct linux: context_tag {
			typedef linux_x86_32 alias;
		};
	} // namespace context
} // namespace coroutine

#else
#	error "architecture pointer size not supported"
#endif

#endif /* CONTEXT_LINUX_H */
