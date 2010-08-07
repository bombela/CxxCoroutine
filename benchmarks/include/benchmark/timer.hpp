/*
 * timer.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <benchmark/clock.hpp>
#include <iostream>

namespace benchmark {

class Timer
{
	public:
		Timer(): _start(clock::time()) { }
		~Timer()
		{
			clock::nanos_t diff = clock::time() - _start;
			std::cout << "elapsed=" << diff << "nsec, "
				"res=" << clock::res() << "nsec" << std::endl;
		}

	private:
		clock::nanos_t _start;
};

} // namespace bep

#endif /* TIMER_H */
