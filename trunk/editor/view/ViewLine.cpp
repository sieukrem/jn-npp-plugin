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
#include "ViewLine.h"
#include "EditorView.h"
#include "common\MyActiveSite.h"

#define CHECK_AND_FAIL 	if (!CheckValidity()) return E_FAIL;

ViewLine::ViewLine(int number, CEditorView* v):CComDispatch(){
	m_View = v;
	m_number = number;

	m_View->AddRef();
}

ViewLine::~ViewLine(void)
{
	m_View->Release();
}

bool ViewLine::CheckValidity(){
	int count = m_View->sci(SCI_GETLINECOUNT, 0, 0);
	
	// allright
	if (m_number < count)
		return true;

	// number out of range
	TCHAR buf[100];
	wsprintf(buf, TEXT("Line number %d out of range [0..%d)"), m_number, count);

	MyActiveSite::Throw(buf, __uuidof(IViewLine) );

	return false;
}

long ViewLine::Length(){
	return m_View->sci(SCI_LINELENGTH, m_number, 0);
}

HRESULT STDMETHODCALLTYPE ViewLine::get_number(int *value){
	*value = m_number;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE ViewLine::get_start( int *value){
	int bp;
	
	if(FAILED(get_byteStart(&bp)))
		return E_FAIL;
	
	if (bp>=0)
		*value = m_View->bytePosToPos(bp);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE ViewLine::get_end( int *value){

	int bp;
	
	if (FAILED(get_byteEnd(&bp)))
		return E_FAIL;
	
	if (bp>=0)
		*value = m_View->bytePosToPos(bp);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE ViewLine::get_byteStart( int *value){

	CHECK_AND_FAIL;

	*value = m_View->sci(SCI_POSITIONFROMLINE,m_number,0);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE ViewLine::get_byteEnd( int *value){

	CHECK_AND_FAIL;

	int byteStart;
	get_byteStart(&byteStart); 

	*value = byteStart + Length();

	return S_OK;
}

HRESULT STDMETHODCALLTYPE ViewLine::get_text( BSTR *result){

	CHECK_AND_FAIL;

	LRESULT len = Length();
	char* buf = new char[len+1];
	buf[len] = 0;

	m_View->sci(SCI_GETLINE, m_number, (LPARAM)buf);

	*result = m_View->adjustFromCodePage(buf, -1);
	
	delete[] buf;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE ViewLine::put_text( BSTR *value){

	CHECK_AND_FAIL;

	int start, end;

	get_byteStart(&start);
	get_byteEnd(&end);

	m_View->put_bytePos(start);
	m_View->put_byteAnchor(end);

	m_View->put_selection(value);

	return S_OK ;
}
