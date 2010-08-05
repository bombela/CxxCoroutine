/*
 * context_oslinux.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_OSLINUX_H
#define CONTEXT_OSLINUX_H

namespace coroutine {
namespace details {
namespace oslinux {

template <typename F, typename C>
inline void trampoline(C* context, F* f)
{
	std::cout << "___ " << f << " - " << context << std::endl;
	(*f)();
	std::cout << "awwww" << std::endl;
	context->swapContext();
	abort();
}

template <size_t STACK_SIZE, size_t PTR_SIZE>
class ContextImpl;

template<size_t STACK_SIZE>
class ContextImpl<STACK_SIZE, 8>
{
	template <typename F, typename C>
		friend inline void trampoline(C* context, F* f);

	public:
		template <typename F>
		ContextImpl(F& cb)
		{
			typedef void (trampoline_t)(ContextImpl*, F*);

			trampoline_t* cb_ptr = &trampoline<F>;
			_sp = (void**)_stack + sizeof _stack / sizeof(void*);
			std::cout << &_stack << ", " << _sp << " - "
				<< ((char*)_sp - _stack) << std::endl;
			
			// red zone begin
			_sp -= 16;     // red zone
			// red zone end
			*--_sp = 0;                 // trampoline return
			*--_sp = (void*)cb_ptr;     // next instruction addr
			*--_sp = (void*)&cb;        // rsi (trampoline arg2)
			*--_sp = (void*)this;       // rdi (trampoline arg1)
			--_sp;                      // rbp
			std::cout << "ctr " << (void*)&cb << " - " << this << std::endl;
		}
	
		void run() { swapContext(); }
		void yield() { swapContext(); }

	private:
		char   _stack[STACK_SIZE];
		void** _sp;

		void swapContext()
		{
			/*
			 * Optimisation discussion:
			 * 	Use an array for store register, high probability
			 * 	for this array to be allocated on the redzon, so, no
			 * 	need to move the stack pointer. Also, maybe move
			 * 	are faster than push/move.
			 * 		- replace push/pop by move.
			 * 			- replace manual allocation by compiler allocation.
			 *
			 * 	Remove the useless push $1f.
			 * 		- need to preapre the stack little bit longer.
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
			 */

			asm volatile (
					// store next instruction
					"push $1f\n\t"

					// store registers
					"push %%rsi\n\t"
					"push %%rsi\n\t"
					"push %%rbp\n\t"

					// switch stack
					"xchg 0(%[_sp]), %%rsp\n\t"

					// restore registers
					"pop %%rbp\n\t"
					"pop %%rdi\n\t"
					"pop %%rsi\n\t"

					// jump to next instruction
					"pop %%rax\n\t"
					"jmp *%%rax\n\t"

					"1:\n\t"

					: // input/output
						// not used
					: // input
						[_sp]   "rax" (&_sp)
					: // modified
						// rax -> integer return value & used by input
						"rbx", "rcx", "rdx",
						// rsp -> manipulated behind the compiler.
						// rbp -> can be used by compiler in debug mode,
						//        so we saving it manually.
						//        - GCC in -O0 mode reserve *bp.
						//        - LLVM doesn't seem to have this caveat.
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

template <size_t STACK_SIZE = 8192>
class Context: public ContextImpl<STACK_SIZE, sizeof(void*)>
{
	public:
		template <typename F>
		Context(F& cb): ContextImpl<STACK_SIZE, sizeof(void*)>(cb) {}
};

} // namespace oslinux
} // namespace details
} // namespace coroutine

#endif /* CONTEXT_OSLINUX_H */
