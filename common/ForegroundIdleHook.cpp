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
		instance->OnHandler();

		// The regular hook works.
		// Hence remove timer handler from the hanler list of timer
		if (instance->m_TimerHandler){
			Timer::GetInstance()->Remove(instance->m_TimerHandler);
			delete instance->m_TimerHandler;
			instance->m_TimerHandler = NULL;
		}
	}
	return CallNextHookEx(instance->m_Hook, code, wParam, lParam); 
}

void ForegroundIdleHook::OnHandler(){
	EnterCriticalSection(&m_CriticalSection);
	if (!m_Handlers.empty()){
		IdleHandler* handler = *m_Handlers.begin();
		
		m_Handlers.erase(m_Handlers.begin());

		handler->execute();
	}
	LeaveCriticalSection(&m_CriticalSection);
}

ForegroundIdleHook::ForegroundIdleHook(){
	m_TimerHandler = NULL;

	InitializeCriticalSection(&m_CriticalSection);

	m_Hook = SetWindowsHookEx(WH_FOREGROUNDIDLE, (HOOKPROC) Handler, 0, GetCurrentThreadId());

	/*
		If hook initialization failes or in case of Wine it does not work at all
		use Timer to simulate necessary functionality.
		For example on Linux systems under Wine does not implement WH_FOREGROUNDIDLE
	*/

	class TimerBasedIdleHandler : public Timer::Handler{
	private:
		ForegroundIdleHook& m_Fih;
	public:
		TimerBasedIdleHandler(ForegroundIdleHook& fih):m_Fih(fih){
		}
		void execute() override{
			// simulate system callback for WH_FOREGROUNDIDLE
			m_Fih.OnHandler(); // I don't care about right parameters

			// reschedule this instance
			Timer::GetInstance()->Add(this, 200);
		}
	};

	m_TimerHandler = new TimerBasedIdleHandler(*this);
	Timer::GetInstance()->Add(m_TimerHandler, 200);
}
ForegroundIdleHook::~ForegroundIdleHook(){
	if (m_Hook)
		UnhookWindowsHookEx(m_Hook);

	if (m_TimerHandler){
		Timer::GetInstance()->Remove(m_TimerHandler);
		delete m_TimerHandler;
	}

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
