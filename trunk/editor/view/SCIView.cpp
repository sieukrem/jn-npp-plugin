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
#include "SCIView.h"

SCIView::SCIView(HWND handle)
{
	m_Handle = handle;
	// retrieve Sci Pointer and Function, for direct calls
	m_SciFn = (int (__cdecl *)(void *,int,int,int))SendMessage(handle,SCI_GETDIRECTFUNCTION,0,0);
	m_SciPtr = (void *)SendMessage(handle,SCI_GETDIRECTPOINTER,0,0);

	LRESULT eventmask = sci(SCI_GETMODEVENTMASK, 0,0);
	eventmask |= SC_MOD_CHANGEFOLD|SC_MOD_DELETETEXT;
	sci(SCI_SETMODEVENTMASK, eventmask, 0);

	// save this pointer, to be able get it again in static functions
	SetProp(m_Handle, TEXT("SCIView Pointer"), (HANDLE)this);

	// subclassing of scintilla
	sciOldWndProc =(WNDPROC)GetWindowLong(m_Handle, GWL_WNDPROC);
	SetWindowLong(m_Handle, GWL_WNDPROC, (LONG) sciPluginMessageProc);
}

SCIView::~SCIView(void)
{
	SetProp(m_Handle, TEXT("SCIView Pointer"), (HANDLE)NULL);
	SetWindowLong(m_Handle, GWL_WNDPROC, (LONG) sciOldWndProc);
}

LRESULT SCIView::sci(int cmd, int wParam, int lParam){
	return m_SciFn(m_SciPtr, cmd, wParam, lParam);
}

LRESULT SCIView::CallOldWndProc(UINT message, WPARAM wParam, LPARAM lParam){
	return CallWindowProc(sciOldWndProc,m_Handle,message,wParam,lParam);
}

LRESULT SCIView::OnMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	return CallOldWndProc(message,wParam,lParam);
}

LRESULT CALLBACK SCIView::sciPluginMessageProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	SCIView* self = (SCIView*)GetProp(hwnd, TEXT("SCIView Pointer"));
	if (self == NULL)
		return 0;

	return  self->OnMessage(hwnd, message, wParam, lParam);
}
