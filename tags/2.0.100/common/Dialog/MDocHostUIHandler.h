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
class Dialog;

class MDocHostUIHandler: public  CComBase<IDocHostUIHandler>{
public:
	Dialog* m_Dialog;
	virtual ULONG STDMETHODCALLTYPE AddRef();

	virtual ULONG STDMETHODCALLTYPE Release();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID *ppv);
	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu( 
		/* [in] */ DWORD dwID,
		/* [in] */ POINT *ppt,
		/* [in] */ IUnknown *pcmdtReserved,
		/* [in] */ IDispatch *pdispReserved);
	virtual HRESULT STDMETHODCALLTYPE GetHostInfo( DOCHOSTUIINFO *pInfo);

	virtual HRESULT STDMETHODCALLTYPE ShowUI( 
		/* [in] */ DWORD dwID,
		/* [in] */ IOleInPlaceActiveObject *pActiveObject,
		/* [in] */ IOleCommandTarget *pCommandTarget,
		/* [in] */ IOleInPlaceFrame *pFrame,
		/* [in] */ IOleInPlaceUIWindow *pDoc);

	virtual HRESULT STDMETHODCALLTYPE HideUI( void);

	virtual HRESULT STDMETHODCALLTYPE UpdateUI( void);

	virtual HRESULT STDMETHODCALLTYPE EnableModeless( 
		/* [in] */ BOOL fEnable);

	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate( 
		/* [in] */ BOOL fActivate);

	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate( 
		/* [in] */ BOOL fActivate);

	virtual HRESULT STDMETHODCALLTYPE ResizeBorder( 
		/* [in] */ LPCRECT prcBorder,
		/* [in] */ IOleInPlaceUIWindow *pUIWindow,
		/* [in] */ BOOL fRameWindow);

	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( 
		/* [in] */ LPMSG lpMsg,
		/* [in] */ const GUID *pguidCmdGroup,
		/* [in] */ DWORD nCmdID);

	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath( LPOLESTR *pchKey,DWORD dw);

	virtual HRESULT STDMETHODCALLTYPE GetDropTarget( IDropTarget *pDropTarget,IDropTarget **ppDropTarget);

	virtual HRESULT STDMETHODCALLTYPE GetExternal( IDispatch **ppDispatch);

	virtual HRESULT STDMETHODCALLTYPE TranslateUrl( DWORD dwTranslate,OLECHAR *pchURLIn,OLECHAR **ppchURLOut);

	virtual HRESULT STDMETHODCALLTYPE FilterDataObject(IDataObject *pDO,IDataObject **ppDORet);

};

