/*
 * functor_signature.hpp
 * Copyright © 2011 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef FUNCTOR_SIGNATURE_H
#define FUNCTOR_SIGNATURE_H

#include <yielder.hpp>

namespace coroutine {
namespace details {

template <class T, void (T::*)()>
struct MemberSignature
{
};

// T::operator()()

template <class T, typename RV, typename FV>
struct MemberSignature<T, RV (T::*)(FV)>
{
};

template <class F>
struct FunctorSignature
{
//    typedef typename MemberSignature<F, &F::operator()>::rv_t lol_t;
	typedef void rv_t;
	typedef void fv_t;
};

} // namespace details
} // namespace coroutine

#endif /* FUNCTOR_SIGNATURE_H */
