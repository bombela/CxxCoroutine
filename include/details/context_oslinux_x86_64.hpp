/*
 * context_oslinux_x86_64.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_OSLINUX_X86_64_H
#define CONTEXT_OSLINUX_X86_64_H

/*
 * Enabled, will use trampoline parameters passing
 * by the stack rather than registers.
 */
//#define CORO_LINUX_8664_BOOTSTRAP_STACK

/*
 * Enabled, will 
 */
//#define CORO_LINUX_8664_2CALLSITE

namespace coroutine {
namespace details {
namespace oslinux {

template<class Stack>
class ContextImpl<Stack, 8>
{
	typedef void (callback_t)();

	public:
		template <typename F>
		ContextImpl(F cb):
#ifdef    CORO_LINUX_8664_BOOTSTRAP_STACK
			_cbptr( (callback_t*)
					(void (*)(int, int, int, int, int, int, ContextImpl*, F))
					&trampoline<F>),
#else // !CORO_LINUX_8664_BOOTSTRAP_STACK
			_cbptr( (callback_t*) (void (*)(ContextImpl*, F))
					&trampoline<F>),
#endif // CORO_LINUX_8664_BOOTSTRAP_STACK
			_funcptr( (void*) cb)
		{
			reset();
		}

		ContextImpl(const ContextImpl& from):
			_cbptr(from._cbptr),
			_funcptr(from._funcptr)
		{
			reset();
		}

		ContextImpl& operator=(const ContextImpl& from)
		{
			_cbptr = from._cbptr;
			_funcptr = from._funcptr;
			reset();
			return *this;
		}

		void reset()
		{
			_sp = (void**)_stack.getStack() + Stack::SIZE / sizeof(void*);
			
			// red zone begin
			_sp -= 16;               // red zone
			// red zone end
#ifdef   CORO_LINUX_8664_BOOTSTRAP_STACK
			*--_sp = _funcptr;       // rsi (trampoline arg2)
			*--_sp = (void*)this;    // rdi (trampoline arg1)
			*--_sp = 0;              // trampoline return
			*--_sp = (void*) _cbptr; // next instruction addr
#else // !CORO_LINUX_8664_BOOTSTRAP_STACK
			*--_sp = 0;              // trampoline return
			*--_sp = (void*) _cbptr; // next instruction addr
			*--_sp = _funcptr;       // rsi (trampoline arg2)
			*--_sp = (void*)this;    // rdi (trampoline arg1)
#endif // CORO_LINUX_8664_BOOTSTRAP_STACK
			--_sp;                   // rbp
		}
	
		void run() { swapContext(); }
		void yield() { swapContext(); }

	private:
		callback_t* _cbptr;
		void*       _funcptr;
		void**      _sp;
		Stack       _stack;

		template <typename F>
		static void trampoline(
#ifdef   CORO_LINUX_8664_BOOTSTRAP_STACK
				int, int, int, int, int, int, // fill reg passing,
				// so everything else will by passed trough the stack.
#endif // CORO_LINUX_8664_BOOTSTRAP_STACK
				ContextImpl* context, F f
				)
		{
			(*f)();
			context->swapContext();
			abort();
		}

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
			 * 	Using two different call site can help?
			 * 		- copy/paste two time (maybe macro)
			 * 		- or use template for copye/paste.
			 *
			 * 	So we have 5 flags for optimizations to manage:
			 * 		- CORO_LINUX_8664_MOVE
			 * 		- CORO_LINUX_8664_MOVE_REDZONE
			 * 		- CORO_LINUX_8664_NOJUMP
			 * 		- CORO_LINUX_8664_2CALLSITE_COPYPASTE
			 * 		- CORO_LINUX_8664_2CALLSITE_TEMPLATE
			 *
			 * 	Maybe better to copy past the all code, 
			 * 	and make different templated version...
			 * 	Maybe more readable and easier to debug.
			 *
			 */

			asm volatile (
					// store next instruction
					"push $1f\n\t"

					// store registers
#ifndef   CORO_LINUX_8664_BOOTSTRAP_STACK
					"push %%rsi\n\t"
					"push %%rdi\n\t"
#endif // CORO_LINUX_8664_BOOTSTRAP_STACK
					"push %%rbp\n\t"

					// switch stack
					"xchg (%[_sp]), %%rsp\n\t"

					// restore registers
					"pop %%rbp\n\t"
#ifndef   CORO_LINUX_8664_BOOTSTRAP_STACK
					"pop %%rdi\n\t"
					"pop %%rsi\n\t"
#endif // CORO_LINUX_8664_BOOTSTRAP_STACK

					// jump to next instruction
					"pop %%rax\n\t"
					"jmp *%%rax\n\t"

					"1:\n\t"

					: // output
						// not used
					: // input
						[_sp]   "d" (&_sp)
					: // modified
						"rax", "rbx", "rcx",
						// rdx -> used as input
						// rsp -> manipulated behind the compiler.
						// rbp -> can be used by compiler in debug mode,
						//        so we saving it manually.
						//        - GCC in -O0 mode reserve *bp.
						//        - LLVM doesn't seem to have this caveat.
#ifdef    CORO_LINUX_8664_BOOTSTRAP_STACK
						"rsi", "rdi",
#else // !CORO_LINUX_8664_BOOTSTRAP_STACK
						// rsi -> used as trampoline first arg
						// rdi -> used as trampoline second arg
#endif // CORO_LINUX_8664_BOOTSTRAP_STACK
						"r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
						"%st(1)", "%st(2)", "%st(3)", "%st(4)", "%st(5)",
						"%st(6)", "%st(7)",
						"mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7",
						"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6",
						"xmm7", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12",
						"xmm13", "xmm14", "xmm15",
						"memory"
					);
		}
};

} // namespace oslinux
} // namespace details
} // namespace coroutine

#endif /* CONTEXT_OSLINUX_X86_64_H */
