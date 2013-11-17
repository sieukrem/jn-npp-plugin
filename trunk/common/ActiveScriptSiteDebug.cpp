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

ActiveScriptSiteDebug::ActiveScriptSiteDebug(TCHAR *appName, IActiveScript* as):m_ActiveScript(as){

	if (FAILED(CoCreateInstance(
		CLSID_ProcessDebugManager, NULL,
		CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER | CLSCTX_LOCAL_SERVER,
        __uuidof(IProcessDebugManager), (void **)&m_Pdm)))
		throw "PDM creation failed";

	if (FAILED(m_Pdm->CreateApplication(&m_App)))
		throw "Debug application creation failed";

	HRESULT res = m_App->SetName(appName);

	if (FAILED(m_Pdm->AddApplication(m_App, &m_AppCookie)))
		throw "Debug application adding failed";


}

ActiveScriptSiteDebug::~ActiveScriptSiteDebug(){
	for(Docs::const_iterator it=m_Docs.begin(); it!=m_Docs.end(); ++it){
		it->second->Detach();
		it->second->Release();
	}

	if (m_App)
		m_App->Close();

	if (FAILED(m_Pdm->RemoveApplication(m_AppCookie)))
		throw "Debug application removing failed";
}

DWORD ActiveScriptSiteDebug::AddScript(BSTR script, BSTR name){
	IDebugDocumentHelper* doc;
	m_Pdm->CreateDebugDocumentHelper(NULL, &doc);

	if (!doc)
		return 0;

	TCHAR buf[100];
	if (name == 0){
		StringCbPrintf(buf, sizeof(buf) , TEXT("local_%d.js"), doc);
		name = buf;
	}

	HRESULT res = doc->Init(m_App, name, name, TEXT_DOC_ATTR_READONLY);
	res = doc->Attach(NULL);
	res = doc->AddUnicodeText(script);

	DWORD srcContext = 0;
	res = doc->DefineScriptBlock(0, SysStringLen(script), m_ActiveScript, false, &srcContext);
	m_Docs.insert(Docs::value_type(srcContext, doc));

	return srcContext;
}

HRESULT ActiveScriptSiteDebug::GetApplication(IDebugApplication **ppda){
	if (!ppda)
		return E_POINTER;

	m_App.CopyTo(ppda);

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
