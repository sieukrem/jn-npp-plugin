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
#include "Timer.h"
#include "Windows.h"
#include "..\Interfaces.h"

#include "LastError.h"

Timer::Timer(void)
{
}

bool Timer::AddHandler(ScriptObj* cfg)
{
	VARIANT* millis = cfg->getProperty(TEXT("millis"), VT_I4);
	if (millis == NULL){
		MyActiveSite::Throw(TEXT("Expect integer 'millis' property"), __uuidof(ISystem) );
		return false;
	}

	if (millis->intVal < USER_TIMER_MINIMUM){
		TCHAR buf[100];
		wsprintf(buf, TEXT("The millis value %d should be greater than %d"), millis->intVal, USER_TIMER_MINIMUM);

		MyActiveSite::Throw(buf, __uuidof(ISystem) );
		return false;
	}

	if (millis->intVal > USER_TIMER_MAXIMUM){
		TCHAR buf[100];
		wsprintf(buf, TEXT("The millis value %d should be less than %d"), millis->intVal, USER_TIMER_MAXIMUM);

		MyActiveSite::Throw(buf, __uuidof(ISystem) );
		return false;
	}

	UINT_PTR timerId = SetTimer(NULL, NULL, millis->intVal, Timer::TimerProc);
	if (timerId == NULL){
		LastError errMsg;
		MyActiveSite::Throw(errMsg, __uuidof(ISystem) );

		return false;
	}

	m_Handlers.insert(Handlers::value_type(timerId, cfg));

	VariantClear(millis);
	delete millis;

	return true;
}

Timer::~Timer(void){
}

void Timer::TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime){
	KillTimer(NULL, idEvent);

	Timer* t = GetInstance();

	Handlers::iterator findIt = t->m_Handlers.find(idEvent);
	
	if (findIt == t->m_Handlers.end())
		return;

	ScriptObj* cfg = findIt->second;

	t->m_Handlers.erase(findIt);

	cfg->callMethod(TEXT("cmd"));

	delete cfg;
}

Timer* Timer::GetInstance(){
	static Timer t;
	return &t;
}