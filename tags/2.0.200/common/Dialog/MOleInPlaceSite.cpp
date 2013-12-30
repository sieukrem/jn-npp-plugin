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
#include "MOleInPlaceSite.h"
#include "Dialog.h"


ULONG STDMETHODCALLTYPE MOleInPlaceSite::AddRef(){
	return m_Dialog->AddRef();
}

ULONG STDMETHODCALLTYPE MOleInPlaceSite::Release(){
	return m_Dialog->Release();
}

HRESULT STDMETHODCALLTYPE MOleInPlaceSite::QueryInterface(REFIID riid, LPVOID *ppv){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceSite::GetWindow( HWND *phwnd){
	*phwnd = m_Dialog->hwnd();
	return(S_OK);
}
        
HRESULT STDMETHODCALLTYPE MOleInPlaceSite::ContextSensitiveHelp( BOOL fEnterMode){
	return E_NOTIMPL;
}
 
HRESULT STDMETHODCALLTYPE MOleInPlaceSite::CanInPlaceActivate( ){
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceSite::OnInPlaceActivate( ){
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceSite::OnUIActivate( ){
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceSite::GetWindowContext( IOleInPlaceFrame **ppFrame, IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect,
	LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo){

	*ppFrame = (IOleInPlaceFrame *)&m_Dialog->m_OleInPlaceFrame;
	m_Dialog->m_OleInPlaceFrame.AddRef();

	// We have no OLEINPLACEUIWINDOW
	*ppDoc = 0;

	// Fill in some other info for the browser
	lpFrameInfo->fMDIApp = FALSE;
	lpFrameInfo->hwndFrame = m_Dialog->hwnd();
	lpFrameInfo->haccel = 0;
	lpFrameInfo->cAccelEntries = 0;

	// Give the browser the dimensions of where it can draw. We give it our entire window to fill.
	// We do this in InPlace_OnPosRectChange() which is called right when a window is first
	// created anyway, so no need to duplicate it here.
	//	GetClientRect(lpFrameInfo->hwndFrame, lprcPosRect);
	//	GetClientRect(lpFrameInfo->hwndFrame, lprcClipRect);

	return(S_OK);
}

HRESULT STDMETHODCALLTYPE MOleInPlaceSite::Scroll( SIZE scrollExtant){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceSite::OnUIDeactivate( BOOL fUndoable){
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceSite::OnInPlaceDeactivate( ){
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceSite::DiscardUndoState( ){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceSite::DeactivateAndUndo( ){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MOleInPlaceSite::OnPosRectChange( LPCRECT lprcPosRect){
	IOleInPlaceObject	*inplace;

	// We need to get the browser's IOleInPlaceObject object so we can call its SetObjectRects
	// function.
	if (!m_Dialog->m_BrowserObject->QueryInterface(IID_IOleInPlaceObject, (void**)&inplace))
	{
		// Give the browser the dimensions of where it can draw.
		inplace->SetObjectRects(lprcPosRect, lprcPosRect);
		inplace->Release();
	}

	return(S_OK);
}

