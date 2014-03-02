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

void Timer::Add(Handler* handler, int millis){
	static TCHAR exceptionBuf[100];

	if (millis < USER_TIMER_MINIMUM){
		wsprintf(exceptionBuf, TEXT("The millis value %d should be greater than %d"), millis, USER_TIMER_MINIMUM);
		throw exceptionBuf;
	}

	if (millis > USER_TIMER_MAXIMUM){
		wsprintf(exceptionBuf, TEXT("The millis value %d should be less than %d"), millis, USER_TIMER_MAXIMUM);
		throw exceptionBuf;
	}


	UINT_PTR timerId = SetTimer(NULL, NULL, millis, Timer::TimerProc);
	if (timerId == NULL){
		LastError errMsg;
		throw errMsg.staticmessage();
	}

	m_Handlers.insert(Handlers::value_type(timerId, handler));
}

void Timer::Remove(Handler* handler){
	for(Handlers::iterator it=m_Handlers.begin(); it!=m_Handlers.end(); ){
		Handlers::iterator current = it;
		++it;

		if (current->second == handler)
			m_Handlers.erase(current);
	}
}

Timer::~Timer(void){
}

void Timer::TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime){
	KillTimer(NULL, idEvent);

	Timer* t = GetInstance();

	Handlers::iterator findIt = t->m_Handlers.find(idEvent);
	
	if (findIt == t->m_Handlers.end())
		return;

	Handler* cfg = findIt->second;

	t->m_Handlers.erase(findIt);

	cfg->execute();
}

Timer* Timer::GetInstance(){
	static Timer t;
	return &t;
}