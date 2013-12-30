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

#include <set>
#include <windows.h>

// hook Message befor npp receive it
// try to translate accelerator and if was translated 
// say npp to do nothing

class AcceleratorHook{
private:
	HHOOK m_Hook;
	CRITICAL_SECTION m_CriticalSection; 

	static DWORD CALLBACK Handler( int code,   DWORD wParam,    LONG lParam	);

	AcceleratorHook();
	~AcceleratorHook();

public:
	class AcceleratorHandler{
	public:
		virtual int execute(MSG* msg){
			// overload this method
			return 0;
		}
	};

	static AcceleratorHook* getInstance();

	void remove(AcceleratorHandler* h);

	void add(AcceleratorHandler* h);

private:
	typedef std::set<AcceleratorHandler*> Handlers;
	Handlers m_Handlers;

};
