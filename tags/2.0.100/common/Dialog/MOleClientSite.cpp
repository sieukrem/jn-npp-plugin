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
#include "MOleClientSite.h"
#include "Dialog.h"

ULONG STDMETHODCALLTYPE MOleClientSite::AddRef(){
	return m_Dialog->AddRef();
}

ULONG STDMETHODCALLTYPE MOleClientSite::Release(){
	return m_Dialog->Release();
}


HRESULT STDMETHODCALLTYPE MOleClientSite::QueryInterface(REFIID riid, LPVOID *ppv){
	// It just so happens that the first arg passed to us is our _IOleClientSiteEx struct we allocated
	// and passed to DoVerb() and OleCreate(). Nevermind that 'This' is declared is an IOleClientSite *.
	// Remember that in EmbedBrowserObject(), we allocated our own _IOleClientSiteEx struct, and lied
	// to OleCreate() and DoVerb() -- passing our _IOleClientSiteEx struct and saying it was an
	// IOleClientSite struct. It's ok. An _IOleClientSiteEx starts with an embedded IOleClientSite, so
	// the browser didn't mind. So that's what the browser object is passing us now. The browser doesn't
	// know that it's really an _IOleClientSiteEx struct. But we do. So we can recast it and use it as
	// so here.

	// If the browser is asking us to match IID_IOleClientSite, then it wants us to return a pointer to
	// our IOleClientSite struct. Then the browser will use the VTable in that struct to call our
	// IOleClientSite functions. It will also pass this same pointer to all of our IOleClientSite
	// functions.
	//
	// Actually, we're going to lie to the browser again. We're going to return our own _IOleClientSiteEx
	// struct, and tell the browser that it's a IOleClientSite struct. It's ok. The first thing in our
	// _IOleClientSiteEx is an embedded IOleClientSite, so the browser doesn't mind. We want the browser
	// to continue passing our _IOleClientSiteEx pointer wherever it would normally pass a IOleClientSite
	// pointer.
	// 
	// The IUnknown interface uses the same VTable as the first object in our _IOleClientSiteEx
	// struct (which happens to be an IOleClientSite). So if the browser is asking us to match
	// IID_IUnknown, then we'll also return a pointer to our _IOleClientSiteEx.

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IOleClientSite)){
		*ppv = this;
		this->AddRef();
	}
	// If the browser is asking us to match IID_IOleInPlaceSite, then it wants us to return a pointer to
	// our IOleInPlaceSite struct. Then the browser will use the VTable in that struct to call our
	// IOleInPlaceSite functions.  It will also pass this same pointer to all of our IOleInPlaceSite
	// functions (except for Site_QueryInterface, Site_AddRef, and Site_Release. Those will always get
	// the pointer to our _IOleClientSiteEx.
	//
	// Actually, we're going to lie to the browser. We're going to return our own _IOleInPlaceSiteEx
	// struct, and tell the browser that it's a IOleInPlaceSite struct. It's ok. The first thing in
	// our _IOleInPlaceSiteEx is an embedded IOleInPlaceSite, so the browser doesn't mind. We want the
	// browser to continue passing our _IOleInPlaceSiteEx pointer wherever it would normally pass a
	// IOleInPlaceSite pointer.
	else if (IsEqualIID(riid, IID_IOleInPlaceSite)){
		*ppv =  &m_Dialog->m_OleInPlaceSite;
		m_Dialog->m_OleInPlaceSite.AddRef();
	}
	// If the browser is asking us to match IID_IDocHostUIHandler, then it wants us to return a pointer to
	// our IDocHostUIHandler struct. Then the browser will use the VTable in that struct to call our
	// IDocHostUIHandler functions.  It will also pass this same pointer to all of our IDocHostUIHandler
	// functions (except for Site_QueryInterface, Site_AddRef, and Site_Release. Those will always get
	// the pointer to our _IOleClientSiteEx.
	//
	// Actually, we're going to lie to the browser. We're going to return our own _IDocHostUIHandlerEx
	// struct, and tell the browser that it's a IDocHostUIHandler struct. It's ok. The first thing in
	// our _IDocHostUIHandlerEx is an embedded IDocHostUIHandler, so the browser doesn't mind. We want the
	// browser to continue passing our _IDocHostUIHandlerEx pointer wherever it would normally pass a
	// IDocHostUIHandler pointer. My, we're really playing dirty tricks on the browser here. heheh.
	else if (IsEqualIID(riid, IID_IDocHostUIHandler)){
		*ppv = &m_Dialog->m_DocHostUIHandler;
		m_Dialog->m_DocHostUIHandler.AddRef();
	}
	// For other types of objects the browser wants, just report that we don't have any such objects.
	// NOTE: If you want to add additional functionality to your browser hosting, you may need to
	// provide some more objects here. You'll have to investigate what the browser is asking for
	// (ie, what REFIID it is passing).
	else
	{
		*ppv = 0;
		return(E_NOINTERFACE);
	}

	return(S_OK);
} 

HRESULT STDMETHODCALLTYPE MOleClientSite::SaveObject(){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MOleClientSite::GetMoniker( DWORD dwAssign,    DWORD dwWhichMoniker,    IMoniker **ppmk){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MOleClientSite::GetContainer( IOleContainer **ppContainer){
	// Tell the browser that we are a simple object and don't support a container
	*ppContainer = 0;

	return(E_NOINTERFACE);
}

HRESULT STDMETHODCALLTYPE MOleClientSite::ShowObject(){
	return(NOERROR);
}

HRESULT STDMETHODCALLTYPE MOleClientSite::OnShowWindow(BOOL fShow){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MOleClientSite::RequestNewObjectLayout(){
	return E_NOTIMPL;
}




