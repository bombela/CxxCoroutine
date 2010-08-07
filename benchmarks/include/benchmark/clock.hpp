/*
 * clock.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CLOCK_H
#define CLOCK_H

#include <cstdio>

#ifdef __USE_POSIX
#	include <benchmark/details/posix.hpp>
namespace benchmark {
namespace clock = benchmark::details::clock::posix;
} // namespace benchmark
#else
#	error Non posix system not yet supported!
#endif

#endif /* CLOCK_H */
