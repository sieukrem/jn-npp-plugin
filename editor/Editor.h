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

#include "view\EditorView.h"

#include "..\common\Menu.h"
#include "..\common\MyActiveSite.h"
#include "..\common\AcceleratorTable.h"

#define FROM_VAR_BOOL(value) (value == VARIANT_TRUE)? TRUE : FALSE
#define TO_VAR_BOOL(value) (value == TRUE)? VARIANT_TRUE : VARIANT_FALSE

class CEditor : public CComDispatch<IEditor>
{
private :

	static const int m_ViewsNumber = 2;
	CEditorView* m_Views[m_ViewsNumber];
	HMENU m_mainMenu;
	HWND m_NppHandle;

	CAcceleratorTable m_Accelerators;

	static int GetCurrentView();
	int GetMenuItemPosition(HANDLE hMenu, TCHAR* text);

public:
	ScriptObj* m_Listener;

	void CallListener(TCHAR* method, int view = -1, int file = -1);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam); 

	CEditor(HWND nppHandle);
	
	~CEditor();

	void getViewAndPosFrom(SCNotification* eventArgs, int* view, int* pos);

	void setIndicatorLinesUpdater();

	void doOnMessage(SCNotification* eventArgs);

#pragma region IEditor implementing

        HRESULT STDMETHODCALLTYPE get_firstView(  IView **result);
        
        HRESULT STDMETHODCALLTYPE get_currentView( IView **result);
        
        HRESULT STDMETHODCALLTYPE get_secondView( IView **result);

        HRESULT STDMETHODCALLTYPE alert( BSTR *value);

		HRESULT STDMETHODCALLTYPE saveAll();

		HRESULT STDMETHODCALLTYPE get_langs( IDispatch **result);

        HRESULT STDMETHODCALLTYPE get_pluginConfigDir( BSTR *result);
        
        HRESULT STDMETHODCALLTYPE get_nppDir( BSTR *result);
        
        HRESULT STDMETHODCALLTYPE get_tabBarHidden( VARIANT_BOOL *result);
        
        HRESULT STDMETHODCALLTYPE put_tabBarHidden( VARIANT_BOOL value);
        
        HRESULT STDMETHODCALLTYPE get_toolBarHidden(  VARIANT_BOOL *result);
        
        HRESULT STDMETHODCALLTYPE put_toolBarHidden( VARIANT_BOOL value);
        
        HRESULT STDMETHODCALLTYPE get_menuHidden( VARIANT_BOOL* result);

        HRESULT STDMETHODCALLTYPE put_menuHidden( VARIANT_BOOL  value);

        HRESULT STDMETHODCALLTYPE get_statusBarHidden( VARIANT_BOOL* result);
        
        HRESULT STDMETHODCALLTYPE put_statusBarHidden( VARIANT_BOOL  value);

        HRESULT STDMETHODCALLTYPE open( BSTR *value);

		HRESULT STDMETHODCALLTYPE addMenu(  VARIANT cfg, IMenu **result);
		
		HRESULT STDMETHODCALLTYPE runMenuCmd( int cmd);

		HRESULT STDMETHODCALLTYPE setListener(IDispatch* cfg);

		HRESULT STDMETHODCALLTYPE get_handle(int *result);

		HRESULT STDMETHODCALLTYPE createDockable(IDispatch* cfg, IDialog** result);

		HRESULT STDMETHODCALLTYPE addHotKey(IDispatch* cfg);

		HRESULT STDMETHODCALLTYPE createContextMenu(VARIANT cfg,ICtxMenu **result);

#pragma endregion
};
