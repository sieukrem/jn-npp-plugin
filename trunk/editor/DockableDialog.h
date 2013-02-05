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

#include "Common\Dialog\Dialog.h"
#include "Common\SysStr.h"

#include "..\npp\Docking.h"

class DockableDialog:public Dialog{
protected:
	void BeforeCreate() override;
	void AfterCreate() override;
private:
	tTbData m_Data;

	SysStr m_Name;
	TCHAR  m_Info[64];
	SysStr m_Modul;
	
	void RegisterAsDockable();
	void UpdateInfo();
	void ResetUI();

	int GetDockingFlags();

	LRESULT CALLBACK MessageProc(UINT message, WPARAM wParam, LPARAM lParam) override;
public:
	DockableDialog(IDispatchEx* cfg, HWND parent);

	virtual HRESULT STDMETHODCALLTYPE get_title( BSTR* result) override;

	virtual HRESULT STDMETHODCALLTYPE put_title( BSTR* value) override;

	virtual HRESULT STDMETHODCALLTYPE get_visible( VARIANT_BOOL *result) override;
	    
	virtual HRESULT STDMETHODCALLTYPE put_visible( VARIANT_BOOL value) override;

};