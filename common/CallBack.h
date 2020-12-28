#pragma once

#include <DispEx.h>
#include "combase.h"
#include "../Interfaces.h"

class CallBack :public CComDispatch<ICallBack> 
{
private:
	unsigned int m_Stack;
	IDispatchEx* m_Cfg;
	unsigned int m_Num;


	CallBack(unsigned int stacksize, IDispatchEx* cfg, unsigned  int num);

	DWORD Call(void* ebp);

	static CallBack** GetCallBack(unsigned int index);

	typedef DWORD (WINAPI CBP)(void);

public:
	static CallBack* RegisterCallBack( IDispatchEx *cfg, unsigned int stacksize);

	~CallBack();

	CBP* Proc();

	template<int NUM>
	static DWORD WINAPI CallBackProc(){
#ifndef _M_X64
		void* v_ebp;	// base pointer, points to actual parameters of called callback function

		__asm {
			mov v_ebp, ebp
		}

		CallBack* cb = *GetCallBack(NUM);
		return cb->Call(v_ebp); // provide actual parameters
#else
#pragma message(": warning: Library::call not available")
		return 0;
#endif

	};
};

