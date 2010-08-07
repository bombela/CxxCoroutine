/*
 * error.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef ERROR_COROUTINE_H
#define ERROR_COROUTINE_H

#include <stdexcept>

namespace coroutine {
namespace error {

class System: public std::runtime_error
{
	public:
		explicit System(const std::string& w): std::runtime_error(w) {}
};

} // namespace error
} // namespace coroutine

#endif /* ERROR_COROUTINE_H */
