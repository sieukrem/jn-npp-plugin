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
#include "WindowClass.h"
#include "..\LastError.h"

WindowClass::~WindowClass(){
	UnregisterClass(name,hinst);
}

WindowClass::WindowClass(TCHAR* _name, WNDPROC dlgProc, HINSTANCE _hinst){
	hinst = _hinst;
	name = _name;

	WNDCLASSEX wcl;
	memset(&wcl, 0, sizeof(wcl));

	wcl.cbSize = sizeof(wcl);
	//wcl.style  = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
//			wcl.style  = CS_DBLCLKS;
	wcl.lpfnWndProc = dlgProc;
	wcl.hInstance   = hinst;
	wcl.lpszClassName = name;
	wcl.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
//			wcl.hIcon = IDI_APPLICATION;

	wclAtom = RegisterClassEx(&wcl);

	if (wclAtom == NULL){
		LastError le;
		throw le.staticmessage();
	}
}
