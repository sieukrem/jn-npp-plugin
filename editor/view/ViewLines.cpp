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
#include "ViewLines.h"
#include "ViewLine.h"

#include "EditorView.h"
#include "common\MyActiveSite.h"
 
ViewLines::~ViewLines(){
	m_View->Release();
}

ViewLines::ViewLines(CEditorView* v):CComDispatch(){
	m_View = v;
	m_View->AddRef();
}

HRESULT STDMETHODCALLTYPE ViewLines::get( int line, IViewLine **result){
	*result = new ViewLine(line, m_View);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE ViewLines::get_count(int* result){
	*result = m_View->sci(SCI_GETLINECOUNT, 0, 0);
	return S_OK;
}


HRESULT STDMETHODCALLTYPE ViewLines::get_current( int *result){
	int pos = m_View->sci(SCI_GETCURRENTPOS,0,0);
	*result = m_View->sci(SCI_LINEFROMPOSITION,pos,0);
	return S_OK;						
}

HRESULT STDMETHODCALLTYPE ViewLines::put_current(int value){
	int count = m_View->sci(SCI_GETLINECOUNT, 0, 0);
	if (value >= count || value < 0){
		TCHAR buf[100];
		wsprintf(buf, TEXT("Line number %d out of range [0..%d)"), value, count);

		MyActiveSite::Throw(buf, __uuidof(IViewLine) );

		return E_FAIL;
	}

	m_View->sci(SCI_GOTOLINE,value,0);
	return S_OK;						
}
