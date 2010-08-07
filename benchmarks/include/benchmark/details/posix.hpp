/*
 * posix.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef POSIX_H
#define POSIX_H

#include <error.hpp>
#include <time.h>
#include <stdint.h>
#include <iostream>
namespace benchmark {
namespace details {
namespace clock {
namespace posix {

typedef int64_t nanos_t;

inline nanos_t res()
{
	timespec res;

	if (clock_getres(CLOCK_PROCESS_CPUTIME_ID, &res) != 0)
		throw coroutine::error::System("clock_getres");

	return res.tv_sec * 1000000000 + res.tv_nsec;
}

inline nanos_t time()
{
	timespec ts;

	if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts) != 0)
		throw coroutine::error::System("clock_gettime");

	return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

} // namespace posix
} // namespace clock
} // namespace details
} // namespace benchmark

#endif /* POSIX_H */
