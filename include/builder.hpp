/*
 * builder.hpp
 * Copyright © 2011 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef BUILDER_H
#define BUILDER_H

#include <coroutine.hpp>

namespace coroutine {
namespace builder {

template <typename F>
class Coro // extract functor signature!
{
};

template <typename RV, typename FV>
class Coro<RV (FV)> // build function signature
{
};

} // namespace builder
} // namespace coroutine

#endif /* BUILDER_H */
