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
#include <set>

#include "..\common\Statics.h"

#include "..\npp\PluginDefinition.h"
#include "..\npp\dockingResource.h"
#include "DockableDialog.h"


// Because of wrong architecture of Notepad++ it is necessary
// to destroy Strings given to docable dialog manager after destoroying 
// of it self. Hence we don't destroy them at all. To reduce memory consumption 
// create cache.
class StringCache{
	struct StringCmp{
		bool operator()(TCHAR* left, TCHAR* right) const {
			int result = StrCmp(left, right);
			return result < 0;
		}
	};
	typedef std::set<TCHAR*, StringCmp> Cache;
	Cache m_Cache;
public:
	TCHAR* GetCached(TCHAR* value){
		Cache::iterator findIt = m_Cache.find(value);
		if (findIt != m_Cache.end())
			return *findIt;

		// create a copy of value and put it to the cache
		TCHAR* copy = StrDup(value); // uses LocalAlloc
		m_Cache.insert(copy);

		return copy;
	}
	~StringCache(){
		for(Cache::iterator it = m_Cache.begin(); it!=m_Cache.end(); ++it){
			LocalFree(*it);
		}
	}
};

StringCache CachedStrings;

#define DD_AFTER_REGISTERASDOCKABLE WM_USER+1 // sent to control after

DockableDialog::DockableDialog(IDispatchEx* cfg, HWND parent): Dialog(cfg, parent),CComDispatch(),m_Modul(TEXT("jN.dll,")), m_Name(TEXT("jN"))
{
	StringCchCopy(m_Info,sizeof(m_Info), TEXT(""));

	VARIANT* textValue = MyActiveSite::getProperty(TEXT("name"), m_Cfg, VT_BSTR);
	if (textValue != NULL){
		m_Name.Set(textValue->bstrVal);
		m_Modul.Append(textValue->bstrVal);
		VariantClear(textValue);
		delete textValue;
	}
}


void DockableDialog::BeforeCreate(){
	m_Style   = WS_CHILD;
	m_ExStyle = WS_EX_CLIENTEDGE;
}

void DockableDialog::AfterCreate(){
	// Nothing to do override standard behavior
	RegisterAsDockable();
}

int DockableDialog::GetDockingFlags(){
	int result = DWS_DF_CONT_BOTTOM;

	VARIANT* dockingText = MyActiveSite::getProperty(TEXT("docking"), m_Cfg, VT_BSTR);
	if (dockingText){
		if (CSTR_EQUAL == StrCmp(TEXT("top"),dockingText->bstrVal))
			result = DWS_DF_CONT_TOP;
		else if (CSTR_EQUAL == StrCmp(TEXT("right"),dockingText->bstrVal))
			result = DWS_DF_CONT_RIGHT;
		else if (CSTR_EQUAL == StrCmp(TEXT("bottom"),dockingText->bstrVal))
			result = DWS_DF_CONT_BOTTOM;
		else if (CSTR_EQUAL == StrCmp(TEXT("left"),dockingText->bstrVal))
			result = DWS_DF_CONT_LEFT;
		else if (CSTR_EQUAL == StrCmp(TEXT("floating"),dockingText->bstrVal))
			result = DWS_DF_FLOATING;


		VariantClear(dockingText);
		delete dockingText;
	}
	return result;
}
void DockableDialog::RegisterAsDockable(){
	int mask = GetDockingFlags() | DWS_ICONTAB | DWS_ADDINFO;

	memset(&m_Data,0,sizeof(m_Data));

	m_Data.dlgID = 0;												//Nr of menu item to assign (!= _cmdID, beware)
	m_Data.hIconTab      = (HICON) LoadImage(Statics::instance().hInstance, MAKEINTRESOURCE(1),IMAGE_ICON, 0,0,LR_CREATEDIBSECTION|LR_SHARED|LR_LOADTRANSPARENT);												//icon to use
	m_Data.pszAddInfo    = m_Info;											//Titlebar info pointer
	m_Data.pszModuleName = CachedStrings.GetCached( m_Modul );							//name of the dll
	m_Data.pszName       = CachedStrings.GetCached( m_Name );												//Name for titlebar
	m_Data.uMask         = mask;													//Flags to use (see docking.h)
	m_Data.hClient       =  m_Hwnd;												//HWND Handle of window this dock belongs to
	m_Data.iPrevCont     = -1;

	SendMessage(nppData._nppHandle, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&m_Data);		//Register it

	// update ui
	SendMessage(m_Hwnd,  DD_AFTER_REGISTERASDOCKABLE, 0, 0);
}
void DockableDialog::ResetUI(){
	//Registering as dockable breaks window styles, reset them

	LONG_PTR oldStyle = ::GetWindowLongPtr(m_Hwnd, GWL_STYLE);
	::SetWindowLongPtr(m_Hwnd, GWL_STYLE, m_Style | oldStyle);
	::SetWindowLongPtr(m_Hwnd, GWL_EXSTYLE, m_ExStyle);

	SetWindowPos(m_Hwnd, 0, 0, 0, 0, 0,  SWP_FRAMECHANGED| SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	UpdateWindow(m_Hwnd);
}

void DockableDialog::UpdateInfo(){
	m_Data.pszAddInfo = m_Info;
	m_Data.pszName = m_Name;

	SendMessage(m_Hwnd, NPPM_DMMUPDATEDISPINFO, 0, (LPARAM)m_Hwnd);
}

LRESULT CALLBACK DockableDialog::MessageProc(UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
		case DD_AFTER_REGISTERASDOCKABLE:{
			ResetUI();
			return TRUE;
		}
		case WM_NOTIFY:{
			NMHDR nmh = (NMHDR) *((NMHDR*)lParam);
			if (nmh.hwndFrom == nppData._nppHandle) {
				if (nmh.code == DMN_CLOSE) {
					::PostMessage(m_Hwnd, WM_CLOSE, 0, 0);
					return TRUE;
				} else if ((nmh.code & DMN_DOCK) == DMN_DOCK && (nmh.code & DMN_FLOAT) != DMN_FLOAT) {// only if just docked
					PostMessage(m_Hwnd, DD_AFTER_REGISTERASDOCKABLE,0,0);
					return TRUE;
				}
			} 
			break;
		}
		case WM_ERASEBKGND:
			return TRUE;
		//case WM_SETFOCUS: {
		//	//Why restore focus here? This window should never be able to get focus in the first place
		//	HWND hPrev = (HWND)wParam;
		//	if (hPrev != NULL)
		//		::SetFocus(hPrev);
		//	break; 
		//}
	}
	return Dialog::MessageProc(message, wParam, lParam);
}

HRESULT STDMETHODCALLTYPE DockableDialog::get_title( BSTR* result){
	*result = SysAllocString(m_Info);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE DockableDialog::put_title( BSTR* value){
	StringCchCopy(m_Info, sizeof(m_Info), *value);

	UpdateInfo();
	return S_OK;
}


HRESULT STDMETHODCALLTYPE DockableDialog::get_visible( VARIANT_BOOL *result){
	if (isCreated())
		*result = IsWindowVisible(m_Hwnd);

	return S_OK;
}
    
HRESULT STDMETHODCALLTYPE DockableDialog::put_visible( VARIANT_BOOL value){
	::SendMessage(nppData._nppHandle, value ? NPPM_DMMSHOW : NPPM_DMMHIDE, 0, (LPARAM)m_Hwnd);
	return S_OK;
}
