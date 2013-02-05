/*
This file is part of jN, a plugin for Notepad++
Copyright (C)2013 Eugen Kremer <eugen DOT kremer AT gmail DOT com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <vector>

#include "combase.h"
#include "SysStr.h"
#include "MyActiveSite.h"

#include "..\Interfaces.h"

#include "dialog\windowclass.h"
#include "MessageOnlyWindow.h"


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

		CallBack* cb = *CallBack::GetCallBack(NUM);
		return cb->Call(v_ebp); // provide actual parameters
#else
#pragma message(": warning: Library::call not available")
		return 0;
#endif

	};
};



class System:  public CComDispatch<ISystem>, public MessageOnlyWindow
{
private:
	SysStr m_ScriptFullName;

	int m_SystemHotKeysCount;
	
	typedef std::vector<IDispatchEx*> HotKeyHandlers;
	
	HotKeyHandlers m_SystemHotKeysHandler;

	MyActiveSite& m_ActiveSite;
public:
	System(TCHAR* scriptFullName, MyActiveSite& activeSite);
	virtual ~System(void);

	static WORD getKeyCode(IDispatchEx* cfgEx);
	static int  getAcceleratorModifier(IDispatchEx* cfgEx);

	LRESULT MessageProc(UINT message, WPARAM wp, LPARAM lp) override;

	HRESULT STDMETHODCALLTYPE decodeFrom( int codepage,BSTR *value,BSTR *result);

	HRESULT STDMETHODCALLTYPE encodeTo( int codepage,BSTR *value,BSTR *result);

	HRESULT STDMETHODCALLTYPE addSystemHotKey(IDispatch* cfg);


	HRESULT STDMETHODCALLTYPE createDialog(IDispatch* cfg, IDialog** result);
	HRESULT STDMETHODCALLTYPE addIdleHandler(IDispatch* cfg);
	HRESULT STDMETHODCALLTYPE addScript(BSTR* value);

	HRESULT STDMETHODCALLTYPE setTimeout(IDispatch* cfg);

    HRESULT STDMETHODCALLTYPE put_clipBoard( BSTR *value);

    HRESULT STDMETHODCALLTYPE get_clipBoard( BSTR *result);

	HRESULT STDMETHODCALLTYPE loadLibrary(BSTR* libraryPath, ILibrary** result);

	HRESULT STDMETHODCALLTYPE registerCallBack( IDispatch *cfg, unsigned int stacksize, ICallBack **result);

    HRESULT STDMETHODCALLTYPE get_scriptFullName( BSTR *result);

};
