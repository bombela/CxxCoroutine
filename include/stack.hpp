/*
 * stack.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef STACK_H
#define STACK_H

#include <details/stack_static.hpp>
#include <details/stack_dynamic.hpp>

namespace coroutine {
namespace stack {

static const size_t default_size = 2048;

template <size_t SIZE = default_size>
struct Static:  public details::stack::Static<SIZE> {};

template <size_t SIZE = default_size>
struct Dynamic: public details::stack::Dynamic<SIZE> {};

template <size_t SIZE = default_size>
struct Default: public Static<SIZE> {};

} // namespace stack
} // namespace coroutine

#endif /* STACK_H */
