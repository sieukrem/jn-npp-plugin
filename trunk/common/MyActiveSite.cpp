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
#include "MyActiveSite.h"
#include <stdarg.h>
#include "SysStr.h"
 
EXTERN_C const GUID DECLSPEC_SELECTANY CLSID_JSScript = { 0xf414c260, 0x6ac0, 0x11cf,  {0xb6,0xd1,0x0, 0xaa, 0x0, 0xbb, 0xbb, 0x58 } };

MyActiveSite::MyActiveSite(){

	AddRef();

	HRESULT res = CoCreateInstance(
		CLSID_JSScript, 
		NULL, CLSCTX_INPROC_SERVER,
		IID_IActiveScript,
		(void **)&m_ActiveScript
	);

	res = m_ActiveScript->QueryInterface(IID_IActiveScriptParse, (void **)&m_ActiveScriptParse);

	// set newest language feature set 2 == (5.8)
	IActiveScriptProperty* prop;
	res = m_ActiveScript->QueryInterface(IID_IActiveScriptProperty, (void **)&prop);
	if (res == S_OK){
		DWORD _SCRIPTPROP_INVOKEVERSIONING = 0x00004000;
		VARIANT v;
		v.intVal = 2;
		v.vt = VT_I4;
		res = prop->SetProperty( _SCRIPTPROP_INVOKEVERSIONING, NULL, &v);
		prop->Release();
	}


	//Передаем скрипт-машине наш интерфейс IActiveScriptSite...
	//Заметьте что до этого момента счетчик ссылок объекта pMySite
	//(pMySite->m_dwRef) равен нулю. Скрипт-машина, действуя в 
	//соответствии с законами COM, увеличивает счетчик ссылок у pMySite 
	//на единицу и автоматически становится единственным объектом, имеющим
	//ссылку на pMySite. Так что когда скрипт-машина освободит
	//ссылку на pMySite, он (pMySite) автоматически уничтожится.
	//НЕ ЗАБУДЬТЕ вызвать activeScript->Close(), иначе pMySite, 
	//а зачастую и MyObject, будут жить вечно ;-).
	res = m_ActiveScript->SetScriptSite(this);

	// Дадим скрипт-машине шанс инициализироваться...
	res = m_ActiveScriptParse->InitNew();
}

void MyActiveSite::CloseScript(){

	for (ScriptedObjects::iterator it=m_ScriptObjects.begin(); it!=m_ScriptObjects.end(); ++it){
		it->second->Release();
	}
	
	m_ScriptObjects.clear(); 

	if (m_ActiveScript!=NULL){
		m_ActiveScript->Close();
	}
}
void MyActiveSite::ReleaseScriptElement(IDispatchEx** elem){
	SCRIPTSTATE scrstate;
	HRESULT res = m_ActiveScript->GetScriptState(&scrstate);
	if (res==S_OK && scrstate==SCRIPTSTATE_STARTED && elem != NULL && *elem!=NULL){
		(*elem)->Release();
	}
	*elem = NULL;
}

void MyActiveSite::Throw(TCHAR* errmsg, REFGUID refguid){
	ICreateErrorInfo *pcerrinfo;
	IErrorInfo *perrinfo;

	SysStr errMsg(errmsg);

	HRESULT hr;
	hr = CreateErrorInfo(&pcerrinfo);
	if (SUCCEEDED(hr)){
		pcerrinfo->SetDescription(errMsg);
		pcerrinfo->SetGUID(refguid);

		hr = pcerrinfo->QueryInterface(IID_IErrorInfo, (LPVOID FAR*) &perrinfo);
		if (SUCCEEDED(hr)){
		  SetErrorInfo(0, perrinfo);
		  perrinfo->Release();
		}

		pcerrinfo->Release();
	}
}

MyActiveSite::~MyActiveSite(void){
	if (m_ActiveScript!=NULL){
//			AddRef();// чтобы не был вызван деструктор еще раз
//			m_ActiveScript->Close();
		m_ActiveScript->Release();
		m_ActiveScript = NULL;
	}
	if (m_ActiveScriptParse!=NULL){
		m_ActiveScriptParse->Release();
		m_ActiveScriptParse = NULL;
	}

}

MyActiveSite* MyActiveSite::getInstance(){
	static MyActiveSite instance;
	return &instance;
}


void MyActiveSite::addNamedItem(TCHAR* name, IUnknown* item, bool global){
	// Добавляем MyObject в пространство имен скрипт-машины...
	HRESULT hr; // SCRIPTITEM_GLOBALMEMBERS 


	m_ScriptObjects.insert(ScriptedObjects::value_type(name, item));

	item->AddRef();
	
	int flags = SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISSOURCE;
	if (global)
		flags |= SCRIPTITEM_GLOBALMEMBERS;

	hr = m_ActiveScript->AddNamedItem(name, flags);
}
IDispatchEx* MyActiveSite::queryDispatchEx(IDispatch* obj){
	IDispatchEx* result;
	HRESULT hr = obj->QueryInterface(IID_IDispatchEx,(void**) &result);
	return result;
}
IDispatchEx* MyActiveSite::createObj(TCHAR* ctorName){
	// Create object by calling "Object" constructor
	HRESULT hr;
	DISPID*	dispid;
	VARIANT var;
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};

	IDispatch* disActiveScript;
	hr = m_ActiveScript->GetScriptDispatch(NULL, (IDispatch**) &disActiveScript);

	IDispatchEx* dexActiveScript = this->queryDispatchEx(disActiveScript);
	disActiveScript->Release();
	
	dispid = getDISPID(ctorName, dexActiveScript);
	if (dispid){
		hr = dexActiveScript->InvokeEx(
				*dispid, 
				LOCALE_USER_DEFAULT, 
				DISPATCH_CONSTRUCT,
				&dispparamsNoArgs,
				&var,
				NULL,
				NULL);
		delete dispid;
	}

	dexActiveScript->Release();

	IDispatchEx* result = this->queryDispatchEx(var.pdispVal); 
	//hr = var.pdispVal->QueryInterface(IID_IDispatchEx,(void**) &result);
	VariantClear(&var);

	return result;
}

DISPID* MyActiveSite::getDISPID(TCHAR* name, IDispatchEx* obj){
	DISPID dispid;
	BSTR mod = SysAllocString((const OLECHAR*)name);
	HRESULT hr = obj->GetDispID(mod, 0, &dispid);
	SysFreeString(mod);
	return (hr == S_OK)?new DISPID(dispid):NULL;		
}

VARIANT* MyActiveSite::getProperty(TCHAR* name, IDispatchEx* obj, int as){
	VARIANT* result = NULL;
	DISPID* dispid = getDISPID(name, obj);
	if (dispid != NULL){
		result = new VARIANT();
		DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
		HRESULT hr = obj->InvokeEx(*dispid, LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET, &dispparamsNoArgs,result, NULL, NULL);
		delete dispid;

		if	( hr != S_OK || // there is not this property
			( (result->vt & as) != as ) &&  // property is another types than expected
			( as != -1 ) )	// property type does care
		{
			VariantClear( result );
			delete result;
			result = NULL;
		}

	}

	return result;
}

BOOL MyActiveSite::callMethod(TCHAR* method, IDispatchEx* obj){
	DISPID* dispid = getDISPID(method, obj);
	if (dispid){
		// Invoke method with "this" pointer
		VARIANTARG var[1];
		var[0].vt = VT_DISPATCH;
		var[0].pdispVal = obj;

		DISPID putid = DISPID_THIS;
		DISPPARAMS dispparams;
		dispparams.rgvarg = var;
		dispparams.rgdispidNamedArgs = &putid;
		dispparams.cArgs = 1;
		dispparams.cNamedArgs = 1;
		HRESULT res = obj->InvokeEx(*dispid, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, NULL, NULL, NULL);
		delete dispid;
	}
	return dispid != NULL;
}

BOOL MyActiveSite::callMethod(TCHAR* method, IDispatchEx* obj, int value){
	VARIANTARG arg;
	arg.intVal = value;
	arg.vt		= VT_INT;
	return callMethod(method, obj, &arg,1);
}

BOOL MyActiveSite::callMethod(TCHAR* method, IDispatchEx* obj, BSTR value){
	VARIANTARG arg;
	arg.bstrVal = value;
	arg.vt		= VT_BSTR;
	return callMethod(method, obj, &arg,1);
}
// cleans arg at the end
BOOL MyActiveSite::callMethod(TCHAR* method, IDispatchEx* obj,VARIANTARG* arg, int count, VARIANT* result){ 
	DISPID* dispid = getDISPID(method, obj);
	if (dispid){

		// Invoke method with "this" pointer
		VARIANTARG* var = new VARIANTARG[1+count];
		var[0].vt = VT_DISPATCH;
		var[0].pdispVal = obj;

		// revert positions of args
		for (int i=1,j=count-1; i <= count; i++, j--)
			var[i] = arg[j];

		DISPID putid = DISPID_THIS;
		DISPPARAMS dispparams;
		dispparams.rgvarg = var;
		dispparams.rgdispidNamedArgs = &putid;
		dispparams.cArgs = 1 + count;
		dispparams.cNamedArgs = 1;
		HRESULT res = obj->InvokeEx(*dispid, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, result, NULL, NULL);
		delete dispid;
		delete var;
	} 

	for(int i=0; i<count; i++)
		VariantClear(&arg[i]);

	return dispid != NULL;
}
void MyActiveSite::pushValueInToArr(IDispatchEx* arr, TCHAR* value){
	callMethod(TEXT("push"),arr,SysAllocString((const OLECHAR*)value));
}

void MyActiveSite::Connect(){
   HRESULT hr = m_ActiveScript->SetScriptState(SCRIPTSTATE_CONNECTED);

   if(FAILED(hr))
   {
	   m_ActiveScript->Close();
   }
}

void MyActiveSite::runScript(BSTR script){
   EXCEPINFO ei;
   HRESULT hr = m_ActiveScriptParse->ParseScriptText((LPCOLESTR)script, NULL/*OLESTR("MyObject")*/, NULL, NULL, 0, 0, 0L, NULL, &ei);
}

// Методы IActiveScriptSite...
HRESULT __stdcall MyActiveSite::GetLCID(LCID *plcid) 
{
  return S_OK; 
}

HRESULT __stdcall MyActiveSite::GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppunkItem, ITypeInfo **ppti) 
{
	if(ppti) 
	{
		*ppti = NULL;

		// Если просят ITypeInfo... 
		if(dwReturnMask & SCRIPTINFO_ITYPEINFO) 
		{
		// *ppti = m_spTypeInfo;
		// (*ppti)->AddRef();
		}
	}
  
	// Если Windows Scripting разместил ppunkItem...
	if(!ppunkItem)
	  return S_OK;

	*ppunkItem = NULL;

	// Если Windows Scripting требует IUnknown...
	if(!(dwReturnMask & SCRIPTINFO_IUNKNOWN))
		return S_OK;

	ScriptedObjects::iterator findIt = m_ScriptObjects.find((TCHAR*)pstrName);
	if (findIt!=m_ScriptObjects.end()){
		*ppunkItem = findIt->second;
		(*ppunkItem)->AddRef();
	}
	
	return S_OK;
}

HRESULT __stdcall MyActiveSite::GetDocVersionString(BSTR *pbstrVersion) 
{
  return S_OK;
}

HRESULT __stdcall MyActiveSite::OnScriptTerminate(  const VARIANT *pvarResult, const EXCEPINFO *pexcepInfo) 
{
  return S_OK;
}

HRESULT __stdcall MyActiveSite::OnStateChange(SCRIPTSTATE ssScriptState) 
{
  return S_OK;
}

HRESULT __stdcall MyActiveSite::OnScriptError(IActiveScriptError *pscriptError) 
{
  // Это сообщение появится в случае ошибки в скрипте.
  // Более подробная информация в pscriptError.
  EXCEPINFO ei;
  HRESULT hr = pscriptError->GetExceptionInfo(&ei);
  if (hr == S_OK){
	  ULONG ln;
	  LONG cp;
	  DWORD co;
	  hr = pscriptError->GetSourcePosition(&co, &ln, &cp);

	  TCHAR buf[1024];
	  TCHAR* msg = ei.bstrDescription;
	  if (SysStringLen(ei.bstrDescription) < (1024 - 13 - 10 - 10)){
		wsprintf(buf, TEXT("%s\nline: %d, pos: %d"), ei.bstrDescription, ln+1, cp+1);
		msg = buf;
	  }

	  ::MessageBoxW(::GetActiveWindow(), msg, ei.bstrSource, MB_SETFOREGROUND|MB_ICONERROR);

	  SysFreeString(ei.bstrDescription);
	  SysFreeString(ei.bstrHelpFile);
	  SysFreeString(ei.bstrSource);
  }
  return S_OK;
}

HRESULT __stdcall MyActiveSite::OnEnterScript(void) 
{
  return S_OK;
}

HRESULT __stdcall MyActiveSite::OnLeaveScript(void) 
{
  return S_OK;
}
