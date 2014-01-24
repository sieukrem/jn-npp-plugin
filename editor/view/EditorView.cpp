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
#include "common\MyActiveSite.h"

#include "EditorView.h"
#include "ViewLines.h"

#pragma warning (disable : 4355)
CEditorView::CEditorView(int id):CComDispatch(),m_IndPanel(this), SCIView((((id-1)==0)? nppData._scintillaMainHandle : nppData._scintillaSecondHandle)){
	m_Id = id;

	m_Listener = NULL;
}


int CEditorView::getBufferId(){
	int curDoc =(int) NPPM(GETCURRENTDOCINDEX, 0, m_Id-1);
	return (int)NPPM(GETBUFFERIDFROMPOS, curDoc, m_Id-1);		
}

BSTR CEditorView::adjustFromCodePage(char* buf, int len){
	int cp;  get_codepage(&cp);
	BSTR str;

	// if len == -1 counts chars including terminating 0
	int newlen = MultiByteToWideChar(cp, 0, buf, len, NULL,0);
	if (len == -1)
		str = SysAllocStringLen(NULL, newlen-1);
	else
		str = SysAllocStringLen(NULL, newlen);

	int res = MultiByteToWideChar(cp, 0, buf, len, str, newlen);

	return str;
}

char* CEditorView::adjustToCodePage(BSTR str){
	int cp;  get_codepage(&cp);
	char* result;

	int newlen = WideCharToMultiByte(cp, 0, (LPCWSTR)str, SysStringLen(str), NULL,0, NULL, NULL);
	result = new char[newlen+1];
	int res = WideCharToMultiByte(cp, 0, (LPCWSTR)str, SysStringLen(str), result, newlen+1, NULL,NULL);
	result[res] = 0;

	return result;		
}

int CEditorView::posToBytePos(int value){
	BSTR text;	get_text(&text);

	int cp;  get_codepage(&cp);

	int pos = WideCharToMultiByte(cp,0,text, value, NULL,0,NULL,NULL);

	SysFreeString(text);

	return pos;
}


int CEditorView::bytePosToPos(int pos){
	/*
		because position is a byte position, get text from 0 to pos
		and calculate character count regarding codepage
	*/
	char* buf = (char*)	sci(SCI_GETCHARACTERPOINTER, 0, 0);

	int cp;  get_codepage(&cp);

	int result = MultiByteToWideChar(cp, 0, buf, pos, NULL,0);

	return result;
}

void CEditorView::setIndicatorLinesUpdater(int begin, int end){

	ForegroundIdleHook::IdleHandler* indicatorPixelsUpdater = &m_IndPanel.m_IndicPixelsUp;

	if (indicatorPixelsUpdater != NULL)
		ForegroundIdleHook::getInstance()->remove(indicatorPixelsUpdater);

	if (m_IndPanel.m_IndicLinesUp.m_Begin > begin || (m_IndPanel.m_IndicLinesUp.m_End == 0 /* first time */))
		m_IndPanel.m_IndicLinesUp.m_Begin = begin;

	if (end == -1 || m_IndPanel.m_IndicLinesUp.m_End < end )
		m_IndPanel.m_IndicLinesUp.m_End = end;

	ForegroundIdleHook::getInstance()->add(&m_IndPanel.m_IndicLinesUp);
}

void CEditorView::DoMessage(SCNotification* eventArgs){
	switch(eventArgs->nmhdr.code){
		case SCN_MODIFIED:
			if (eventArgs->modificationType & (SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT)){
				if (eventArgs->linesAdded != 0) // changes in more than one line
					setIndicatorLinesUpdater(eventArgs->position,-1); // from position to bottom
				else // changes in the single line, update only necessary positions
					setIndicatorLinesUpdater(eventArgs->position, eventArgs->position + eventArgs->length);
			} else if (eventArgs->modificationType & ( SC_MOD_CHANGEINDICATOR)){
				// update only necessary positions
				setIndicatorLinesUpdater(eventArgs->position, eventArgs->position  + eventArgs->length);
			}
			break;
		case NPPN_BUFFERACTIVATED:
			// update all lines
			setIndicatorLinesUpdater(-1,-1);
			break;
		case SCN_ZOOM:{
			// update only pixels
			ForegroundIdleHook::getInstance()->add(&m_IndPanel.m_IndicPixelsUp);
			break;
		}
		case SCN_MARGINCLICK:
			if (eventArgs->margin == 2){ // click above fold symbol
				setIndicatorLinesUpdater(-1,-1);
			}
			break;
	}
}


HRESULT STDMETHODCALLTYPE CEditorView::get_text( BSTR *result){
	char* buf = (char*)sci(SCI_GETCHARACTERPOINTER, 0, 0);
	*result = adjustFromCodePage(buf, -1);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditorView::put_text(BSTR *value){

	char* buf = adjustToCodePage(*value);

	sci(SCI_SETTEXT, 0, (LPARAM)buf);

	delete buf;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditorView::get_files( long *value){
	*value = ::SendMessage(nppData._nppHandle, NPPM_GETNBOPENFILES, 0, m_Id);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditorView::get_selection( BSTR *value){

	LRESULT len = sci(SCI_GETSELTEXT, 0, 0);
	char* buf = new char[len];

	sci(SCI_GETSELTEXT, len, (LPARAM)buf);

	*value = adjustFromCodePage(buf, -1);  
	delete[] buf;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditorView::put_selection(BSTR *value){
	char* buf = adjustToCodePage(*value);

	sci(SCI_REPLACESEL, 0, (LPARAM)buf);
	delete[] buf;
	 
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditorView::get_codepage(int *value){

	*value = sci(SCI_GETCODEPAGE, 0, 0);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditorView::put_codepage(int value){
	sci(SCI_SETCODEPAGE, (WPARAM)value,0);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditorView::get_files(IDispatch **result){

	// prepare for getting filenames
	long nbFiles;
	get_files(&nbFiles);
	TCHAR** filesArray = new TCHAR*[nbFiles];

	for(int i=0; i<nbFiles; i++){
		filesArray[i] = new TCHAR[MAX_PATH];
	}

	// get filenames
	int realyFiles=0;
	if (m_Id==1){
		realyFiles = NPPM(GETOPENFILENAMESPRIMARY,filesArray,(int)nbFiles);
	}else {
		realyFiles = NPPM(GETOPENFILENAMESSECOND,filesArray,(int)nbFiles); 
	}
	
	// create JS Array and fill it
	IDispatchEx* arr = MyActiveSite::getInstance()->createObj(TEXT("Array"));
	// fill and free allocated Resources
	for(int i=0; i<nbFiles; i++){
		MyActiveSite::getInstance()->pushValueInToArr(arr, filesArray[i]);
		delete[] filesArray[i];
	}

	delete[] filesArray;

	*result = arr;
	return S_OK;
};

HRESULT STDMETHODCALLTYPE CEditorView::get_file(int *value){
	*value = NPPM(GETCURRENTDOCINDEX,0,m_Id-1);
	return S_OK;
};

 HRESULT STDMETHODCALLTYPE CEditorView::put_file( int value){
	NPPM(ACTIVATEDOC,m_Id-1, value);
	return S_OK;
};

HRESULT STDMETHODCALLTYPE CEditorView::get_lang( int *result){
	int bufId = getBufferId();

	*result = NPPM(GETBUFFERLANGTYPE, bufId, 0);
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CEditorView::put_lang( int value){

	int bufId = getBufferId();

	NPPM(SETBUFFERLANGTYPE, bufId, value);
	return S_OK;						
}
HRESULT STDMETHODCALLTYPE CEditorView::get_line( int *result){
	int pos = sci(SCI_GETCURRENTPOS,0,0);
	*result = sci(SCI_LINEFROMPOSITION,pos,0);
	return S_OK;						
}

HRESULT STDMETHODCALLTYPE CEditorView::put_line( int value){
	sci(SCI_GOTOLINE,value,0);
	return S_OK;						
}

HRESULT STDMETHODCALLTYPE CEditorView::get_pos( int *result){
	int pos = sci(SCI_GETCURRENTPOS,0,0);
	*result = bytePosToPos(pos);
	return S_OK;						
}

HRESULT STDMETHODCALLTYPE CEditorView::put_pos( int value){
	int pos = posToBytePos(value);

	sci(SCI_SETCURRENTPOS,pos,0);

	return S_OK;						
}

HRESULT STDMETHODCALLTYPE CEditorView::get_bytePos( int *result){
	*result = sci(SCI_GETCURRENTPOS,0,0);

	return S_OK;						
}

HRESULT STDMETHODCALLTYPE CEditorView::put_bytePos( int value){
	sci(SCI_SETCURRENTPOS,value,0);

	return S_OK;						
}

HRESULT STDMETHODCALLTYPE CEditorView::get_column( int *result){
	int pos = sci(SCI_GETCURRENTPOS,0,0);
	*result = sci(SCI_GETCOLUMN,pos,0);

	return S_OK;						
}

HRESULT STDMETHODCALLTYPE CEditorView::get_anchor( int *result){
	int pos = sci(SCI_GETANCHOR,0,0);

	*result = bytePosToPos(pos);

	return S_OK;						
}

HRESULT STDMETHODCALLTYPE CEditorView::put_anchor(int value){
	int anchor = posToBytePos(value);

	sci(SCI_SETANCHOR,anchor,0);

	return S_OK;						
}

HRESULT STDMETHODCALLTYPE CEditorView::get_byteAnchor( int *result){
	*result = sci(SCI_GETANCHOR, 0, 0);
	return S_OK;						
}

HRESULT STDMETHODCALLTYPE CEditorView::put_byteAnchor(int value){
	sci(SCI_SETANCHOR, value, 0);
	return S_OK;						
}


HRESULT STDMETHODCALLTYPE CEditorView::get_lineCount(int* line){
	*line = sci(SCI_GETLINECOUNT, 0, 0);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditorView::get_lines(IViewLines** result){
	*result = new ViewLines(this);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditorView::get_navBarHidden( VARIANT_BOOL *result){
	*result = m_IndPanel.m_Disabled;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditorView::put_navBarHidden( VARIANT_BOOL value){
	m_IndPanel.SetDisabled(value == TRUE);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEditorView::get_handle(int *result){
	*result = (int)m_Handle;
	return S_OK;
}


LRESULT CEditorView::OnMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
		case WM_NCCALCSIZE:{
			return m_IndPanel.OnNCCalcSize(hwnd, message, wParam, lParam);
		}
		case WM_NCPAINT:{
			return m_IndPanel.OnNCPaint(hwnd, message, wParam, lParam);
		}
		case WM_LBUTTONUP:
			if (m_Listener){
				VARIANT var[1];
				var[0].vt = VT_DISPATCH;
				var[0].pdispVal = this; 
				
				m_Listener->callMethod(TEXT("CLICK"), var, 1);
			}
			break;
		case WM_CONTEXTMENU:
			if (m_Listener){
				VARIANT var[1];
				var[0].vt = VT_DISPATCH;
				var[0].pdispVal = this; 
				
				VARIANT res;
				m_Listener->callMethod(TEXT("CONTEXTMENU"), var, 1, &res);

				if (res.vt == VT_BOOL && res.boolVal != VARIANT_FALSE ){
					return 0; // avoid showing of original context menu
				}

				HRESULT ok = VariantClear(&res);
			}
			break;
	}
	return SCIView::OnMessage(hwnd, message, wParam, lParam);
}