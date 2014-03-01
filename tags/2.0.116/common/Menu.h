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

#include <dispex.h>

#include "combase.h"
#include "..\Interfaces.h"
#include "MyActiveSite.h"

#define CHECK_AND_FAIL 	if (!CheckValidity()) return E_FAIL;

template<class Interface>
class CAbstractMenuItem : public CComDispatch<Interface>{
protected:
	BOOL m_Removed;
	HMENU m_Id_Handle;
	HMENU m_ParentMenuHandle;

	UINT  getMenuItemState(){
		MENUITEMINFO mii;
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STATE;

		BOOL res = GetMenuItemInfo(
			m_ParentMenuHandle,
			(UINT)m_Id_Handle,
			FALSE,
			&mii
		);
		if (!res){
			DWORD err = GetLastError();
			err = err;
		}

		return mii.fState;
	}
	
	bool CheckValidity(){
		// allright
		if (!m_Removed)
			return true;

		// menu element removed
		MyActiveSite::Throw(TEXT("Any operations on removed Menu or MenuItem are not allowed!"), __uuidof(IMenu) );

		return false;
	}

	CAbstractMenuItem():CComDispatch(){
		m_Removed = FALSE;
	}
public:
    virtual HRESULT STDMETHODCALLTYPE get_text( BSTR *result){
		
		CHECK_AND_FAIL;	// menu item is removed!

		MENUITEMINFO mii;
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STRING;
		mii.fType = MFT_STRING;
		mii.dwTypeData = 0;

		// get menu item length
		BOOL res=GetMenuItemInfo(
			m_ParentMenuHandle,
			(UINT)m_Id_Handle,
			FALSE,
			&mii
		);


		*result = SysAllocStringLen(NULL, mii.cch);
		mii.cch++;

		mii.dwTypeData = *result;
		
		// get menu item
		res = GetMenuItemInfo(
			m_ParentMenuHandle,
			(UINT)m_Id_Handle,
			FALSE,
			&mii
		);

		return S_OK;
	}
    
    virtual HRESULT STDMETHODCALLTYPE put_text( BSTR *value){
		CHECK_AND_FAIL;	// menu item is removed!
		
		MENUITEMINFO mii;
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STRING;
		mii.fType = MFT_STRING;

		mii.cch = SysStringLen(*value);
		
		mii.dwTypeData = *value;

		BOOL res = SetMenuItemInfo(
			m_ParentMenuHandle,
			(UINT)m_Id_Handle,
			FALSE,
			&mii
		);

		return S_OK;
	}
    virtual  HRESULT STDMETHODCALLTYPE get_checked(  VARIANT_BOOL *result){
		CHECK_AND_FAIL;	// menu item is removed!

		UINT state = getMenuItemState();
		*result = state & MFS_CHECKED;

		return S_OK;
	}
    
    virtual HRESULT STDMETHODCALLTYPE put_checked( VARIANT_BOOL value){
		CHECK_AND_FAIL;	// menu item is removed!

		UINT state = getMenuItemState();

		MENUITEMINFO mii;
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STATE;
		UINT s1 = (state & ~MFS_CHECKED);
		mii.fState = s1 | ((value)? MFS_CHECKED : MFS_UNCHECKED);

		BOOL res = SetMenuItemInfo(
			m_ParentMenuHandle,
			(UINT)m_Id_Handle,
			FALSE,
			&mii
		);

		return S_OK;
	}
    
    virtual HRESULT STDMETHODCALLTYPE get_disabled( VARIANT_BOOL *result){
		CHECK_AND_FAIL;	// menu item is removed!

		UINT state = getMenuItemState();
		*result = state & MFS_DISABLED;
		return S_OK;
	}
    
    virtual  HRESULT STDMETHODCALLTYPE put_disabled(  VARIANT_BOOL value){
		CHECK_AND_FAIL;	// menu item is removed!

		UINT state = getMenuItemState();

		MENUITEMINFO mii;
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STATE;
		mii.fState = (state & ~MFS_DISABLED) | ((value)? MFS_DISABLED : MFS_ENABLED);

		BOOL res = SetMenuItemInfo(
			m_ParentMenuHandle,
			(UINT)m_Id_Handle,
			FALSE,
			&mii
		);
		
		return S_OK;
	}
    
	virtual HRESULT STDMETHODCALLTYPE remove(){
		CHECK_AND_FAIL;	// menu item is removed!

		BOOL res = DeleteMenu(m_ParentMenuHandle,(UINT)m_Id_Handle, MF_BYCOMMAND);
		if (res){
			m_Removed = TRUE;
			Release(); // delete self reference
		}

		return S_OK;
	}
};

class CMenuItem : public CAbstractMenuItem<IMenuItem>
{
private:
	ScriptObj* m_Config;

public:
	static CMenuItem* GetInstance(HMENU hmenu, UINT itemPos);

	CMenuItem(HMENU h, WORD id, ScriptObj* config);

	~CMenuItem();

	void call();
};

class CMenu : public CAbstractMenuItem<IMenu>
{
protected: 
	ScriptObj* m_Config;

private:
	WORD  m_Items;
	HWND m_Hwnd;

public:
	static CMenu* GetInstance(HMENU hmenu);

	CMenu(HMENU parent, HWND menuBarWindow = 0);
	CMenu(HMENU parent, int position, VARIANT cfg, HWND menuBarWindow = 0);

	void createMenu(HMENU parent, int position, TCHAR* text, HWND menuBarWindow = 0);

	void oninitpopup();

	~CMenu(void);

	HMENU getHandle();

    virtual HRESULT STDMETHODCALLTYPE addMenu( VARIANT cfg, IMenu **result);
    
    virtual HRESULT STDMETHODCALLTYPE addItem( IDispatch *config, IMenuItem **result);

    virtual HRESULT STDMETHODCALLTYPE addSeparator( void);

	virtual  HRESULT STDMETHODCALLTYPE put_disabled(  VARIANT_BOOL value);

    virtual HRESULT STDMETHODCALLTYPE put_text( BSTR *value);

    virtual HRESULT STDMETHODCALLTYPE put_checked( VARIANT_BOOL value);

};

class CContextMenu: public CComDispatch<ICtxMenu>{
private:
	CMenu* m_Menu;
	HWND m_Window;
public:
	CContextMenu(VARIANT cfg, HWND window);
	
	~CContextMenu();

	virtual HRESULT STDMETHODCALLTYPE addMenu( VARIANT cfg, IMenu **result);
    
    virtual HRESULT STDMETHODCALLTYPE addItem( IDispatch *config, IMenuItem **result);

    virtual HRESULT STDMETHODCALLTYPE addSeparator( void);

    virtual HRESULT STDMETHODCALLTYPE show(void);
};