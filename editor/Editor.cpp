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
#include "Editor.h"
#include "common\System.h"
#include "DockableDialog.h"

#define SUBCLASSING TEXT("jN.Npp.Subclassing")

CEditor::~CEditor(void){

	// Release Views
	for(int i=0; i<m_ViewsNumber; i++){
		m_Views[i]->Release();
	}
	
	// Release Scripting elements
	if (m_Listener){
		delete m_Listener;
		m_Listener = NULL;
	}

	// Restore WndProc
	WNDPROC oldWndProc = (WNDPROC)GetProp(m_NppHandle, SUBCLASSING);
	SetWindowLong(m_NppHandle, GWL_WNDPROC, (LPARAM)oldWndProc);

	AcceleratorHook::getInstance()->remove(&m_Accelerators);
}

int CEditor::GetCurrentView(){
	int currentEdit;
	NPPM(GETCURRENTSCINTILLA,0,&currentEdit);
	return currentEdit;
}

// 1.) because NPP use lets IDs for  menu items 
// and we can not get new IDs for our dynamic menu items. To solve 
// this problem we switch menu message handling from WM_COMMAND to WM_MENUCOMMAND
// see 2.) and 3.)
LRESULT CALLBACK CEditor::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){

	if (message == WM_MENUCOMMAND){
		CMenuItem* mItem = CMenuItem::GetInstance((HMENU)lParam, wParam);
		if (mItem){ 
			// here we have our menu item
			mItem->call();
		}else{ 
			// we have menu items from NPP
			// simulate WM_COMMAND, NPP handles his menu items self

			MENUITEMINFO mii;
			mii.cbSize = sizeof(mii);
			mii.fMask = MIIM_ID | MIIM_DATA;
			// 2.) WM_MENUCOMMAND + submenu handle and item position help us to
			// get MenuItemInfo without using of menu item ID
			BOOL res = GetMenuItemInfo((HMENU)lParam, wParam, TRUE, &mii);
			if (res)
				::SendMessage(hwnd, WM_COMMAND,mii.wID,0);
		}
	} else if (message == WM_INITMENUPOPUP){
		CMenu* menu = CMenu::GetInstance((HMENU)wParam);
		if (menu){
			menu->oninitpopup();
			return NULL;
		}
	}	

	WNDPROC oldWndProc = (WNDPROC)GetProp(hwnd, SUBCLASSING);

	return CallWindowProc(oldWndProc, hwnd, message, wParam, lParam);

}

CEditor::CEditor(HWND nppHandle):CComDispatch(),m_Accelerators(nppHandle){
	AcceleratorHook::getInstance()->add(&m_Accelerators);

	// 3.) set our wndproc to have WM_MENUCOMMAND instead WM_COMMAND
	// see 1.) and 2.)
	m_NppHandle = nppHandle;

	WNDPROC oldWndProc = (WNDPROC)SetWindowLong(nppHandle, GWL_WNDPROC, (LPARAM)CEditor::WndProc);
	SetProp(m_NppHandle, SUBCLASSING, oldWndProc);

	m_Listener = NULL;
	
	// get main menu handle
	m_mainMenu = (HMENU)NPPM(GETMENUHANDLE,1,0);
	if (!m_mainMenu){
		VARIANT_BOOL hidden;

		get_menuHidden(&hidden);

		if (!hidden){
			m_mainMenu = GetMenu(nppData._nppHandle);
		}else{
			put_menuHidden(VARIANT_FALSE); 
			m_mainMenu = GetMenu(nppData._nppHandle);
			put_menuHidden(VARIANT_TRUE);
		}
	}
	// create views
	for(int i=0; i<m_ViewsNumber; i++){
		m_Views[i] = new CEditorView(i+1);
	}

	
	MENUINFO mi;
	memset(&mi, 0, sizeof(mi));
	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_STYLE | MIM_APPLYTOSUBMENUS;
	mi.dwStyle = MNS_NOTIFYBYPOS;
	BOOL res = SetMenuInfo( m_mainMenu, &mi);	
}

void CEditor::getViewAndPosFrom(SCNotification* eventArgs, int* view, int* pos){
	int view_pos = NPPM(GETPOSFROMBUFFERID,eventArgs->nmhdr.idFrom,0);
	if (view_pos!=-1){
		*pos = view_pos & 0xdfffffff;
		*view = view_pos>>30;
	}else{
		*view	= GetCurrentView();
		m_Views[(*view)]->get_file(pos);
	}
}

void CEditor::doOnMessage(SCNotification* eventArgs){
	int r = 0;
	
	// provide messages to current view
	int view = GetCurrentView(); // can be -1 what is not valid
	if (view >= 0 && view < m_ViewsNumber){
		m_Views[view]->DoMessage(eventArgs);
	}

	if (m_Listener==NULL)
		return ;

	TCHAR* method1 = NULL;
	TCHAR* method2 = NULL;

	switch(eventArgs->nmhdr.code){
		case SCN_UPDATEUI:
			method2 = TEXT("UPDATEUI");
			break;
		case SCN_CHARADDED:
			method2 = TEXT("CHARADDED");
			break;
		case SCN_DOUBLECLICK:
			method2 = TEXT("DOUBLECLICK");
			break;
		case SCN_MODIFYATTEMPTRO:
			method2 = TEXT("MODIFYATTEMPTRO");
			break;
		case NPPN_SHUTDOWN:{
			method1 = TEXT("SHUTDOWN");
			break;
		}
		case NPPN_FILECLOSED:{
			method1 = TEXT("FILECLOSED");
			break;
		}
		case NPPN_READONLYCHANGED:{
			method2 = TEXT("READONLYCHANGED");
			break;
		}
		case NPPN_LANGCHANGED:{
			method2 = TEXT("LANGCHANGED");
			break;
		}
		case NPPN_BUFFERACTIVATED:{
			method2 = TEXT("BUFFERACTIVATED");
			break;
		}
		case NPPN_FILESAVED:{
			method2 = TEXT("FILESAVED");
			break;
		}
		case NPPN_FILEOPENED:{
			method2 = TEXT("FILEOPENED");
			break;
		}
	}

	if (method1){

		CallListener(method1);
	}else if(method2){

		int view, pos;	getViewAndPosFrom(eventArgs, &view, &pos);

		CallListener(method2, view, pos);
	}
}
void CEditor::CallListener(TCHAR* method, int view, int file){
	if (m_Listener == NULL)
		return;

	if (view >= 0 && view < m_ViewsNumber){
		VARIANT var[2];
		var[0].vt = VT_DISPATCH;
		var[0].pdispVal = m_Views[view]; 

		var[1].vt = VT_INT;
		var[1].intVal = file;
		
		m_Listener->callMethod(method, var, 2);
		
	}else{
		m_Listener->callMethod(method);
	}
}



HRESULT STDMETHODCALLTYPE CEditor::get_firstView(  IView **result){
	*result	= m_Views[0];
	(*result)->AddRef();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::get_currentView( IView **result){
		*result	= m_Views[GetCurrentView()];
		(*result)->AddRef();
		return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::get_secondView( IView **result){
		*result	= m_Views[1];
		(*result)->AddRef();
		return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::alert( BSTR *value){
		::MessageBox(nppData._nppHandle,*value,TEXT("Editor"), MB_OK);

		return S_OK;
}


HRESULT STDMETHODCALLTYPE CEditor::saveAll( void){
	NPPM(SAVEALLFILES,0,0);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::get_langs( IDispatch **result){
	
	// create JS Array and fill it
	IDispatchEx* arr = MyActiveSite::getInstance()->createObj(TEXT("Array"));
	// fill and free allocated Resources
	int langscount = sizeof(langs)/sizeof(langs[0]);
	for(int i=0; i< langscount; i++){
		MyActiveSite::getInstance()->pushValueInToArr(arr, (TCHAR*)langs[i]);
	}
	*result = arr;
	return S_OK;		
}

HRESULT STDMETHODCALLTYPE CEditor::get_pluginConfigDir( BSTR *result){
	TCHAR path[MAX_PATH];
	NPPM(GETPLUGINSCONFIGDIR,MAX_PATH,path);
	*result = SysAllocString(path);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::get_nppDir( BSTR *result){
	TCHAR path[MAX_PATH];
	NPPM(GETNPPDIRECTORY,MAX_PATH,path);
	*result = SysAllocString(path);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::get_tabBarHidden( VARIANT_BOOL *result){

	LRESULT r = NPPM(ISTABBARHIDDEN,0,0); 
	*result = TO_VAR_BOOL( r );

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::put_tabBarHidden( VARIANT_BOOL value){

	NPPM(HIDETABBAR,0, FROM_VAR_BOOL(value));

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::get_toolBarHidden(  VARIANT_BOOL *result){

	*result =  TO_VAR_BOOL( NPPM(ISTOOLBARHIDDEN,0,0) );

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::put_toolBarHidden( VARIANT_BOOL value){

	NPPM(HIDETOOLBAR, 0, FROM_VAR_BOOL(value));

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::get_menuHidden( VARIANT_BOOL* result){

	*result =  TO_VAR_BOOL( NPPM(ISMENUHIDDEN,0,0) );

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::put_menuHidden( VARIANT_BOOL  value){

	NPPM(HIDEMENU,0, FROM_VAR_BOOL(value));

	return S_OK;
}


HRESULT STDMETHODCALLTYPE CEditor::get_statusBarHidden( VARIANT_BOOL* result){

	*result =  TO_VAR_BOOL(NPPM(ISSTATUSBARHIDDEN,0,0));

	return S_OK;
}


HRESULT STDMETHODCALLTYPE CEditor::put_statusBarHidden( VARIANT_BOOL  value){

	NPPM(HIDESTATUSBAR,0,FROM_VAR_BOOL(value));

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::open( BSTR *value){
	if (value!=NULL){	
		NPPM(DOOPEN, 0, *value);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::addMenu(  VARIANT cfg, IMenu **result){
	if (cfg.vt != VT_EMPTY){

		int count = GetMenuItemCount(m_mainMenu);

		// -2 for [?] and [x] Buttons
		CMenu* m = new CMenu(m_mainMenu, count-2, cfg, nppData._nppHandle); 
		//m->put_text(text);
		*result = m;
		m->AddRef();

		// redraw the main menu after inserting an element on visible window
		BOOL res = DrawMenuBar(nppData._nppHandle);
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::runMenuCmd( int cmd){
	NPPM(MENUCOMMAND,0,cmd);
	return S_OK;
}


HRESULT STDMETHODCALLTYPE CEditor::setListener(IDispatch* cfg){
	if (m_Listener != NULL){
		delete m_Listener;
		m_Listener = NULL;
	}

	if (cfg != NULL)
		m_Listener = MyActiveSite::getInstance()->WrapScriptObj(cfg);

	for(int i=0; i<m_ViewsNumber; i++){
		m_Views[i]->m_Listener = m_Listener;
	}

	return S_OK;
}


HRESULT STDMETHODCALLTYPE CEditor::createDockable(IDispatch* cfg, IDialog** result){
	if (!cfg)
		return S_OK;

	Dialog* d = new DockableDialog(MyActiveSite::getInstance()->WrapScriptObj(cfg), nppData._nppHandle);
	d->Init();

	d->AddRef();
	*result = d;

	NPPM(MODELESSDIALOG,MODELESSDIALOGADD,d->hwnd());

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::get_handle(int *result){
	*result = (int)m_NppHandle;
	return S_OK;
}


class AcceleratorHandler:public CAcceleratorTable::Accelerator{
private:
	ScriptObj* cfgEx;
public:

	AcceleratorHandler(ScriptObj* _cfgEx):cfgEx(_cfgEx){
	}

	int execute(){
		cfgEx->callMethod(TEXT("cmd"));

		return 0;
	}

	~AcceleratorHandler(){
		delete cfgEx;
	}
};


HRESULT STDMETHODCALLTYPE CEditor::addHotKey(IDispatch* cfg){
	if (cfg == NULL)
		return S_OK;
	
	ScriptObj* cfgEx = MyActiveSite::getInstance()->WrapScriptObj(cfg);

	if (!cfgEx)
		return S_OK;
	

	WORD keyCode = System::getKeyCode(cfgEx);

	if (keyCode == 0)
		return S_OK;

	int modifier = System::getAcceleratorModifier(cfgEx);
	
	m_Accelerators.add(modifier|FVIRTKEY, keyCode, new AcceleratorHandler(cfgEx));

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditor::createContextMenu(VARIANT cfg, ICtxMenu **result){
	CContextMenu* m = new CContextMenu(cfg, m_NppHandle); 
	*result = m->As<ICtxMenu>();

	return S_OK;
}
