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
#include "..\ComBase.h"

class Dialog;

class MOleInPlaceSite : public CComBase<IOleInPlaceSite>{
public:
	Dialog* m_Dialog;
	virtual ULONG STDMETHODCALLTYPE AddRef();

	virtual ULONG STDMETHODCALLTYPE Release();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID *ppv);

	virtual HRESULT STDMETHODCALLTYPE GetWindow( HWND *phwnd);
	        
	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp( BOOL fEnterMode);
	 
	virtual HRESULT STDMETHODCALLTYPE CanInPlaceActivate( );

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivate( );

	virtual HRESULT STDMETHODCALLTYPE OnUIActivate( );

	virtual HRESULT STDMETHODCALLTYPE GetWindowContext( IOleInPlaceFrame **ppFrame, IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect,
		LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo);

	virtual HRESULT STDMETHODCALLTYPE Scroll( SIZE scrollExtant);

	virtual HRESULT STDMETHODCALLTYPE OnUIDeactivate( BOOL fUndoable);

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate( );

	virtual HRESULT STDMETHODCALLTYPE DiscardUndoState( );

	virtual HRESULT STDMETHODCALLTYPE DeactivateAndUndo( );

	virtual HRESULT STDMETHODCALLTYPE OnPosRectChange( LPCRECT lprcPosRect);
};

