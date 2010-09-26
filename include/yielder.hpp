/*
 * yielder.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef YIELDER_H
#define YIELDER_H

namespace coroutine {

template <typename C, typename T>
class Yielder
{
	public:

	private:
		Yielder(const Yielder& from); // disabled
		Yielder& operator=(const Yielder& from); // disabled
};

} // namespace coroutine

#endif /* YIELDER_H */
