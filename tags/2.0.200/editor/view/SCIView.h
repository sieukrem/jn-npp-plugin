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
#include "npp\Scintilla.h"

class SCIView
{
private:
	int (__cdecl *m_SciFn)(void*,int,int,int);
	void* m_SciPtr;

public:
	HWND m_Handle;
	SCIView(HWND handle);
	~SCIView(void);

	LRESULT sci(int cmd, int wParam, int lParam);

	// subclassing for scintilla
	WNDPROC sciOldWndProc;
	static LRESULT CALLBACK sciPluginMessageProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	LRESULT CallOldWndProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
