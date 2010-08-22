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
		Timer(const char* pkg, const char* name, int cnt):
			_pkg(pkg),
			_name(name),
			_cnt(cnt),
			_start(clock::time()) { }
		~Timer()
		{
			clock::nanos_t diff = clock::time() - _start;
			std::cout
				<< "elapsed=" << diff << "nsec, "
				<< "cnt=" << _cnt << ", "
				<< "res=" << clock::res() << "nsec, "
				<< "pkg=" << _pkg << ", "
				<< "name=" << _name
				<< std::endl;
		}

	private:
		const char*    _pkg;
		const char*    _name;
		int            _cnt;
		clock::nanos_t _start;
};

} // namespace benchmark

#endif /* TIMER_H */
