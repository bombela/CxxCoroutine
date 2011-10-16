/*
 * context_linux_x86_32.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_LINUX_X86_32_H
#define CONTEXT_LINUX_X86_32_H

#include <coroutine/stack.hpp>

/*
 * Enabled, will use a copy paste of the code
 * (implemented trough template).
 */
#ifndef NO_CORO_LINUX_8632_2SWAPSITE
#	define CORO_LINUX_8632_2SWAPSITE
#endif

namespace coroutine {
	namespace context {

		struct linux_x86_32: context_tag {
			template <typename STACK>
				struct default_stack_size: stack::size_in_mb<32> {};
		};

		template <>
			struct linux_x86_32::default_stack_size<stack::static_>
			: stack::size_in_mb<4> {};

		template <class STACK>
			struct context<linux_x86_32, STACK> {
				typedef void (function_t)(void*);

			public:
				typedef STACK stack_t;

				context(function_t* f, void* arg):
					_f(f), _arg(arg) { reset(); }

				context(const Context& from) = delete;
				context& operator=(const Context& from) = delete;

				void reset()
				{
					_sp = reinterpret_cast<void**>(_stack.get_stack_ptr())
						+ _stack.get_size() / sizeof(void*);

					*--_sp = _funcptr;      // (trampoline arg2)
					*--_sp = (void*)this;   // (trampoline arg1)
					*--_sp = 0;             // trampoline return
					*--_sp = (void*)_cbptr; // next instruction addr
					--_sp;                  // rbp
				}

				void run()
				{
#ifdef    CORO_LINUX_8632_2SWAPSITE
					swapContext<1>();
#else  // !CORO_LINUX_8632_2SWAPSITE
					swapContext();
#endif // CORO_LINUX_8632_2SWAPSITE
				}
				void yield()
				{
#ifdef    CORO_LINUX_8632_2SWAPSITE
					swapContext<1>();
#else  // !CORO_LINUX_8632_2SWAPSITE
					swapContext();
#endif // CORO_LINUX_8632_2SWAPSITE
				}

				static const char* getImplName() { return "linux x86_32"; }

			private:
				function_t* _cbptr;
				void*       _funcptr;
				void**      _sp;
				Stack       _stack;

				template <typename C, typename F>
					static void trampoline(C* context, F f)
					{
						(*f)();
						context->yield();
						abort();
					}

#ifdef    CORO_LINUX_8632_2SWAPSITE
				template <int>
#endif // CORO_LINUX_8632_2SWAPSITE
					void swapContext()
					{
						asm volatile (
								// store next instruction
								"push $1f\n\t"

								// store registers
								"push %%ebp\n\t"

								// switch stack
								"xchg (%[_sp]), %%esp\n\t"

								// restore registers
								"pop %%ebp\n\t"

								// jump to next instruction
								"pop %%eax\n\t"
								"jmp *%%eax\n\t"

								"1:\n\t"

								: // output
								// not used
								: // input
								[_sp]   "d" (&_sp)
								: // modified
									"eax", "ebx", "ecx",
									// edx -> used as input
									// esp -> manipulated behind the
									// compiler.
									// ebp -> can be used by compiler in
									// debug mode, so we saving it manually.
									//  - GCC in -O0 mode reserve *bp.
									//  - LLVM doesn't seem to have this
									//  caveat.
									"esi", "edi",
									"%st(1)", "%st(2)", "%st(3)", "%st(4)",
									"%st(5)", "%st(6)", "%st(7)",
									"mm0", "mm1", "mm2", "mm3", "mm4",
									"mm5", "mm6", "mm7",
									"xmm0", "xmm1", "xmm2", "xmm3", "xmm4",
									"xmm5", "xmm6",	"xmm7",
									"memory"
									 );
					}
			};

	} // namespace context
} // namespace coroutine

#endif /* CONTEXT_LINUX_X86_32_H */
