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
#include "ActiveScriptSiteDebug.h"
#include <strsafe.h>
#include <shlwapi.h>
#include "SysStr.h"

class ActiveScriptSiteDebugExImpl : public CComBase<IActiveScriptSiteDebugEx>{
public:
	virtual HRESULT STDMETHODCALLTYPE OnCanNotJITScriptErrorDebug(IActiveScriptErrorDebug *pErrorDebug, BOOL *pfCallOnScriptErrorWhenContinuing){
		*pfCallOnScriptErrorWhenContinuing = true;
		return S_OK;
	}
};

class DebugDocumentHostImpl : public CComBase<IDebugDocumentHost>{
private:
	SysStr m_FilePath;
	LocRef<IActiveScriptDebug, true> m_ActiveScriptDebug;
public:
	DebugDocumentHostImpl(const BSTR filePath, IActiveScriptDebug* activeScriptDebug):m_FilePath(filePath), m_ActiveScriptDebug(activeScriptDebug){
	}
	virtual HRESULT STDMETHODCALLTYPE GetDeferredText( 
		/* [in] */ DWORD dwTextStartCookie,
		/* [size_is][length_is][out][in] */ __RPC__inout_ecount_part(cMaxChars, *pcNumChars) WCHAR *pcharText,
		/* [size_is][length_is][out][in] */ __RPC__inout_ecount_part(cMaxChars, *pcNumChars) SOURCE_TEXT_ATTR *pstaTextAttr,
		/* [out][in] */ __RPC__inout ULONG *pcNumChars,
		/* [in] */ ULONG cMaxChars){
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetScriptTextAttributes( 
		/* [size_is][in] */ __RPC__in_ecount_full(uNumCodeChars) LPCOLESTR pstrCode,
		/* [in] */ ULONG uNumCodeChars,
		/* [in] */ __RPC__in LPCOLESTR pstrDelimiter,
		/* [in] */ DWORD dwFlags,
		/* [size_is][out][in] */ __RPC__inout_ecount_full(uNumCodeChars) SOURCE_TEXT_ATTR *pattr){
		
		if (!m_ActiveScriptDebug)		
			return E_NOTIMPL;

		return m_ActiveScriptDebug->GetScriptTextAttributes(pstrCode, uNumCodeChars, pstrDelimiter, dwFlags, pattr);
	}

	virtual HRESULT STDMETHODCALLTYPE OnCreateDocumentContext(IUnknown **ppunkOuter){
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetPathName(BSTR *pbstrLongName, BOOL *pfIsOriginalFile){
		*pfIsOriginalFile = true;
		*pbstrLongName = SysAllocString(m_FilePath);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetFileName(BSTR *pbstrShortName){
		*pbstrShortName = SysAllocString(PathFindFileName(m_FilePath)); 
		return (*pbstrShortName == NULL)? E_FAIL : S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE NotifyChanged(){
		return E_NOTIMPL;
	}
};


ActiveScriptSiteDebug::ActiveScriptSiteDebug(TCHAR *appName, IActiveScript* as):m_ActiveScript(as){

	if (FAILED(CoCreateInstance(
		CLSID_ProcessDebugManager, NULL,
		CLSCTX_INPROC_SERVER | /*CLSCTX_INPROC_HANDLER |*/ CLSCTX_LOCAL_SERVER,
        __uuidof(IProcessDebugManager), (void **)m_Pdm)))
		throw TEXT("PDM creation failed");

	if (FAILED(m_Pdm->CreateApplication(&m_App)))
		throw TEXT("Debug application creation failed");

	HRESULT res = m_App->SetName(appName);

	if (FAILED(m_Pdm->AddApplication(m_App, &m_AppCookie)))
		throw TEXT("Debug application adding failed");
}

ActiveScriptSiteDebug::~ActiveScriptSiteDebug(){
	for(Docs::const_iterator it=m_Docs.begin(); it!=m_Docs.end(); ++it){
		it->second->Detach();
		it->second->Release();
	}

	if (FAILED(m_Pdm->RemoveApplication(m_AppCookie)))
		throw "Debug application removing failed";

	if (m_App)
		m_App->Close();

}

DWORD ActiveScriptSiteDebug::AddScript(BSTR script, BSTR name){
	IDebugDocumentHelper* doc;
	m_Pdm->CreateDebugDocumentHelper(NULL, &doc);

	if (!doc)
		return 0;

	DWORD scriptAttr = 0;
	TCHAR buf[100];
	if (name == 0){
		StringCbPrintf(buf, sizeof(buf) , TEXT("local_%d.js"), doc);
		name = buf;
		scriptAttr = TEXT_DOC_ATTR_READONLY;
	}

	HRESULT res;

	res = doc->Init(m_App, name, name, scriptAttr);
	res = doc->Attach(NULL);

	res = doc->SetDocumentAttr(scriptAttr);

	// Syntax highlighting via script engine and right file names
	LocRef<IActiveScriptDebug> asd;
	if ((res = m_ActiveScript->QueryInterface(__uuidof(IActiveScriptDebug), (void**)asd))==S_OK){
		LocRef<DebugDocumentHostImpl> ddh(new DebugDocumentHostImpl(name, asd));
		res = doc->SetDebugDocumentHost(ddh);
	}

	res = doc->AddUnicodeText(script);

	DWORD srcContext = 0;
	res = doc->DefineScriptBlock(0, SysStringLen(script), m_ActiveScript, false, &srcContext);

	m_Docs.insert(Docs::value_type(srcContext, doc));

	return srcContext;
}

HRESULT ActiveScriptSiteDebug::GetApplication(IDebugApplication **ppda){
	if (!ppda)
		return E_POINTER;

	*ppda = static_cast<IDebugApplication*>(m_App);
	(*ppda)->AddRef();

	return S_OK;
}

HRESULT ActiveScriptSiteDebug::GetDocumentContextFromPosition(DWORD dwSourceContext, ULONG uCharacterOffset, ULONG uNumChars, IDebugDocumentContext **ppsc){
	if (!ppsc) 
		return E_POINTER; 

	Docs::iterator it=m_Docs.find(dwSourceContext);
	*ppsc = NULL; 
	if (it==m_Docs.end())
		return E_UNEXPECTED; 

	IDebugDocumentHelper* doc = it->second;
	
	// every running document has a special "cookie" associated with it. 
	// this code assumes only 1 document with a cookie value stored in 
	// m_dwDocCookie. It then asks the helper interface IDebugDocumentHelper 
	// to convert from a character offset to a document context interface. 
	ULONG ulStartPos=0; 
	HRESULT hr = doc->GetScriptBlockInfo(dwSourceContext,NULL,&ulStartPos,NULL);
	if ( SUCCEEDED(hr) ) 
		return doc->CreateDebugDocumentContext( ulStartPos + uCharacterOffset, uNumChars, ppsc); 

	return hr; 
}

HRESULT ActiveScriptSiteDebug::GetRootApplicationNode(IDebugApplicationNode **ppdanRoot){
	if (!ppdanRoot) 
		return E_POINTER; 

	*ppdanRoot = NULL;

	return S_OK;
}

HRESULT ActiveScriptSiteDebug::OnScriptErrorDebug(IActiveScriptErrorDebug *pErrorDebug, BOOL *pfEnterDebugger, BOOL *pfCallOnScriptErrorWhenContinuing){
	*pfEnterDebugger=FALSE; 
	*pfCallOnScriptErrorWhenContinuing=TRUE;
	return S_OK; 
}

HRESULT ActiveScriptSiteDebug::QueryInterface(REFIID riid, LPVOID *ppv){
	if (S_OK == CComBase<IActiveScriptSiteDebug>::QueryInterface(riid, ppv))
		return S_OK;

	
	if (IsEqualIID(riid, __uuidof(IActiveScriptSiteDebugEx))){
		LocRef<ActiveScriptSiteDebugExImpl> debEx(new ActiveScriptSiteDebugExImpl());
		return debEx->QueryInterface(riid, ppv);
	}

	return E_NOINTERFACE;
} 
