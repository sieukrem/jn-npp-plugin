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

#include "Dialog.h"

ULONG STDMETHODCALLTYPE MDocHostUIHandler::AddRef(){ 
	return m_Dialog->AddRef();
}

ULONG STDMETHODCALLTYPE MDocHostUIHandler::Release(){
	return m_Dialog->Release();
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::QueryInterface(REFIID riid, LPVOID *ppv){
	// The browser assumes that our IDocHostUIHandler object is associated with our IOleClientSite
	// object. So it is possible that the browser may call our IDocHostUIHandler's QueryInterface()
	// to ask us to return a pointer to our IOleClientSite, in the same way that the browser calls
	// our IOleClientSite's QueryInterface() to ask for a pointer to our IDocHostUIHandler.
	//
	// Rather than duplicate much of the code in IOleClientSite's QueryInterface, let's just get
	// a pointer to our _IOleClientSiteEx object, substitute it as the 'This' arg, and call our
	// our IOleClientSite's QueryInterface. Note that since our _IDocHostUIHandlerEx is embedded right
	// inside our _IOleClientSiteEx, and comes immediately after the _IOleInPlaceSiteEx, we can employ
	// the following trickery to get the pointer to our _IOleClientSiteEx.
	if (IsEqualIID(riid, IID_IOleClientSite)){
		*ppv =&m_Dialog->m_OleClientSite;
		m_Dialog->m_OleClientSite.AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
} 

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::ShowContextMenu( 
		/* [in] */ DWORD dwID,
		/* [in] */ POINT *ppt,
		/* [in] */ IUnknown *pcmdtReserved,
		/* [in] */ IDispatch *pdispReserved){
   // If desired, we can pop up your own custom context menu here. Of course,
	// we would need some way to get our window handle, so what we'd probably
	// do is like what we did with our IOleInPlaceFrame object. We'd define and create
	// our own IDocHostUIHandlerEx object with an embedded IDocHostUIHandler at the
	// start of it. Then we'd add an extra HWND field to store our window handle.
	// It could look like this:
	//
	// typedef struct _IDocHostUIHandlerEx {
	//		IDocHostUIHandler	ui;		// The IDocHostUIHandler must be first!
	//		HWND				window;
	// } IDocHostUIHandlerEx;
	//
	// Of course, we'd need a unique IDocHostUIHandlerEx object for each window, so
	// EmbedBrowserObject() would have to allocate one of those too. And that's
	// what we'd pass to our browser object (and it would in turn pass it to us
	// here, instead of 'This' being a IDocHostUIHandler *).

	// We will return S_OK to tell the browser not to display its default context menu,
	// or return S_FALSE to let the browser show its default context menu. For this
	// example, we wish to disable the browser's default context menu.
	// return (S_OK);
		return S_FALSE;
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::GetHostInfo( DOCHOSTUIINFO *pInfo){
	pInfo->cbSize = sizeof(DOCHOSTUIINFO);

	// Set some flags. We don't want any 3D border. You can do other things like hide
	// the scroll bar (DOCHOSTUIFLAG_SCROLL_NO), display picture display (DOCHOSTUIFLAG_NOPICS),
	// disable any script running when the page is loaded (DOCHOSTUIFLAG_DISABLE_SCRIPT_INACTIVE),
	// open a site in a new browser window when the user clicks on some link (DOCHOSTUIFLAG_OPENNEWWIN),
	// and lots of other things. See the MSDN docs on the DOCHOSTUIINFO struct passed to us.
	pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER;

	// Set what happens when the user double-clicks on the object. Here we use the default.
	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;

	return(S_OK);
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::ShowUI( 
	/* [in] */ DWORD dwID,
	/* [in] */ IOleInPlaceActiveObject *pActiveObject,
	/* [in] */ IOleCommandTarget *pCommandTarget,
	/* [in] */ IOleInPlaceFrame *pFrame,
	/* [in] */ IOleInPlaceUIWindow *pDoc){
	// We've already got our own UI in place so just return S_OK to tell the browser
	// not to display its menus/toolbars. Otherwise we'd return S_FALSE to let it do
	// that.
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::HideUI( void){
		return S_OK;
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::UpdateUI( void){
	// We update our UI in our window message loop so we don't do anything here.
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::EnableModeless( 
	/* [in] */ BOOL fEnable){
		return S_OK;
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::OnDocWindowActivate( 
	/* [in] */ BOOL fActivate){
		return S_OK;
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::OnFrameWindowActivate( 
	/* [in] */ BOOL fActivate){
		return S_OK;
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::ResizeBorder( 
	/* [in] */ LPCRECT prcBorder,
	/* [in] */ IOleInPlaceUIWindow *pUIWindow,
	/* [in] */ BOOL fRameWindow){
		return S_OK;
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::TranslateAccelerator( 
	/* [in] */ LPMSG lpMsg,
	/* [in] */ const GUID *pguidCmdGroup,
	/* [in] */ DWORD nCmdID){
	// We don't intercept any keystrokes, so we do nothing here. But for example, if we wanted to
	// override the TAB key, perhaps do something with it ourselves, and then tell the browser
	// not to do anything with this keystroke, we'd do:
	//
	//	if (pMsg && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB)
	//	{
	//		// Here we do something as a result of a TAB key press.
	//
	//		// Tell the browser not to do anything with it.
	//		return(S_FALSE);
	//	}
	//
	//	// Otherwise, let the browser do something with this message.
	//return(S_OK);

	// For our example, we want to make sure that the user can invoke some key to popup the context
	// menu, so we'll tell it to ignore all messages.
	return(S_FALSE);
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::GetOptionKeyPath( LPOLESTR *pchKey,DWORD dw){
	// Let the browser use its default registry settings.
	return(S_FALSE);
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::GetDropTarget( IDropTarget *pDropTarget,IDropTarget **ppDropTarget){
	// Return our IDropTarget object associated with this IDocHostUIHandler object. I don't
	// know why we don't do this via UI_QueryInterface(), but we don't.

	// NOTE: If we want/need an IDropTarget interface, then we would have had to setup our own
	// IDropTarget functions, IDropTarget VTable, and create an IDropTarget object. We'd want to put
	// a pointer to the IDropTarget object in our own custom IDocHostUIHandlerEx object (like how
	// we may add an HWND field for the use of UI_ShowContextMenu). So when we created our
	// IDocHostUIHandlerEx object, maybe we'd add a 'idrop' field to the end of it, and
	// store a pointer to our IDropTarget object there. Then we could return this pointer as so:
	//
	// *pDropTarget = ((IDocHostUIHandlerEx FAR *)This)->idrop;
	// return(S_OK);

	// But for our purposes, we don't need an IDropTarget object, so we'll tell whomever is calling
	// us that we don't have one.
	return(S_FALSE);
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::GetExternal( IDispatch **ppDispatch){
	// Return our IDispatch object associated with this IDocHostUIHandler object. I don't
	// know why we don't do this via UI_QueryInterface(), but we don't.

	// NOTE: If we want/need an IDispatch interface, then we would have had to setup our own
	// IDispatch functions, IDispatch VTable, and create an IDispatch object. We'd want to put
	// a pointer to the IDispatch object in our custom _IDocHostUIHandlerEx object (like how
	// we may add an HWND field for the use of UI_ShowContextMenu). So when we defined our
	// _IDocHostUIHandlerEx object, maybe we'd add a 'idispatch' field to the end of it, and
	// store a pointer to our IDispatch object there. Then we could return this pointer as so:
	//
	// *ppDispatch = ((_IDocHostUIHandlerEx FAR *)This)->idispatch;
	// return(S_OK);

	// But for our purposes, we don't need an IDispatch object, so we'll tell whomever is calling
	// us that we don't have one. Note: We must set ppDispatch to 0 if we don't return our own
	// IDispatch object.
	*ppDispatch = 0;
	return(S_FALSE);
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::TranslateUrl( DWORD dwTranslate,OLECHAR *pchURLIn,OLECHAR **ppchURLOut){
	// We don't need to modify the URL. Note: We need to set ppchURLOut to 0 if we don't
	// return an OLECHAR (buffer) containing a modified version of pchURLIn.
	*ppchURLOut = 0;
	return(S_FALSE);
}

HRESULT STDMETHODCALLTYPE MDocHostUIHandler::FilterDataObject(IDataObject *pDO,IDataObject **ppDORet){
	// Return our IDataObject object associated with this IDocHostUIHandler object. I don't
	// know why we don't do this via UI_QueryInterface(), but we don't.

	// NOTE: If we want/need an IDataObject interface, then we would have had to setup our own
	// IDataObject functions, IDataObject VTable, and create an IDataObject object. We'd want to put
	// a pointer to the IDataObject object in our custom _IDocHostUIHandlerEx object (like how
	// we may add an HWND field for the use of UI_ShowContextMenu). So when we defined our
	// _IDocHostUIHandlerEx object, maybe we'd add a 'idata' field to the end of it, and
	// store a pointer to our IDataObject object there. Then we could return this pointer as so:
	//
	// *ppDORet = ((_IDocHostUIHandlerEx FAR *)This)->idata;
	// return(S_OK);

	// But for our purposes, we don't need an IDataObject object, so we'll tell whomever is calling
	// us that we don't have one. Note: We must set ppDORet to 0 if we don't return our own
	// IDataObject object.
	*ppDORet = 0;
	return(S_FALSE);
}


