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
#include "Dialog.h"


Dialog::Dialog(ScriptObj* cfg, HWND parent):CComDispatch(),m_ActiveObject(NULL),m_BrowserObject(NULL), m_WebBrowser2(NULL){


	m_OleInPlaceFrame.m_Dialog	= this;

	m_OleInPlaceSite.m_Dialog	= this;

	m_OleClientSite.m_Dialog	= this;

	m_DocHostUIHandler.m_Dialog = this;

	m_Cfg = cfg;

	if (parent == 0)
		m_Parent = GetDesktopWindow();
	else
		m_Parent = parent;

	AddRef(); // 1. window should be visible, although it is not referenced from script, otherwise it will be closed with
			  // last reference

}

Dialog::~Dialog(){
	AcceleratorHook::getInstance()->remove(this);

	if (m_Cfg){
		delete m_Cfg;
		m_Cfg = NULL;
	}

	if (isCreated()) {
		::SetWindowLongPtr(m_Hwnd, GWLP_USERDATA, (long)NULL);	//Prevent MessageProc from doing anything, since its virtual
		destroy();
	}
}

int Dialog::execute(MSG* msg){
	if (!m_ActiveObject)
		return 0;

	HWND activeW = 0;
	m_ActiveObject->GetWindow(&activeW);
	
	if (!isInParentChain(activeW, msg->hwnd))
		return 0;

	int result = (m_ActiveObject->TranslateAccelerator(msg) == S_OK)? 1 : 0;
	return result;
}

void Dialog::ReleaseBrowser(){
	if (!m_BrowserObject)
		return;

	// release all script cyclic references to Dialog subelements by loading of empty page
	SysStr aboutBlank(TEXT("about:blank"));
	m_WebBrowser2->Navigate(aboutBlank, NULL, NULL, NULL, NULL);

	m_WebBrowser2->Release();
	m_WebBrowser2 = NULL;

	HRESULT res = m_BrowserObject->SetClientSite(NULL);

	m_BrowserObject->Close(OLECLOSE_NOSAVE);
	m_BrowserObject->Release();
	m_BrowserObject = NULL;
}

void Dialog::BeforeCreate(){
	m_ExStyle = WS_EX_DLGMODALFRAME
			  | WS_EX_WINDOWEDGE
			  | WS_EX_LEFT
			  | WS_EX_LTRREADING
			  | WS_EX_RIGHTSCROLLBAR
			  | WS_EX_CONTROLPARENT
			  ;

	m_Style = WS_OVERLAPPED 
			| WS_CLIPSIBLINGS
			| WS_BORDER
			| WS_SYSMENU
			| WS_THICKFRAME
			| DS_SETFONT
			| DS_MODALFRAME
			| DS_3DLOOK
			;

}

ATOM Dialog::GetWindowClassAtom(){
	// a trick to register a window class once
	static WindowClass wc(TEXT("HTML Based Dialog"), WNDPROC(dlgProc), Statics::instance().hInstance);

	return wc.wclAtom;
}

void Dialog::destroy() {
	if (isCreated()){
		AcceleratorHook::getInstance()->remove(this);

		::DestroyWindow(m_Hwnd);
		m_Hwnd = NULL;
	}
};


bool Dialog::isCreated() const {
	return (m_Hwnd != NULL);
};

bool  Dialog::isInParentChain(HWND parent, HWND child) const{
	HWND curParent = child;
	while(curParent != 0){
		if (parent == curParent)
			return true;

		curParent = GetParent(curParent);
	}
	return false;
}

int Dialog::getWidth() const {
	RECT rc;
	::GetWindowRect(m_Hwnd, &rc);
	return (rc.right - rc.left);
};

int Dialog::getHeight() const {
	RECT rc;
	//::GetClientRect(m_Hwnd, &rc);
	::GetWindowRect(m_Hwnd, &rc);
	if (::IsWindowVisible(m_Hwnd) == TRUE)
		return (rc.bottom - rc.top);
	else
		return (rc.bottom - rc.top);
};

int Dialog::getClientHeight() const {
	RECT rc;
	::GetClientRect(m_Hwnd, &rc);
	if (::IsWindowVisible(m_Hwnd) == TRUE)
		return (rc.bottom - rc.top);
	return (rc.bottom - rc.top);
};

int Dialog::getClientWidth() const {
	RECT rc;
	::GetClientRect(m_Hwnd, &rc);
	return (rc.right - rc.left);
};

void Dialog::display(bool toShow) const {
	if (m_Hwnd){
		BOOL res = ::ShowWindow(m_Hwnd, toShow?SW_SHOW:SW_HIDE);
	}
};

void Dialog::goToCenter()
{
	RECT rc;
	BOOL res2 = GetClientRect(m_Parent, &rc);
	if (res2 == FALSE){
		LastError err;
	}

	POINT center;
	center.x = rc.left + (rc.right - rc.left)/2;
	center.y = rc.top + (rc.bottom - rc.top)/2;

	::ClientToScreen(m_Parent, &center);

	RECT _rc;
	BOOL res = ::GetWindowRect(m_Hwnd, &_rc);
	if (!res){
		DWORD err = GetLastError();
	}


	int x = center.x - (_rc.right - _rc.left)/2;
	int y = center.y - (_rc.bottom - _rc.top)/2;

	::SetWindowPos(m_Hwnd, HWND_TOP, x, y, _rc.right - _rc.left, _rc.bottom - _rc.top, SWP_SHOWWINDOW);
}



void Dialog::create()
{
	m_Hwnd = CreateWindowEx(
							m_ExStyle
							, LPCWSTR(LOWORD(GetWindowClassAtom())), TEXT(""), 
							m_Style, CW_USEDEFAULT,CW_USEDEFAULT, 60,30, m_Parent, 0, Statics::instance().hInstance, this);

	if (!m_Hwnd)
	{
		LastError le;
		throw le.staticmessage();
	}
}


LRESULT CALLBACK Dialog::dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) 
	{
		case WM_NCCREATE:
			return TRUE;
		case WM_CREATE:
		{
			CREATESTRUCT* cstruct = (CREATESTRUCT*)lParam;

			Dialog *dialog =(Dialog *)(cstruct->lpCreateParams);
			dialog->m_Hwnd = hwnd;
			::SetWindowLongPtr(hwnd, GWLP_USERDATA, (long)dialog);
			dialog->MessageProc(message, wParam, lParam);
			return 0;
		}
		default :
		{
			Dialog *dialog = (Dialog *)(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (!dialog)
				return DefWindowProc(hwnd, message, wParam, lParam);
			return dialog->MessageProc(message, wParam, lParam);
		}
	}
}

bool Dialog::IsClosingAllowed(){
	// give a chance to cancel the close operation
	VARIANT beforeCloseRes;

	// not defined onbeforeclose
	if (!m_Cfg->callMethod(TEXT("onbeforeclose"), NULL, 0, &beforeCloseRes))
		return true;

	// invalid value, expect boolean
	if (beforeCloseRes.vt != VT_BOOL)
		return true;

	return beforeCloseRes.boolVal == TRUE;
}

LRESULT CALLBACK Dialog::MessageProc(UINT message, WPARAM wParam, LPARAM lParam){
	switch (message){
		case WM_CLOSE :
			if (!isCreated())
				return TRUE;

			if (!IsClosingAllowed())
				return FALSE; // cancel close operation
			
			if (m_Cfg){
				m_Cfg->callMethod(TEXT("onclose"));

				delete m_Cfg;
				m_Cfg = NULL;
			}

			ReleaseBrowser();
			destroy(); // destroys window
		
			Release(); // see 1.

			return TRUE;
		case WM_SIZE:
			if (m_WebBrowser2){
				//WORD height = HIWORD(lParam);
				int height;
				get_clientHeight(&height);
				//WORD width = LOWORD(lParam);
				int width;
				get_clientWidth(&width);

				m_WebBrowser2->put_Height(height);
				m_WebBrowser2->put_Width(width);
			}
			return FALSE;
	}

	return DefWindowProc(m_Hwnd, message, wParam, lParam); 
}

void Dialog::Init(){
	BeforeCreate();

	create();

	AfterCreate();

	EmbedBrowserObject(m_Hwnd);

	AcceleratorHook::getInstance()->add(this);

}

void Dialog::AfterCreate(){
	goToCenter();
}


HRESULT STDMETHODCALLTYPE Dialog::get_visible( VARIANT_BOOL *result){
	if (isCreated())
		*result = IsWindowVisible(m_Hwnd);

	return S_OK;
}
    
HRESULT STDMETHODCALLTYPE Dialog::put_visible( VARIANT_BOOL value){
	display(value?true:false);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::get_height(  int *result){
	*result = getHeight();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::put_height( int value){
	BOOL  res = SetWindowPos(m_Hwnd, NULL, 0, 0, getWidth(), value,  SWP_NOZORDER|SWP_NOMOVE);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::get_width( int *result){
	*result = getWidth();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::put_width( int value){
	BOOL  res = SetWindowPos(m_Hwnd, NULL, 0, 0, value, getHeight(), SWP_NOZORDER|SWP_NOMOVE);
	return S_OK;
}
HRESULT STDMETHODCALLTYPE Dialog::get_clientHeight( /* [retval][out] */ int *result){
	*result = getClientHeight();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::put_clientHeight( /* [in] */ int value){
	int borderH = getHeight() -  getClientHeight();

	BOOL  res = SetWindowPos(m_Hwnd, NULL, 0, 0, getWidth(), value + borderH,  SWP_NOZORDER|SWP_NOMOVE);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::get_clientWidth(/* [retval][out] */ int *result){
	*result = getClientWidth();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::put_clientWidth( /* [in] */ int value){
	int borderW = getWidth() - getClientWidth();

	BOOL  res = SetWindowPos(m_Hwnd, NULL, 0, 0, value + borderW , getHeight(), SWP_NOZORDER|SWP_NOMOVE);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::get_top( /* [retval][out] */ int *result){
	RECT rc;
	::GetWindowRect(m_Hwnd, &rc);
	*result = rc.top;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::put_top( /* [in] */ int value){
	int left;
	get_left(&left);
	BOOL  res = SetWindowPos(m_Hwnd, NULL, left, value, 0 , 0, SWP_NOZORDER|SWP_NOSIZE);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::get_left( /* [retval][out] */ int *result){
	RECT rc;
	::GetWindowRect(m_Hwnd, &rc);
	*result = rc.left;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::put_left( /* [in] */ int value){
	int top;
	get_top(&top);
	BOOL  res = SetWindowPos(m_Hwnd, NULL, value, top, 0 , 0, SWP_NOZORDER|SWP_NOSIZE);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::get_title( BSTR* result){
	int len = ::GetWindowTextLength(m_Hwnd);
	*result = SysAllocStringLen(NULL, len);
	len = GetWindowText(m_Hwnd, *result,len+1);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::put_title( BSTR* value){
	SetWindowText(m_Hwnd, *value);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::get_document( IDispatch **result){
	
	if (!m_WebBrowser2)
		return S_FALSE;

	HRESULT hr =  m_WebBrowser2->get_Document(result);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::close(){
	PostMessage(m_Hwnd, WM_CLOSE, 0,0);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::get_handle(int *result){
	*result = (int)hwnd();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Dialog::foreground(){
	if (m_Hwnd)
		SetForegroundWindow(m_Hwnd);
	return S_OK;
}
