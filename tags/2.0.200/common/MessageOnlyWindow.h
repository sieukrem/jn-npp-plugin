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

#include "Dialog\WindowClass.h"

class MessageOnlyWindow{
private:
	HWND m_Hwnd;

	static LRESULT CALLBACK Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
		switch (message) 
		{
			case WM_NCCREATE:
				return TRUE;
			case WM_CREATE:
			{
				CREATESTRUCT* cstruct = (CREATESTRUCT*)lParam;

				MessageOnlyWindow *instance =(MessageOnlyWindow *)(cstruct->lpCreateParams);
				instance->m_Hwnd = hwnd;
				::SetWindowLongPtr(hwnd, GWLP_USERDATA, (long)instance);

				instance->MessageProc(message, wParam, lParam);

				return 0;
			}
			default :
			{
				MessageOnlyWindow *instance = (MessageOnlyWindow *)(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (!instance)
					return DefWindowProc(hwnd, message, wParam, lParam);

				return instance->MessageProc(message, wParam, lParam);
			}
		}
	} 

	ATOM GetWindowClassAtom(){
		// a trick to register a window class once
		static WindowClass wc(TEXT("MessageOnlyWindow"), WNDPROC(Proc), Statics::instance().hInstance);

		return wc.wclAtom;
	}

public:
	MessageOnlyWindow(){
	    CreateWindowEx( 0, LPCWSTR(LOWORD(GetWindowClassAtom())), TEXT("dummy_name"), 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, this );
	}

	virtual LRESULT MessageProc(UINT message, WPARAM wParam, LPARAM lParam){
		return DefWindowProc(m_Hwnd, message, wParam, lParam);
	}

	HWND handle(){
		return m_Hwnd;
	}
};