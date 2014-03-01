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

#include <shobjidl.h>

#include "System.h"
#include "MyActiveSite.h"
#include "Dialog\Dialog.h"
#include "Timer.h"

#include "ForegroundIdleHook.h"
#include "ScriptedIdleHandler.h"

#include "Library.h"

#include "LastError.h"



CallBack::CallBack(unsigned int stacksize, IDispatchEx* cfg, unsigned int num):CComDispatch(){
	m_Stack = stacksize;
	m_Cfg = cfg;
	cfg->AddRef();
	m_Num = num;
	*CallBack::GetCallBack(num) = this;
}

DWORD CallBack::Call(void* ebp){
	void* params = ((DWORD*)ebp) + 2; // 1 for the return adress

	VARIANT result;

	VARIANT var[1];
	var[0].vt = VT_BSTR;
	var[0].bstrVal = SysAllocStringByteLen(((LPCSTR)params),m_Stack); 

	MyActiveSite::callMethod(TEXT("cmd"), m_Cfg, var, 1, &result);

	SysFreeString(var[0].bstrVal);

	VARTYPE type = VT_UI4;
	HRESULT hr = VariantChangeType(&result, &result, 0, type);
	
	DWORD dwResult = 0;
	dwResult = result.uintVal;

	return dwResult;
}

CallBack::~CallBack(){
	m_Cfg->Release();
	*CallBack::GetCallBack(m_Num) = NULL;
}

CallBack::CBP* CallBack::Proc(){
	// say compiler it should create 12 function stubs
	static CallBack::CBP* cbps[]={
		&CallBack::CallBackProc<0>,&CallBack::CallBackProc<1>,
		&CallBack::CallBackProc<2>,&CallBack::CallBackProc<3>,
		&CallBack::CallBackProc<4>,&CallBack::CallBackProc<5>,
		&CallBack::CallBackProc<6>,&CallBack::CallBackProc<7>,
		&CallBack::CallBackProc<8>,&CallBack::CallBackProc<9>,
		&CallBack::CallBackProc<10>,&CallBack::CallBackProc<11>,
	};

	return cbps[m_Num];
}
CallBack** CallBack::GetCallBack(unsigned int index){
	static CallBack* callBacks[12]={0};
	return &callBacks[index];
}


CallBack* CallBack::RegisterCallBack( IDispatchEx *cfg, unsigned int stacksize){
	for(int i=0; i<12; i++){
		if (*CallBack::GetCallBack(i)!=NULL)
			continue;

		return new CallBack(stacksize, cfg, i);
	}

	return NULL;
}
System::System(TCHAR* scriptFullName, MyActiveSite& activeSite):CComDispatch(), m_ScriptFullName(scriptFullName), m_ActiveSite(activeSite)
{
}

System::~System(void)
{
	int keyId = 0;
	for(HotKeyHandlers::iterator it=m_SystemHotKeysHandler.begin(); it!=m_SystemHotKeysHandler.end(); ++it){
		UnregisterHotKey(handle(), keyId);
		keyId++;

		delete *it;
	}
}
LRESULT System::MessageProc(UINT message, WPARAM wp, LPARAM lp) {
	switch(message){
		case WM_HOTKEY:{
			if (m_SystemHotKeysHandler.size() > wp){
				m_SystemHotKeysHandler[wp]->callMethod(TEXT("cmd"));
			}

		}
		case WM_COMMAND:{
		
			WORD command = LOWORD(wp);
			//m_Accelerators.CallHandler(command);
		}
	}

	return 0;
}


HRESULT STDMETHODCALLTYPE System::decodeFrom( 
    /* [in] */ int codepage,
    /* [in] */ BSTR *value,
	/* [retval][out] */ BSTR *result){
	if (value != NULL){
		char* buf;
		
		int newlen = WideCharToMultiByte(0, 0, (LPCWSTR)*value, SysStringLen(*value), NULL,0, NULL, NULL);
		buf = new char[newlen+1];
		int res = WideCharToMultiByte(0,0, (LPCWSTR)*value, SysStringLen(*value), buf, newlen, NULL,NULL);
		buf[res] = 0;
	
		int len = MultiByteToWideChar(codepage, 0, buf, newlen, NULL,0);
		
		*result = SysAllocStringLen(NULL, len);
		res = MultiByteToWideChar(codepage, 0, buf, newlen, *result, len);

		delete buf;
	}
	return S_OK;
}
HRESULT STDMETHODCALLTYPE System::encodeTo( int codepage, BSTR *value, BSTR *result){
	if (value != NULL){
		char* buf;
		
		int newlen = WideCharToMultiByte(codepage, 0, (LPCWSTR)*value, SysStringLen(*value), NULL,0, NULL, NULL);
		buf = new char[newlen];
		int res = WideCharToMultiByte(codepage,0, (LPCWSTR)*value, SysStringLen(*value), buf, newlen, NULL,NULL);
	
		// every char byte as multibyte char
		int len = MultiByteToWideChar(0, 0, buf, newlen, NULL,0);
		*result = SysAllocStringLen(NULL, len);
		res = MultiByteToWideChar(0, 0, buf, newlen, *result, len);

		delete buf;
	}
	return S_OK;
}


/*
var hk = addSystemHotKey({
	key:"g",
	ctrl: true,
	shift:true,
	cmd:function(){alert("aa");}
});
*/
HRESULT STDMETHODCALLTYPE System::addSystemHotKey(IDispatch* cfg){
	if (cfg != NULL){
		ScriptObj* cfgEx = MyActiveSite::getInstance()->WrapScriptObj(cfg);
		if (cfgEx){
			int modifier = 0;

			WORD keyCode = getKeyCode(cfgEx);

			if (keyCode == 0)
				return S_OK;

			VARIANT* modifierBOOL = cfgEx->getProperty(TEXT("shift"), VT_BOOL);
			if (modifierBOOL != NULL){
				modifier = modifier | (modifierBOOL->boolVal? MOD_SHIFT : 0);
				VariantClear(modifierBOOL);
				delete modifierBOOL;
			}

			modifierBOOL = cfgEx->getProperty(TEXT("alt"), VT_BOOL);
			if (modifierBOOL != NULL){
				modifier = modifier | ((modifierBOOL->boolVal)? MOD_ALT : 0);
				VariantClear(modifierBOOL);
				delete modifierBOOL;
			}

			modifierBOOL = cfgEx->getProperty(TEXT("ctrl"), VT_BOOL);
			if (modifierBOOL != NULL){
				modifier = modifier | ((modifierBOOL->boolVal)? MOD_CONTROL : 0);
				VariantClear(modifierBOOL);
				delete modifierBOOL;
			}

			BOOL res = RegisterHotKey(handle(), m_SystemHotKeysHandler.size(), modifier,keyCode);
			if (res){
				m_SystemHotKeysHandler.push_back(cfgEx);
			}else{
				LastError err;
				MyActiveSite::Throw(err.message(), __uuidof(ISystem)); 
			}
		}
	}

	return S_OK;
}


HRESULT STDMETHODCALLTYPE System::createDialog(IDispatch* cfg, IDialog** result){

	if (!cfg)
		return S_OK;

	Dialog* d = new Dialog(MyActiveSite::getInstance()->WrapScriptObj(cfg), Statics::instance().hWindow);
	d->Init();

	*result = d;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE System::addIdleHandler(IDispatch* cfg){
	if (cfg != NULL){
		IDispatchEx* cfgEx = MyActiveSite::queryDispatchEx(cfg);

		if (cfgEx){
			ForegroundIdleHook::getInstance()->add( new ScriptedIdleHandler(cfgEx) );
		}
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE System::addScript(BSTR* value, VARIANT* name){
	if (value != NULL){
		BSTR name_ = NULL;
		if (name && name->vt == VT_BSTR)
			name_ = name->bstrVal;

		m_ActiveSite.runScript(*value, name_);
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE System::setTimeout(IDispatch* cfg){
	return Timer::GetInstance()->AddHandler(MyActiveSite::getInstance()->WrapScriptObj(cfg)) ? S_OK : E_FAIL;
}

HRESULT STDMETHODCALLTYPE System::put_clipBoard( BSTR *value){

	if (OpenClipboard(NULL)) { // NULL - open task clipboard

		BSTR str = *value;
		int newlen = SysStringByteLen(*value);

		HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, newlen + sizeof(TCHAR)); 
		if (hglbCopy == NULL){
			MyActiveSite::Throw(TEXT("Can't copy to clipboard"), __uuidof(ISystem));
			return E_FAIL; 
		}

		char*  lptstrCopy = (char*) GlobalLock(hglbCopy); 

        memcpy(lptstrCopy, *value, newlen); 
		*(TCHAR*)(&lptstrCopy[newlen]) = 0; // end of line

        BOOL guerr = GlobalUnlock(hglbCopy); 

		// Place the handle on the clipboard. 
        HANDLE res = SetClipboardData(CF_UNICODETEXT, hglbCopy); 
			
		CloseClipboard(); 
	}

	return S_OK;

}

HRESULT STDMETHODCALLTYPE System::get_clipBoard( BSTR *result){

	if (IsClipboardFormatAvailable(CF_UNICODETEXT) && OpenClipboard(NULL)) {
		HGLOBAL hglb = GetClipboardData(CF_UNICODETEXT); 
		if (hglb != NULL) 
		{ 
			LPTSTR lptstr = (LPTSTR)GlobalLock(hglb); 
			if (lptstr != NULL) 
			{ 
				*result = SysAllocString(lptstr);  
				GlobalUnlock(hglb); 
			} 
		} else{
			LastError errMsg;
		}

		CloseClipboard(); 
	}

	if (*result==NULL)
		*result = SysAllocString(TEXT(""));
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE System::loadLibrary(BSTR* libraryPath, ILibrary** result){
	Library* lib = new Library(libraryPath);
	if (lib->handle() == 0){
		*result = NULL;
		lib->Release();

		LastError err;

		MyActiveSite::Throw(err.message(), __uuidof(ISystem) );

		return E_FAIL;
	} else {
		*result = lib;
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE System::registerCallBack( IDispatch *cfg, unsigned int stacksize, ICallBack **result){
	CallBack* cb = CallBack::RegisterCallBack(MyActiveSite::queryDispatchEx(cfg),stacksize);
	if (cb==NULL){
		MyActiveSite::Throw(TEXT("All callbacks are in using."), __uuidof(ISystem) );
		return E_FAIL;
	}

	*result = static_cast<ICallBack*>(cb);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE System::get_scriptFullName( BSTR *result){
	*result = m_ScriptFullName.Clone();

	return S_OK;
}

HRESULT STDMETHODCALLTYPE System::showNotification(IDispatch* cfg){
	LocRef<IUserNotification> notification;
	if (FAILED(CoCreateInstance(CLSID_UserNotification, NULL, CLSCTX_INPROC_SERVER, __uuidof(IUserNotification), (void**)&notification)))
		return E_NOTIMPL;

	class CloseRequest : public CComBase<IQueryContinue>{
		HRESULT STDMETHODCALLTYPE QueryContinue(VOID){
			if (false)
				return S_OK;
			else
				return S_FALSE;
		}

	};
	LocRef<IQueryContinue> cont = new CloseRequest();

	HRESULT hr = notification->SetBalloonInfo(TEXT("Titel"), TEXT("message"), NIIF_NONE );
	hr = notification->SetBalloonRetry(0,250,0);
	hr = notification->Show(cont, 150);

	return S_OK;
}

int System::getAcceleratorModifier(ScriptObj* cfgEx){
	int modifier = 0;

	VARIANT* modifierBOOL = cfgEx->getProperty(TEXT("shift"), VT_BOOL);
	if (modifierBOOL != NULL){
		modifier = modifier | (modifierBOOL->boolVal)? FSHIFT : 0;
		VariantClear(modifierBOOL);
		delete modifierBOOL;
	}

	modifierBOOL = cfgEx->getProperty(TEXT("alt"), VT_BOOL);
	if (modifierBOOL != NULL){
		modifier = modifier | ((modifierBOOL->boolVal)? FALT : 0);
		VariantClear(modifierBOOL);
		delete modifierBOOL;
	}

	modifierBOOL = cfgEx->getProperty(TEXT("ctrl"), VT_BOOL);
	if (modifierBOOL != NULL){
		modifier = modifier | ((modifierBOOL->boolVal)? FCONTROL : 0);
		VariantClear(modifierBOOL);
		delete modifierBOOL;
	}

	return modifier;
}

WORD System::getKeyCode(ScriptObj* cfgEx){
	WORD keyCode = 0;

	VARIANT* keySTR = cfgEx->getProperty(TEXT("key"), -1);

	if (keySTR != NULL){
		if (keySTR->vt == VT_BSTR && SysStringLen(keySTR->bstrVal) > 0){
			keyCode =  VkKeyScan(keySTR->bstrVal[0]);
		} else if (keySTR->vt == VT_INT){
			keyCode = keySTR->intVal;
		} else if (keySTR->vt == VT_I4){
			keyCode = LOWORD(keySTR->lVal);
		} else {
			keyCode = keyCode;
		}

		VariantClear(keySTR);
		delete keySTR;
	}
	return keyCode;
}




