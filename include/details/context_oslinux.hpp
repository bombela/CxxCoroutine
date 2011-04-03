/*
 * context_oslinux.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_OSLINUX_H
#define CONTEXT_OSLINUX_H

#if   __SIZEOF_POINTER__ == 8
#	include <details/context_oslinux_x86_64.hpp>
#elif __SIZEOF_POINTER__ == 4
#	include <details/context_oslinux_x86_32.hpp>
#else
#	error "architecture pointer size not supported"
#endif

#endif /* CONTEXT_OSLINUX_H */
