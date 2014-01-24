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

#include <windows.h>
#include "AcceleratorHook.h"

DWORD CALLBACK AcceleratorHook::Handler( int code,   DWORD wParam,    LONG lParam	){
	AcceleratorHook* instance = AcceleratorHook::getInstance(); 

	MSG* msg = (MSG*)lParam;

	if (msg && code == HC_ACTION){
		bool q = msg->message == WM_QUIT;
		
		EnterCriticalSection(&instance->m_CriticalSection);
		for(Handlers::iterator it=instance->m_Handlers.begin(); it!=instance->m_Handlers.end(); ++it){
			AcceleratorHandler* handler = *it;
			if (handler->execute(msg)){
				msg->message = WM_NULL;
				break;
			}
		}
		LeaveCriticalSection(&instance->m_CriticalSection);
	}

	return CallNextHookEx(instance->m_Hook, code, wParam, lParam); 
}

AcceleratorHook::AcceleratorHook(){

	InitializeCriticalSection(&m_CriticalSection);

	m_Hook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC) Handler, 0, GetCurrentThreadId());

}
AcceleratorHook::~AcceleratorHook(){
	UnhookWindowsHookEx(m_Hook);

	DeleteCriticalSection(&m_CriticalSection);
}

AcceleratorHook* AcceleratorHook::getInstance(){
	static AcceleratorHook instance;

	return &instance;
}

void AcceleratorHook::remove(AcceleratorHandler* h){
	EnterCriticalSection(&m_CriticalSection);

	m_Handlers.erase(h);

	LeaveCriticalSection(&m_CriticalSection);
}

void AcceleratorHook::add(AcceleratorHandler* h){
	EnterCriticalSection(&m_CriticalSection);
	
	if (h)
		m_Handlers.insert(h);

	LeaveCriticalSection(&m_CriticalSection);
}
