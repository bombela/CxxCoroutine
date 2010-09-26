/*
 * context_oslinux_x86_32.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_OSLINUX_X86_32_H
#define CONTEXT_OSLINUX_X86_32_H

namespace coroutine {
namespace details {
namespace oslinux {

template<class Stack>
class ContextImpl<Stack, 4>
{
	typedef void (callback_t)();

	public:
		template <typename F>
		ContextImpl(F cb):
			_cbptr( (callback_t*)
					(void (*)(ContextImpl*, F)) &trampoline<ContextImpl, F>),
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
			
			*--_sp = _funcptr;      // (trampoline arg2)
			*--_sp = (void*)this;   // (trampoline arg1)
			*--_sp = 0;             // trampoline return
			*--_sp = (void*)_cbptr; // next instruction addr
			--_sp;                  // rbp
		}
	
		void run() { swapContext(); }
		void yield() { swapContext(); }
		
		static const char* getImplName() { return "linux x86_32"; }

	private:
		callback_t* _cbptr;
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
						// esp -> manipulated behind the compiler.
						// ebp -> can be used by compiler in debug mode,
						//        so we saving it manually.
						//        - GCC in -O0 mode reserve *bp.
						//        - LLVM doesn't seem to have this caveat.
						"esi", "edi",
						"%st(1)", "%st(2)", "%st(3)", "%st(4)", "%st(5)",
						"%st(6)", "%st(7)",
						"mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7",
						"xmm0", "xmm1", "xmm2", "xmm3",
						"xmm4", "xmm5", "xmm6",	"xmm7",
						"memory"
					);
		}
};

} // namespace oslinux
} // namespace details
} // namespace coroutine

#endif /* CONTEXT_OSLINUX_X86_32_H */
