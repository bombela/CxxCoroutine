/*
 * context_linux_x86_64.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_LINUX_X86_64_H
#define CONTEXT_LINUX_X86_64_H

#include <stdint.h>
#include <coroutine/stack.hpp>
#include <coroutine/context.hpp>
#include <coroutine/impl/stack_static.hpp>

/*
 * Enabled, will use a copy paste of the code
 * (implemented trough template).
 */
#ifndef NO_CORO_LINUX_8664_2SWAPSITE
#	define CORO_LINUX_8664_2SWAPSITE
#endif

namespace coroutine {
	namespace context {

		struct linux_x86_64: context_tag {
			template <typename STACK>
				struct default_stack_size: stack::size_in_mb<64> {};
		};

		template <>
			struct linux_x86_64::default_stack_size<stack::static_>
			: stack::size_in_mb<8> {};

		template <class STACK>
			struct context<linux_x86_64, STACK> {
				typedef void (function_t)(void*);

			public:
				typedef STACK stack_t;

				context(function_t* f, void* arg):
					_f(f), _arg(arg) { reset(); }

				context(const context& from) = delete;
				context& operator=(const context& from) = delete;
				context& operator=(context&& from) = delete;

				context(context&& from):
					_f(from._f),
					_arg(from._arg),
					_sp(from._sp),
					_stack(std::move(from._stack))
					{
						from._f = 0;
						from._arg = 0;
						from._sp = 0;
					}

				void reset()
				{
					_sp = reinterpret_cast<void**>(
							// 16 bytes aligned
							reinterpret_cast<uintptr_t>(
								_stack.get_stack_ptr() + _stack.get_size()
								) & static_cast<uintptr_t>(~15)
							);

					// red zone begin
					_sp -= 16; // red zone
					// red zone end

					--_sp; // break 16 bytes boundary alignment so:
					*--_sp = (void*)this; // trampoline arg1 is aligned.
					*--_sp = 0; // and trampoline return addr is not.

					_sp -= 2;                    // hack space
					*--_sp = (void*)&trampoline; // next instruction addr
					--_sp;                       // rbp
				}

				void enter()
				{
#ifdef    CORO_LINUX_8664_2SWAPSITE
					swapcontext<1>();
#else  // !CORO_LINUX_8664_2SWAPSITE
					swapcontext();
#endif // CORO_LINUX_8664_2SWAPSITE
				}

				void leave()
				{
#ifdef    CORO_LINUX_8664_2SWAPSITE
					swapcontext<2>();
#else  // !CORO_LINUX_8664_2SWAPSITE
					swapcontext();
#endif // CORO_LINUX_8664_2SWAPSITE
				}

				static const char* getImplName() { return "linux x86_64"; }

			private:
				function_t*      _f;
				void*            _arg;
				void**           _sp;
				stack_t          _stack;

				static void trampoline(
						int, int, int, int, int, int, // fill reg passing,
						// so everything else will by passed on stack.
						context* context
						)
				{
					context->_f(context->_arg);
					context->leave();
					abort();
				}

#ifdef    CORO_LINUX_8664_2SWAPSITE
				template <int>
#endif // CORO_LINUX_8664_2SWAPSITE
					void swapcontext()
					{
						/*
						 * Optimization discussion:
						 *	Use an array for store register, high
						 *	probability for this array to be allocated on
						 *	the redzon, so, no need to move the stack
						 *	pointer. Also, maybe move are faster than
						 *	push/move.
						 *		- replace push/pop by move.
						 *			- replace manual allocation by compiler
						 *			allocation. Problem: how we can deal
						 *			with the booststrap?
						 *
						 *	Remove the useless push $1f.
						 *		- need to prepare the stack little bit
						 *		longer.
						 *
						 *	So we have some optimizations flags to add:
						 *		- CORO_LINUX_8664_MOVE
						 *		- CORO_LINUX_8664_MOVE_REDZONE
						 *		- CORO_LINUX_8664_NOJUMP
						 *
						 */

						asm volatile (
								/* allocate some space... just because
								 * compilers seem to use the stack under rsp
								 * when rsp is not aligned. Because I cant
								 * know if I am playing with an aligned
								 * stack or not and I want to keep the same
								 * one I reserve 16 bytes.
								 */

								"sub $16, %%rsp\n\t"

								// store next instruction
								"push $1f\n\t"

								// store registers
								"push %%rbp\n\t"

								// switch stack
								"xchg (%[sp]), %%rsp\n\t"

								// restore registers
								"pop %%rbp\n\t"

								// retrieve next instruction addr
								"pop %%rax\n\t"

								// release the little space.
								"add $16, %%rsp\n\t"

								// jump to next instruction
								"jmp *%%rax\n\t"

								"1:\n\t"

								: // output
								  // no output
								: // input
								[sp] "d" (&_sp)
								: // modified
									"rax",
									"rbx", "rcx",
									// rdx -> used as input
									// rsp -> manipulated behind the
									// compiler.
									// rbp -> can be used by compiler in
									// debug mode, so we saving it manually.
									//   - GCC in -O0 mode reserve *bp.
									//   - LLVM doesn't seem to have this
									//   caveat.
									"rdi", "rsi", "r8", "r9", "r10", "r11",
									"r12", "r13", "r14", "r15",
//                                    "%st(1)", "%st(2)", "%st(3)", "%st(4)",
//                                    "%st(5)", "%st(6)", "%st(7)",
									"mm0", "mm1", "mm2", "mm3", "mm4",
									"mm5", "mm6", "mm7",
									"xmm0", "xmm1", "xmm2", "xmm3", "xmm4",
									"xmm5", "xmm6", "xmm7", "xmm8", "xmm9",
									"xmm10", "xmm11", "xmm12", "xmm13",
									"xmm14", "xmm15",
									"memory"
									 );
					}
			};

	} // namespace context
} // namespace coroutine

#endif /* CONTEXT_LINUX_X86_64_H */
