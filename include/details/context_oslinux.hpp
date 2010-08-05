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
}

template <size_t STACK_SIZE, size_t PTR_SIZE>
class ContextImpl;

template<size_t STACK_SIZE>
class ContextImpl<STACK_SIZE, 8>
{
	public:
		template <typename F>
		ContextImpl(F& cb)
		{
			typedef void (trampoline_t)(ContextImpl*, F*);

			trampoline_t* cb_ptr = &trampoline<F>;
			_sp = (void**)_stack + sizeof _stack / sizeof(void*);
			
			// red zone begin
			_sp -= (128 - (8 * 2));     // padding
			*--_sp = (void*)this;       // rdi
			*--_sp = (void*)&cb;        // rsi
			// red zone end
			*--_sp = 0;                 // trampoline return
			*(_sp - 1) = (void*)cb_ptr; // next instruction addr
			std::cout << "ctr " << (void*)&cb << " - " << this << std::endl;
		}
	
		void run()
		{
			swapContext();
		}
		
		void yield()
		{
			// backup in red zone
		}

	private:
		char   _stack[STACK_SIZE];
		void** _sp;

		void swapContext()
		{
			// backup in red zone
			void* v;
			asm volatile (
					"mov 16(%[_sp]), %%rdi\n\t"
					"mov 8(%[_sp]),  %%rsi\n\t"
					"mov %[_sp], %%rsp\n\t"
					"jmp -8(%%rsp)\n\t"
					: // output
						[v] "=r" (v)
					: // input
						[_sp]   "a" (_sp)
					: // modified
					);
			std::cout << "aft " << v << std::endl;
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
