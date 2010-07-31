/*
 * posixContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <details/context_posix.hpp>

using namespace details::posix;

BOOST_AUTO_TEST_CASE(create)
{
	Context<> context UNUSED;
}

BOOST_AUTO_TEST_CASE(createWithSpecificSize)
{
	Context<1024> context UNUSED;
}
