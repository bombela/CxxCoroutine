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
			
			// red zone begin
			_sp -= (128 - (8 * 2));     // red zone
			*--_sp = (void*)this;       // rdi (trampoline arg1)
			*--_sp = (void*)&cb;        // rsi (trampoline arg2)
			// red zone end
			*--_sp = 0;                 // trampoline return
			*(_sp - 1) = (void*)cb_ptr; // next instruction addr
			std::cout << "ctr " << (void*)&cb << " - " << this << std::endl;
		}
	
		void run() { swapContext(); }
		void yield() { swapContext(); }

	private:
		char   _stack[STACK_SIZE];
		void** _sp;

		void swapContext()
		{
			// backup in red zone
			asm volatile (
					// store registers in redzone
					"mov %%rsi, 8(%%rsp)\n\t"
					"mov %%rdi, 16(%%rsp)\n\t"

					// store next instruction
					"movl $1f, -8(%%rsp)\n\t"

					// switch stack
					"xchg %[_sp], %%rsp\n\t"

					// restore registers from redzone
					"mov 16(%%rsp), %%rdi\n\t"
					"mov 8(%%rsp),  %%rsi\n\t"

					// jump to next instruction
					"jmp *-8(%%rsp)\n\t"

					"1:\n\t"

					: // output
						[_sp]   "=m" (_sp)
					: // input
						[_sp]   "m" (_sp)
					: // modified
						"rax",
						"rbx",
						"rcx",
						"rdx",
						"rbp",
						"r8",
						"r9",
						"r10",
						"r11",
						"r12",
						"r13",
						"r14",
						"r15"
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
