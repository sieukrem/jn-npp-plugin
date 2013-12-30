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
#include "ForegroundIdleHook.h"

DWORD CALLBACK ForegroundIdleHook::Handler( int code,   DWORD wParam,    LONG lParam	){
	ForegroundIdleHook* instance = ForegroundIdleHook::getInstance(); 
	if (code == HC_ACTION){
		EnterCriticalSection(&instance->m_CriticalSection);
		if (!instance->m_Handlers.empty()){
			IdleHandler* handler = *instance->m_Handlers.begin();
			
			instance->m_Handlers.erase(instance->m_Handlers.begin());

			handler->execute();
		}
		LeaveCriticalSection(&instance->m_CriticalSection);

	}
	return CallNextHookEx(instance->m_Hook, code, wParam, lParam); 
}

ForegroundIdleHook::ForegroundIdleHook(){

	InitializeCriticalSection(&m_CriticalSection);

	m_Hook = SetWindowsHookEx(WH_FOREGROUNDIDLE, (HOOKPROC) Handler, 0, GetCurrentThreadId());

}
ForegroundIdleHook::~ForegroundIdleHook(){
	UnhookWindowsHookEx(m_Hook);

	DeleteCriticalSection(&m_CriticalSection);
}

ForegroundIdleHook* ForegroundIdleHook::getInstance(){
	static ForegroundIdleHook instance;

	return &instance;
}

void ForegroundIdleHook::remove(IdleHandler* h){
	EnterCriticalSection(&m_CriticalSection);

	m_Handlers.erase(h);

	LeaveCriticalSection(&m_CriticalSection);
}

void ForegroundIdleHook::add(IdleHandler* h){
	EnterCriticalSection(&m_CriticalSection);
	
	if (h)
		m_Handlers.insert(h);

	LeaveCriticalSection(&m_CriticalSection);
}
