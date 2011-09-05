/*
 * context_posix.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_POSIX_H
#define CONTEXT_POSIX_H

#include <stdexcept>
#include <coroutine/stack.hpp>

namespace coroutine {
	namespace context {

		struct posix: context_tag {
			template <typename STACK>
				struct default_stack_size: stack::size_in_mb<24> {};
		};

		namespace posix_api {
#include <ucontext.h>
#include <string.h>
#include <errno.h>
		} // namespace posix_api

		template <class STACK>
			struct context<posix, STACK> {
				public:
					typedef void (function_t)(void*);

					context(function_t* f, void* arg):
						_f(f), _arg(arg) { reset(); }

					context(const Context& from) = delete;
					context& operator=(const Context& from) = delete;

					void reset()
					{
						if (getcontext(&_coroContext) == -1)
							error(__PRETTY_FUNCTION__, "getcontext failed");
						_coroContext.uc_link = &_mainContext;
						_coroContext.uc_stack.ss_sp = _stack.get_stack_ptr();
						_coroContext.uc_stack.ss_size = _stack.get_size();
						makecontext(&_coroContext, (void (*)()) _f, 1, _arg);
					}

					void enter()
					{
						if (swapcontext(&_mainContext, &_coroContext) == -1)
							error(__PRETTY_FUNCTION__, "swapcontext failed");
					}

					void leave()
					{
						if (swapcontext(&_coroContext, &_mainContext) == -1)
							error(__PRETTY_FUNCTION__, "swapcontext failed");
					}

					static const char* get_impl_name() { return "posix"; }

				private:
					ucontext    _mainContext;
					ucontext    _coroContext;
					function_t* _f;
					void*       _arg;
					Stack       _stack;

					void error(const char* fname, const char* msg)
					{
						char buf[256];
						std::string errmsg
							= std::string(fname) + ": " + msg + ", ";

						if (strerror_r(errno, buf, sizeof buf) == 0)
							errmsg += buf;
						else
							errmsg += "unknown error";

						throw std::runtime_error(errmsg);
					}
			};

	} // namespace context
} // namespace coroutine

#endif /* CONTEXT_POSIX_H */
