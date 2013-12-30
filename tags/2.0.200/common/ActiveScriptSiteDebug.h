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

#include <ActivScp.h>
#include <map>

#include "combase.h"
#include "Scripting\activdbg.h"


class ActiveScriptSiteDebug : public CComBase<IActiveScriptSiteDebug>
{
private:
	LocRef<IActiveScript, true> m_ActiveScript;
	IProcessDebugManager*       m_Pdm; // because of permanent deadlocks
	IDebugApplication*		    m_App; // make them without LocRef
	DWORD                       m_AppCookie;
	typedef std::map<DWORD, IDebugDocumentHelper*> Docs;
	Docs m_Docs;
public:
	#pragma region IActiveScriptSiteDebug
    HRESULT STDMETHODCALLTYPE GetDocumentContextFromPosition( 
		DWORD dwSourceContext,
		ULONG uCharacterOffset,
		ULONG uNumChars,
		IDebugDocumentContext** ppsc);
    
    HRESULT STDMETHODCALLTYPE GetApplication(IDebugApplication** ppda);
    
    HRESULT STDMETHODCALLTYPE GetRootApplicationNode(IDebugApplicationNode** ppdanRoot);
    
    HRESULT STDMETHODCALLTYPE OnScriptErrorDebug( 
		IActiveScriptErrorDebug* pErrorDebug,
		BOOL* pfEnterDebugger,
		BOOL* pfCallOnScriptErrorWhenContinuing);
	#pragma endregion

	DWORD AddScript(BSTR script, BSTR name);

	ActiveScriptSiteDebug(TCHAR* appName, IActiveScript* as);
	~ActiveScriptSiteDebug();
};
