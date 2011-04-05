/*
 * context_oslinux_x86_64.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include "stack.hpp"

#pragma once
#ifndef CONTEXT_OSLINUX_X86_64_H
#define CONTEXT_OSLINUX_X86_64_H

/*
 * Enabled, will use a copy paste of the code
 * (implemented trough template).
 */
#ifndef NO_CORO_LINUX_8664_2SWAPSITE
#	define CORO_LINUX_8664_2SWAPSITE
#endif

namespace coroutine {
namespace details {
namespace oslinux {

template<class Stack = coroutine::stack::Default<> >
class Context
{
	typedef void (function_t)(void*);

	public:
		Context(function_t* f, void* arg):
			_f(f), _arg(arg)
		{
			reset();
		}

		Context(const Context& from):
			_f(from._f), _arg(from._arg)
		{
			reset();
		}

		Context& operator=(const Context& from)
		{
			_f   = from._f;
			_arg = from._arg;
			reset();
			return *this;
		}

		void reset()
		{
			_sp = reinterpret_cast<void**>(_stack.getStackPointer())
				+ (_stack.getSize() / sizeof(void*));

			// red zone begin
			_sp -= 16;                   // red zone
			// red zone end
			
			*--_sp = (void*)this;        // trampoline arg1
			*--_sp = 0;                  // never return
			_sp -= 16;                   // hack space
			*--_sp = (void*)&trampoline; // next instruction addr
			--_sp;                       // rbp
		}
	
		void enter()
		{
#ifdef    CORO_LINUX_8664_2SWAPSITE
			swapContext<1>();
#else  // !CORO_LINUX_8664_2SWAPSITE
			swapContext();
#endif // CORO_LINUX_8664_2SWAPSITE
		}

		void leave()
		{
#ifdef    CORO_LINUX_8664_2SWAPSITE
			swapContext<2>();
#else  // !CORO_LINUX_8664_2SWAPSITE
			swapContext();
#endif // CORO_LINUX_8664_2SWAPSITE
		}
		
		static const char* getImplName() { return "linux x86_64"; }

	private:
		function_t*      _f;
		void*            _arg;
		void**           _sp;
		Stack            _stack;

		static void trampoline(
				int, int, int, int, int, int, // fill reg passing,
				// so everything else will by passed on stack.
				Context* context
				)
		{
			context->_f(context->_arg);
			context->leave();
			abort();
		}

#ifdef    CORO_LINUX_8664_2SWAPSITE
		template <int>
#endif // CORO_LINUX_8664_2SWAPSITE
		void swapContext()
		{
			/*
			 * Optimization discussion:
			 * 	Use an array for store register, high probability
			 * 	for this array to be allocated on the redzon, so, no
			 * 	need to move the stack pointer. Also, maybe move
			 * 	are faster than push/move.
			 * 		- replace push/pop by move.
			 * 			- replace manual allocation by compiler allocation.
			 * 			  Problem: how we can deal with the booststrap?
			 *
			 * 	Remove the useless push $1f.
			 * 		- need to prepare the stack little bit longer.
			 * 		- 
			 * 	
			 * 	So we have some optimizations flags to add:
			 * 		- CORO_LINUX_8664_MOVE
			 * 		- CORO_LINUX_8664_MOVE_REDZONE
			 * 		- CORO_LINUX_8664_NOJUMP
			 *
			 */

			asm volatile (
					// allocate some space... just because
					// compilers seem to use the stack under rsp
					// without carrying about my code.
					"sub $128, %%rsp\n\t"
					
					// store next instruction
					"push $1f\n\t"

					// store registers
					"push %%rbp\n\t"

					// switch stack
					"xchg (%[sp]), %%rsp\n\t"

					// restore registers
					"pop %%rbp\n\t"

					// jump to next instruction
					"pop %%rax\n\t"

					// release the little space.
					"add $128, %%rsp\n\t"

					"jmp *%%rax\n\t"

					"1:\n\t"

					: // output
						// not used
					: // input
						[sp] "d" (&_sp)
					: // modified
						"rax",
						"rbx", "rcx",
						// rdx -> used as input
						// rsp -> manipulated behind the compiler.
						// rbp -> can be used by compiler in debug mode,
						//        so we saving it manually.
						//        - GCC in -O0 mode reserve *bp.
						//        - LLVM doesn't seem to have this caveat.
						"rdi", "rsi",
						"r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
						"%st(1)", "%st(2)", "%st(3)", "%st(4)", "%st(5)",
						"%st(6)", "%st(7)",
						"mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7",
						"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6",
						"xmm7", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12",
						"xmm13", "xmm14", "xmm15",
						"memory"
					//: end
					);
			//end:
		}
};

} // namespace oslinux
} // namespace details
} // namespace coroutine

#endif /* CONTEXT_OSLINUX_X86_64_H */
