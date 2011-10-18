/*
 * context_posix.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_POSIX_H
#define CONTEXT_POSIX_H

#include <stdexcept>
#include <string.h>
#include <errno.h>
#include <coroutine/context.hpp>
#include <coroutine/impl/stack_static.hpp>

namespace coroutine {
	namespace context {

		struct posix: context_tag {
			template <typename STACK>
				struct default_stack_size: stack::size_in_mb<64> {};
		};

		template <>
			struct posix::default_stack_size<stack::static_>
			: stack::size_in_mb<8> {};

		namespace posix_api {
#				include <ucontext.h>
		} // namespace posix_api

		template <class STACK>
			struct context<posix, STACK> {
				typedef STACK stack_t;
				public:
					typedef void (function_t)(void*);

					context(function_t* f, void* arg):
						_f(f), _arg(arg) { reset(); }

					context(const context& from) = delete;
					context& operator=(const context& from) = delete;
					context& operator=(context&& from) = delete;

					context(context&& from):
						_maincontext(from._maincontext),
						_corocontext(from._corocontext),
						_f(from._f),
						_arg(from._arg),
						_stack(std::move(from._stack))
					{
						_corocontext.uc_link = &_maincontext;
						from._maincontext.uc_stack.ss_sp = 0;
						from._corocontext.uc_stack.ss_sp = 0;
						from._f = 0;
						from._arg = 0;
					}

					void reset()
					{
						if (getcontext(&_corocontext) == -1)
							error(__PRETTY_FUNCTION__, "getcontext failed");
						_corocontext.uc_link = &_maincontext;
						_corocontext.uc_stack.ss_sp = _stack.get_stack_ptr();
						_corocontext.uc_stack.ss_size = _stack.get_size();
						makecontext(&_corocontext, (void (*)()) _f, 1, _arg);
					}

					void enter()
					{
						if (swapcontext(&_maincontext, &_corocontext) == -1)
							error(__PRETTY_FUNCTION__, "swapcontext failed");
					}

					void leave()
					{
						if (swapcontext(&_corocontext, &_maincontext) == -1)
							error(__PRETTY_FUNCTION__, "swapcontext failed");
					}

					static const char* get_impl_name() { return "posix"; }

				private:
					posix_api::ucontext _maincontext;
					posix_api::ucontext _corocontext;
					function_t* _f;
					void*       _arg;
					stack_t     _stack;

					void error(const char* fname, const char* msg)
					{
						char buf[256];
						std::string errmsg
							= std::string(fname) + ": " + msg + ", ";

						if (::strerror_r(errno, buf, sizeof buf) == 0)
							errmsg += buf;
						else
							errmsg += "unknown error";

						throw std::runtime_error(errmsg);
					}
			};

	} // namespace context
} // namespace coroutine

#endif /* CONTEXT_POSIX_H */
