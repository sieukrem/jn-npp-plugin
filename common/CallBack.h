#pragma once

#include <DispEx.h>
#include "combase.h"
#include "../Interfaces.h"

class CallBack :public CComDispatch<ICallBack> 
{
private:
	unsigned int m_StackSize;
	IDispatchEx* m_Cfg;
	unsigned int m_Num;


	CallBack(unsigned int stacksize, IDispatchEx* cfg, unsigned  int num);

	size_t Call(void* ebp);

	static CallBack** GetCallBack(unsigned int index);

	typedef size_t (WINAPI CBP)(void);

public:
	static CallBack* RegisterCallBack( IDispatchEx *cfg, unsigned int stacksize);

	~CallBack();

	CBP* Proc();

	template<int NUM>
	static size_t WINAPI CallBackProc(size_t r1, ...){
#ifndef _M_X64
		void* v_ebp = &r1;	// base pointer, points to actual parameters of called callback function

		/*__asm {
			mov v_ebp, ebp
		}*/

		CallBack* cb = *GetCallBack(NUM);

		DWORD result = cb->Call(v_ebp); // provide actual parameters
		DWORD stackSize = cb->m_StackSize;

		// restore properly the stack pointer of stdcall callee
		// in regard to stack size
		__asm{			
			mov eax, result
			mov edx, stackSize
			mov esp, ebp
			pop ebp
			pop ecx
			add esp, edx
			push ecx
			ret
		}

		return 0;
#else
		va_list vars;		// enforce the compiler to store
		va_start(vars, r1); // integer registers on stack

		void* v_ebp = &r1;	// base pointer, points to actual parameters of called callback function

		CallBack* cb = *GetCallBack(NUM);

		return cb->Call(v_ebp); // provide actual parameters
#endif

	};
};

