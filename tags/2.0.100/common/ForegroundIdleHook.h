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
#include <windows.h>
#include <set>

class ForegroundIdleHook{
private:
	HHOOK m_Hook;
	CRITICAL_SECTION m_CriticalSection; 

	static DWORD CALLBACK Handler( int code,   DWORD wParam,    LONG lParam	);

	ForegroundIdleHook();
	~ForegroundIdleHook();

public:
	class IdleHandler{
	public:
		virtual void execute(){
			// overload this method
		}
	};

	static ForegroundIdleHook* getInstance();

	void remove(IdleHandler* h);

	void add(IdleHandler* h);

private:
	typedef std::set<IdleHandler*> Handlers;
	Handlers m_Handlers;

};
