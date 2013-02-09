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
#include <ExDisp.h>
#include <MsHtmHst.h>
#include <MsHtml.h>

#include "..\lasterror.h"

#include "..\MyActiveSite.h"
#include "..\AcceleratorHook.h"

#include "MDocHostUIHandler.h"
#include "MOleClientSite.h"
#include "MOleInPlaceFrame.h"
#include "MOleInPlaceSite.h"

#include "Interfaces.h"

#include "WindowClass.h"
#include "..\SysStr.h"

class Dialog : virtual public CComDispatch<IDialog>, public AcceleratorHook::AcceleratorHandler
{

protected :
	HWND m_Parent;
	HWND m_Hwnd;

	virtual void BeforeCreate();

	ATOM GetWindowClassAtom();

    void destroy();


    bool isCreated() const;

	bool isInParentChain(HWND parent, HWND child) const;

	int getWidth() const;

	int getHeight() const;

	int getClientHeight() const;

	int getClientWidth() const;

	void display(bool toShow = true) const;

	void goToCenter();
	

	void create();

	bool IsClosingAllowed();


	static LRESULT CALLBACK dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	IDispatchEx* m_Cfg;

	virtual LRESULT CALLBACK MessageProc(UINT message, WPARAM wParam, LPARAM lParam);

/**		PUBLIC		*/
public:

	IOleInPlaceActiveObject* m_ActiveObject;

	MOleClientSite    m_OleClientSite; 
	MOleInPlaceFrame  m_OleInPlaceFrame;
	MOleInPlaceSite   m_OleInPlaceSite;
	MDocHostUIHandler m_DocHostUIHandler;

	DWORD m_ExStyle;
	DWORD m_Style;

	HWND hwnd(){
		return m_Hwnd;
	}

	Dialog(IDispatchEx* cfg, HWND parent);

	~Dialog();

	void Init();

	virtual void AfterCreate();

	int execute(MSG* msg);

	void ReleaseBrowser();


#pragma region IDialog INTERFACE IMPLEMENTING

 HRESULT STDMETHODCALLTYPE get_visible( VARIANT_BOOL *result);

 HRESULT STDMETHODCALLTYPE put_visible( VARIANT_BOOL value);

 HRESULT STDMETHODCALLTYPE get_height(  int *result);

 HRESULT STDMETHODCALLTYPE put_height( int value);

 HRESULT STDMETHODCALLTYPE get_width( int *result);

 HRESULT STDMETHODCALLTYPE put_width( int value);

 HRESULT STDMETHODCALLTYPE get_clientHeight( /* [retval][out] */ int *result);

 HRESULT STDMETHODCALLTYPE put_clientHeight( /* [in] */ int value);

 HRESULT STDMETHODCALLTYPE get_clientWidth(/* [retval][out] */ int *result);

 HRESULT STDMETHODCALLTYPE put_clientWidth( /* [in] */ int value);

 HRESULT STDMETHODCALLTYPE get_top( /* [retval][out] */ int *result);

 HRESULT STDMETHODCALLTYPE put_top( /* [in] */ int value);

 HRESULT STDMETHODCALLTYPE get_left( /* [retval][out] */ int *result);

 HRESULT STDMETHODCALLTYPE put_left( /* [in] */ int value);

 HRESULT STDMETHODCALLTYPE get_title( BSTR* result);

 HRESULT STDMETHODCALLTYPE put_title( BSTR* value);

 HRESULT STDMETHODCALLTYPE get_document( IDispatch **result);

 HRESULT STDMETHODCALLTYPE close();

 HRESULT STDMETHODCALLTYPE get_handle(int *result);

 HRESULT STDMETHODCALLTYPE foreground();

#pragma endregion



#pragma region  BROWSER STUFF 

IOleObject*		m_BrowserObject;
IWebBrowser2*	m_WebBrowser2;

long EmbedBrowserObject(HWND hwnd)
{
	LPCLASSFACTORY		pClassFactory;
	RECT				rect;

	pClassFactory = 0;
	if (!CoGetClassObject(CLSID_WebBrowser, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, NULL, IID_IClassFactory, (void **)&pClassFactory) && pClassFactory)
	{
		// Call the IClassFactory's CreateInstance() to create a browser object
		if (!pClassFactory->CreateInstance(0, IID_IOleObject,(void **) &m_BrowserObject))
		{
			// Free the IClassFactory. We need it only to create a browser object instance
			pClassFactory->Release();

			// Give the browser a pointer to my IOleClientSite object
			if (!m_BrowserObject->SetClientSite((IOleClientSite *)&m_OleClientSite))
			{
				m_BrowserObject->SetHostNames(L"HTML Based Dialog", 0);

				::GetClientRect(m_Hwnd, &rect);

				// Let browser object know that it is embedded in an OLE container.
				if (!OleSetContainedObject((struct IUnknown *)m_BrowserObject, TRUE) &&

					// Set the display area of our browser control the same as our window's size
					// and actually put the browser object into our window.
					!m_BrowserObject->DoVerb(OLEIVERB_SHOW, NULL, (IOleClientSite *)&m_OleClientSite, -1, m_Hwnd, &rect) &&

					// Ok, now things may seem to get even trickier, One of those function pointers in the browser's VTable is
					// to the QueryInterface() function. What does this function do? It lets us grab the base address of any
					// other object that may be embedded within the browser object. And this other object has its own VTable
					// containing pointers to more functions we can call for that object.
					//
					// We want to get the base address (ie, a pointer) to the IWebBrowser2 object embedded within the browser
					// object, so we can call some of the functions in the former's table. For example, one IWebBrowser2 function
					// we intend to call below will be Navigate2(). So we call the browser object's QueryInterface to get our
					// pointer to the IWebBrowser2 object.
					!m_BrowserObject->QueryInterface(IID_IWebBrowser2, (void**)&m_WebBrowser2))
				{
					// Ok, now the pointer to our IWebBrowser2 object is in 'webBrowser2', and so its VTable is
					// webBrowser2->lpVtbl.

					// Let's call several functions in the IWebBrowser2 object to position the browser display area
					// in our window. The functions we call are put_Left(), put_Top(), put_Width(), and put_Height().
					// Note that we reference the IWebBrowser2 object's VTable to get pointers to those functions. And
					// also note that the first arg we pass to each is the pointer to the IWebBrowser2 object.
					m_WebBrowser2->put_Left(0);
					m_WebBrowser2->put_Top(0);
					m_WebBrowser2->put_Width(rect.right);
					m_WebBrowser2->put_Height(rect.bottom);

					SysStr aboutBlank(TEXT("about:blank"));
					m_WebBrowser2->Navigate(aboutBlank, NULL, NULL, NULL, NULL);

					// Success
					return(0);
				}
			}

			// Something went wrong setting up the browser!
			//UnEmbedBrowserObject(hwnd);
			return(-4);
		}

		pClassFactory->Release();

		// Can't create an instance of the browser!
		return(-3);
	}

	// Can't get the web browser's IClassFactory!
	return(-2);
}

#pragma endregion



};