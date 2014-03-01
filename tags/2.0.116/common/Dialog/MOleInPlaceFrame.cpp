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
#include "MOleInPlaceFrame.h"
#include "Dialog.h"

ULONG STDMETHODCALLTYPE MOleInPlaceFrame::AddRef(){
	return m_Dialog->AddRef();
}

ULONG STDMETHODCALLTYPE MOleInPlaceFrame::Release(){
	return m_Dialog->Release();
}

HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::QueryInterface(REFIID riid, LPVOID *ppv){
	return E_NOTIMPL;
} 
HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::GetWindow( HWND *phwnd){
	*phwnd = m_Dialog->hwnd();
	return(S_OK);
}
        
HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::ContextSensitiveHelp( BOOL fEnterMode){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::GetBorder( LPRECT lprectBorder){
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::RequestBorderSpace( LPCBORDERWIDTHS pborderwidths){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::SetBorderSpace( LPCBORDERWIDTHS pborderwidths){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::SetActiveObject(IOleInPlaceActiveObject *pActiveObject,LPCOLESTR pszObjName){
	if (m_Dialog->m_ActiveObject != NULL){
		m_Dialog->m_ActiveObject->Release();
	}

	m_Dialog->m_ActiveObject = pActiveObject;

	if (m_Dialog->m_ActiveObject != NULL)
		m_Dialog->m_ActiveObject->AddRef();
	
	return S_OK;
}
        

HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::InsertMenus( HMENU hmenuShared,LPOLEMENUGROUPWIDTHS lpMenuWidths){
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::SetMenu( HMENU hmenuShared,HOLEMENU holemenu,HWND hwndActiveObject){
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::RemoveMenus( HMENU hmenuShared){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::SetStatusText( LPCOLESTR pszStatusText){
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::EnableModeless(BOOL fEnable){
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceFrame::TranslateAccelerator( LPMSG lpmsg, WORD wID){
	return E_NOTIMPL;
}



