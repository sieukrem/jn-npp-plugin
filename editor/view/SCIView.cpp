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
	m_SciFn = (sptr_t(*)(sptr_t, unsigned int, uptr_t, sptr_t))SendMessage(handle,SCI_GETDIRECTFUNCTION,0,0);
	m_SciPtr = SendMessage(handle,SCI_GETDIRECTPOINTER,0,0);

	LRESULT eventmask = sci(SCI_GETMODEVENTMASK, 0,0);
	eventmask |= SC_MOD_CHANGEFOLD|SC_MOD_DELETETEXT;
	sci(SCI_SETMODEVENTMASK, eventmask, 0);

	// save this pointer, to be able get it again in static functions
	SetProp(m_Handle, TEXT("SCIView Pointer"), (HANDLE)this);

	// subclassing of scintilla
	sciOldWndProc =(WNDPROC)GetWindowLongPtr(m_Handle, GWLP_WNDPROC);
	SetProp(m_Handle, TEXT("SCIView WndProc"), (HANDLE)sciOldWndProc);

	SetWindowLongPtr(m_Handle, GWLP_WNDPROC, (LONG_PTR) sciPluginMessageProc);
}

SCIView::~SCIView(void)
{
	RemoveProp(m_Handle, TEXT("SCIView Pointer"));
}

LRESULT SCIView::sci(unsigned int cmd, uptr_t wParam, sptr_t lParam){
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
	if (self == NULL){
		WNDPROC old = (WNDPROC)GetProp(hwnd, TEXT("SCIView WndProc"));

		return CallWindowProc(old,hwnd,message,wParam,lParam);
	}

	return  self->OnMessage(hwnd, message, wParam, lParam);
}
