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
#include "Menu.h"
#include "MyActiveSite.h"


#pragma region CMenuItem
CMenuItem::CMenuItem(HMENU h, WORD id, IDispatch* config):CAbstractMenuItem(){
	m_ParentMenuHandle = h;
	m_Id_Handle =(HMENU)id;

	m_Config = MyActiveSite::queryDispatchEx(config);

	VARIANT* textValue = MyActiveSite::getProperty(TEXT("text"), m_Config, VT_BSTR);
	BSTR text = NULL;

	if (textValue != NULL){
		text = textValue->bstrVal;
		delete textValue;
	}

	if (text == NULL){
		text = SysAllocString(TEXT("Menu Item"));
	}

	MENUITEMINFO mii;
	memset(&mii,0,sizeof(mii));
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_DATA | MIIM_STRING | MIIM_ID;
	mii.fType = MFT_STRING;
	mii.wID = (UINT)m_Id_Handle;
	mii.cch = SysStringLen(text);
	mii.dwTypeData = text;

	mii.dwItemData = (ULONG_PTR)this;
	this->AddRef();// this object must not be released after js engine release its reference

	BOOL res1 = InsertMenuItem(
		m_ParentMenuHandle,
		(UINT)-1,
		TRUE,
		&mii
	);


	SysFreeString(text);
}
CMenuItem::~CMenuItem(){
	MyActiveSite::getInstance()->ReleaseScriptElement(&m_Config);		
}

void CMenuItem::call(){
	if (m_Config == NULL)
		return;

	VARIANTARG mitem;
	mitem.vt		= VT_DISPATCH;
	mitem.pdispVal	= this;

	if(! MyActiveSite::callMethod(TEXT("cmd"), m_Config, &mitem, 1) ){
		MyActiveSite::Throw(TEXT("Method 'cmd' not found"), __uuidof(IMenu));
	}
}

CMenuItem* CMenuItem::GetInstance(HMENU hmenu, UINT itemPos){
	MENUITEMINFO mii;
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_ID | MIIM_DATA;
	BOOL res = GetMenuItemInfo(hmenu, itemPos, TRUE, &mii);
	
	if (!res)
		return NULL;

	if (0 == mii.dwItemData)
		return NULL;

	// here we have our menu item
	CMenuItem* mItem = (CMenuItem*)mii.dwItemData;
	
	return mItem;
}

#pragma endregion

#pragma region CMenu

CMenu::CMenu(HMENU parent, HWND hwnd):CAbstractMenuItem(){
	createMenu(parent, -1, TEXT("text"), hwnd);
}

CMenu::CMenu(HMENU parent, int position, VARIANT cfg, HWND hwnd):CAbstractMenuItem(){
	if (cfg.vt == VT_DISPATCH){
		IDispatchEx* dispex = MyActiveSite::queryDispatchEx(cfg.pdispVal);	
		VARIANT* var = MyActiveSite::getProperty(TEXT("text"), dispex, VT_BSTR);
		if (var){
			createMenu(parent, position, var->bstrVal);
			VariantClear(var);
			delete var;
		}else
			createMenu(parent, position, TEXT("text"), hwnd);

		m_Config  = dispex;
	} else if (cfg.vt == VT_BSTR){
		createMenu(parent, position, cfg.bstrVal, hwnd );
	}

}

HMENU CMenu::getHandle(){
	return m_Id_Handle;
}

void CMenu::createMenu(HMENU parent, int position, TCHAR* text, HWND menuBarWindow){
	m_Hwnd = menuBarWindow;
	m_Config = NULL;
	m_Items = 0;
	m_ParentMenuHandle = parent;
	m_Id_Handle = ::CreatePopupMenu();

	BOOL res;
	if (position==-1)
		res = ::AppendMenu(m_ParentMenuHandle,  MF_POPUP, (UINT_PTR )m_Id_Handle, text);
	else
		res = ::InsertMenu(m_ParentMenuHandle, position, MF_BYPOSITION | MF_POPUP, (UINT_PTR )m_Id_Handle, text);

	MENUINFO mi;
	memset(&mi, 0, sizeof(mi));
	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_STYLE | MIM_MENUDATA;
	mi.dwStyle = MNS_NOTIFYBYPOS;
	mi.dwMenuData = (ULONG_PTR)this;
	this->AddRef();

	res = SetMenuInfo(          
		m_Id_Handle,
		&mi
	);

}

CMenu* CMenu::GetInstance(HMENU hmenu){
	MENUINFO mi;
	memset(&mi, 0, sizeof(mi));
	mi.fMask =MIM_MENUDATA;
	mi.cbSize = sizeof(mi);

	BOOL res = GetMenuInfo(hmenu, &mi);
	if (!res)
		return NULL;

	if (!mi.dwMenuData)
		return NULL;

	CMenu* menu = (CMenu*)mi.dwMenuData; // hopefully it is our data :-)

	return menu;
}

void CMenu::oninitpopup(){
	if (m_Config)
		MyActiveSite::callMethod(TEXT("oninitpopup"), m_Config);
}

CMenu::~CMenu(void){
	int r = DestroyMenu(m_Id_Handle);
}

HRESULT STDMETHODCALLTYPE CMenu::addMenu( VARIANT cfg, IMenu **result){
	CHECK_AND_FAIL;	// menu item is removed!

	if (cfg.vt == VT_DISPATCH){
		IDispatchEx*  dispCfg = MyActiveSite::queryDispatchEx(cfg.pdispVal);	
		VARIANT* var = MyActiveSite::getProperty(TEXT("text"), dispCfg, VT_BSTR);
		if (!var)
			return S_OK;

		CMenu* menu = new CMenu(m_Id_Handle); 
		menu->m_Config = dispCfg;
		*result = menu;

		(*result)->put_text(&var->bstrVal);
		(*result)->AddRef();
		

		VariantClear(var);
		delete var;
	} else if (cfg.vt == VT_BSTR){
		*result = new CMenu(m_Id_Handle); 
		(*result)->put_text(&cfg.bstrVal);
		(*result)->AddRef();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CMenu::addItem(IDispatch *config, IMenuItem **result){
	CHECK_AND_FAIL;	// menu item is removed!

	if (config!=NULL){
		*result = new CMenuItem(m_Id_Handle, m_Items, config);
		(*result)->AddRef();
		m_Items++;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CMenu::addSeparator( void){
	CHECK_AND_FAIL;	// menu item is removed!

	::AppendMenu(m_Id_Handle, MF_SEPARATOR, m_Items, NULL);
	m_Items++;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CMenu::put_disabled(  VARIANT_BOOL value){

	CAbstractMenuItem<IMenu>::put_disabled(value);

	if (m_Hwnd)
		DrawMenuBar(m_Hwnd);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CMenu::put_text( BSTR *value){
	CAbstractMenuItem<IMenu>::put_text(value);	

	if (m_Hwnd)
		DrawMenuBar(m_Hwnd);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CMenu::put_checked( VARIANT_BOOL value){
	CAbstractMenuItem<IMenu>::put_checked(value);

	if (m_Hwnd)
		DrawMenuBar(m_Hwnd);

	return S_OK;
}

#pragma endregion